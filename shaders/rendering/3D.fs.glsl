//? #version 330 // More info on those special comments : https://marketplace.visualstudio.com/items?itemName=DanielScherzer.GLSL

//? float SDF(vec3 p);

uniform float u_AbsorptionCoefficient;
uniform float u_AbsorptionCutoff;
uniform float u_MarchMultiplier;
uniform int   u_MaxVolumeMarchSteps;
uniform int   u_MaxVolumeLightMarchSteps;
uniform int   u_MaxSdfSphereSteps;
uniform int   u_MaxOpaqueShadowMarchSteps;

in vec2 vUV;
uniform float u_AspectRatio;
uniform vec3 u_CamX;
uniform vec3 u_CamY;
uniform vec3 u_CamZ;
uniform vec3 u_CamPos;
uniform float u_FocalLength;

out vec4 fragColor;

//? uniform float iTime;

// Adapted from Christopher Wallis' great article https://wallisc.github.io/rendering/2020/05/02/Volumetric-Rendering-Part-2.html
#define PI 3.141592654

#define NUM_LIGHTS 3
#define NUM_LIGHT_COLORS 3

#define INVALID_MATERIAL_ID int(-1)
#define LARGE_NUMBER 1e20
#define EPSILON 0.0001
#define CAST_VOLUME_SHADOW_ON_OPAQUES 1

struct OrbLightDescription
{
    vec3 Position;
    vec3 LightColor;
};

vec3 GetLightColor(int lightIndex)
{
    switch(lightIndex % NUM_LIGHT_COLORS)
    {
        case 0: return vec3(1, 0.0, 1.0);
        case 1: return vec3(0, 1.0, 0.0);
    }
    return vec3(0, 0.0, 1.0);
}

OrbLightDescription GetLight(int lightIndex)
{
    const float lightMultiplier = 17.0f;
    float theta = iTime * 0.7 + float(lightIndex) * PI * 2.0 / float(NUM_LIGHT_COLORS);
    float radius = 18.5f;
    
    OrbLightDescription orbLight;
    orbLight.Position = vec3(radius * cos(theta), 6.0 + sin(theta * 2.0) * 2.5, radius * sin(theta));
    orbLight.LightColor = GetLightColor(lightIndex) * lightMultiplier;

    return orbLight;
}

float GetLightAttenuation(float distanceToLight)
{
    return 1.0 / pow(distanceToLight, 2.);
}

float IntersectVolumetric(in vec3 rayOrigin, in vec3 rayDirection, float maxT)
{
    // Precision isn't super important, just want a decent starting point before 
    // ray marching with fixed steps
	float precis = 0.5; 
    float t = 0.0f;
    for(int i=0; i<u_MaxSdfSphereSteps; i++ )
    {
	    float result = SDF( rayOrigin+rayDirection*t);
        if( result < (precis) || t>maxT ) break;
        t += result;
    }
    return ( t>=maxT ) ? -1.0 : t;
}

vec3 Diffuse(in vec3 normal, in vec3 lightVec, in vec3 diffuse)
{
    float nDotL = dot(normal, lightVec);
    return clamp(nDotL * diffuse, 0.0, 1.0);
}

vec3 GetAmbientLight()
{
	return 1.2 * vec3(0.03, 0.018, 0.018);
}

float GetFogDensity(vec3 position, float sdfDistance)
{
    const float maxSDFMultiplier = 1.0;
    bool insideSDF = sdfDistance < 0.0;
    float sdfMultiplier = insideSDF ? min(abs(sdfDistance), maxSDFMultiplier) : 0.0;

    return sdfMultiplier;
}

float BeerLambert(float absorption, float dist)
{
    return exp(-absorption * dist);
}

float GetLightVisiblity(in vec3 rayOrigin, in vec3 rayDirection, in float maxT, in int maxSteps, in float marchSize)
{
    float t = 0.0f;
    float lightVisibility = 1.0f;
    float signedDistance = 0.0;
    for(int i = 0; i < maxSteps; i++)
    {                       
        t += max(marchSize, signedDistance);
        if(t > maxT || lightVisibility < u_AbsorptionCutoff) break;

        vec3 position = rayOrigin + t*rayDirection;

        signedDistance = SDF(position);
        if(signedDistance < 0.0)
        {
            lightVisibility *= BeerLambert(u_AbsorptionCoefficient * GetFogDensity(position, signedDistance), marchSize);
        }
    }
    return lightVisibility;
}


float Luminance(vec3 color)
{
    return (color.r * 0.3) + (color.g * 0.59) + (color.b * 0.11);
}

bool IsColorInsignificant(vec3 color)
{
    const float minValue = 0.009;
    return Luminance(color) < minValue;
}

vec3 Render(vec3 rayOrigin, vec3 rayDirection)
{
    float depth = LARGE_NUMBER;
    vec3 opaqueColor = vec3(0.);
    
    float volumeDepth = IntersectVolumetric(rayOrigin, rayDirection, depth);
    float opaqueVisiblity = 1.0f;
    vec3 volumetricColor = vec3(0.0f);
    if(volumeDepth > 0.)
    {
        const vec3 volumeAlbedo = vec3(0.8);
        float marchSize = 0.6f * u_MarchMultiplier;
        float distanceInVolume = 0.0f;
        float signedDistance = 0.0;
        for(int i = 0; i < u_MaxVolumeMarchSteps; i++)
        {
            volumeDepth += max(marchSize, signedDistance);
            if(volumeDepth > depth || opaqueVisiblity < u_AbsorptionCutoff) break;
            
            vec3 position = rayOrigin + volumeDepth*rayDirection;

            signedDistance = SDF(position);
			if(signedDistance < 0.0f)
            {
                distanceInVolume += marchSize;
                float previousOpaqueVisiblity = opaqueVisiblity;
                opaqueVisiblity *= BeerLambert(u_AbsorptionCoefficient * GetFogDensity(position, signedDistance), marchSize);
                float absorptionFromMarch = previousOpaqueVisiblity - opaqueVisiblity;
                
                for(int lightIndex = 0; lightIndex < NUM_LIGHTS; lightIndex++)
    			{
                    float lightVolumeDepth = 0.0f;
                    vec3 lightDirection = (GetLight(lightIndex).Position - position);
                    float lightDistance = length(lightDirection);
                    lightDirection /= lightDistance;
                    
                    vec3 lightColor = GetLight(lightIndex).LightColor * GetLightAttenuation(lightDistance); 
                    if(IsColorInsignificant(lightColor)) continue;
                    
                    float lightMarchSize = 0.65f * u_MarchMultiplier;
                    float lightVisiblity = GetLightVisiblity(position, lightDirection, lightDistance, u_MaxVolumeLightMarchSteps, lightMarchSize); 
                    volumetricColor += absorptionFromMarch * lightVisiblity * volumeAlbedo * lightColor;
                }
                volumetricColor += absorptionFromMarch * volumeAlbedo * GetAmbientLight();
            }
        }
    }
    
    return volumetricColor + opaqueVisiblity * opaqueColor;
}

vec3 GammaCorrect(vec3 color) 
{
    return pow(color, vec3(1.0/2.2));
}

void main()
{
    vec3 ro = u_CamPos;
    vec3 rd = normalize(
          u_CamX * (vUV.x - 0.5) * u_AspectRatio
        + u_CamY * (vUV.y - 0.5)
        - u_CamZ * u_FocalLength
    );
    
    vec3 color = Render(ro, rd);
    fragColor = vec4(GammaCorrect(color), 1.);
}