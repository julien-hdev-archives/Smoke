//? #version 330 // More info on those special comments : https://marketplace.visualstudio.com/items?itemName=DanielScherzer.GLSL

//? float SDF(vec3 p);

uniform float u_AbsorptionCoefficient;
uniform float u_LightAttenuationFactor;
uniform float u_AbsorptionCutoff;
uniform float u_MarchMultiplier;
uniform int   u_MaxVolumeMarchSteps;
uniform int   u_MaxVolumeLightMarchSteps;
uniform int   u_MaxSdfSphereSteps;
uniform int   u_MaxOpaqueShadowMarchSteps;

varying vec2 vUV;
uniform float u_AspectRatio;
uniform vec3 u_CamX;
uniform vec3 u_CamY;
uniform vec3 u_CamZ;
uniform vec3 u_CamPos;
uniform float u_FocalLength;

// Adapted from Christopher Wallis' great article https://wallisc.github.io/rendering/2020/05/02/Volumetric-Rendering-Part-2.html
#define PI 3.141592654

#define NUM_LIGHTS 3
#define NUM_LIGHT_COLORS 3

#define CHECKER_FLOOR_MATERIAL_ID 0
#define LIGHT_BASE_MATERIAL_ID 1
#define NUM_MATERIALS (LIGHT_BASE_MATERIAL_ID + NUM_LIGHTS)

#define INVALID_MATERIAL_ID int(-1)
#define LARGE_NUMBER 1e20
#define EPSILON 0.0001
#define CAST_VOLUME_SHADOW_ON_OPAQUES 1

struct OrbLightDescription
{
    vec3 Position;
    float Radius;
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
    orbLight.Radius = 0.8f;

    return orbLight;
}

float GetLightAttenuation(float distanceToLight)
{
    return 1.0 / pow(distanceToLight, u_LightAttenuationFactor);
}
   

#define MATERIAL_IS_LIGHT_SOURCE 0x1
struct Material
{
    vec3 albedo;
    int flags;
};
    
Material NormalMaterial(vec3 albedo, int flags)
{
    return Material(albedo, flags);
}

bool IsLightSource(in Material m)
{
    return (m.flags & MATERIAL_IS_LIGHT_SOURCE) != 0;
}

Material GetMaterial(int materialID, vec3 position)
{
    Material materials[NUM_MATERIALS];
	materials[CHECKER_FLOOR_MATERIAL_ID] = NormalMaterial(vec3(0.6, 0.6, 0.7), 0);
    for(int lightIndex = 0; lightIndex < NUM_LIGHTS; lightIndex++)
    {
        materials[LIGHT_BASE_MATERIAL_ID + lightIndex] = NormalMaterial(GetLight(lightIndex).LightColor, MATERIAL_IS_LIGHT_SOURCE);
    }
    
    Material mat;
    if(materialID < int(NUM_MATERIALS))
    {
        mat = materials[materialID];
    }
    else
    {
        // Should never get hit
   		return materials[0];
    }
    
    if(materialID == CHECKER_FLOOR_MATERIAL_ID)
    {
        vec2 uv = position.xz / 13.0;
        uv = vec2(uv.x < 0.0 ? abs(uv.x) + 1.0 : uv.x, uv.y < 0.0 ? abs(uv.y) + 1.0 : uv.y);
        if((int(uv.x) % 2 == 0 && int(uv.y) % 2 == 0) || (int(uv.x) % 2 == 1 && int(uv.y) % 2 == 1))
        {
            mat.albedo = vec3(1, 1, 1) * 0.7;
        }
    }

    return mat;    
}

// https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-plane-and-ray-disk-intersection
float PlaneIntersection(vec3 rayOrigin, vec3 rayDirection, vec3 planeOrigin, vec3 planeNormal, out vec3 normal) 
{ 
    float t = -1.0f;
    normal = planeNormal;
    float denom = dot(-planeNormal, rayDirection); 
    if (denom > EPSILON) { 
        vec3 rayToPlane = planeOrigin - rayOrigin; 
        return dot(rayToPlane, -planeNormal) / denom; 
    } 
 
    return t; 
} 
    
float SphereIntersection(
    in vec3 rayOrigin, 
    in vec3 rayDirection, 
    in vec3 sphereCenter, 
    in float sphereRadius, 
    out vec3 normal)
{
      vec3 eMinusC = rayOrigin - sphereCenter;
      float dDotD = dot(rayDirection, rayDirection);

      float discriminant = dot(rayDirection, (eMinusC)) * dot(rayDirection, (eMinusC))
         - dDotD * (dot(eMinusC, eMinusC) - sphereRadius * sphereRadius);

      if (discriminant < 0.0) 
         return -1.0;

      float firstIntersect = (dot(-rayDirection, eMinusC) - sqrt(discriminant))
             / dDotD;
      
      float t = firstIntersect;
    
      normal = normalize(rayOrigin + rayDirection * t - sphereCenter);
      return t;
}


void UpdateIfIntersected(
    inout float t,
    in float intersectionT, 
    in vec3 intersectionNormal,
    in int intersectionMaterialID,
    out vec3 normal,
    out int materialID
	)
{    
    if(intersectionT > EPSILON && intersectionT < t)
    {
		normal = intersectionNormal;
        materialID = intersectionMaterialID;
        t = intersectionT;
    }
}

float IntersectOpaqueScene(in vec3 rayOrigin, in vec3 rayDirection, out int materialID, out vec3 normal)
{
    float intersectionT = LARGE_NUMBER;
    vec3 intersectionNormal = vec3(0, 0, 0);

    float t = LARGE_NUMBER;
    normal = vec3(0, 0, 0);
    materialID = INVALID_MATERIAL_ID;

    for(int lightIndex = 0; lightIndex < NUM_LIGHTS; lightIndex++)
    {
        UpdateIfIntersected(
            t,
            SphereIntersection(rayOrigin, rayDirection, GetLight(lightIndex).Position, GetLight(lightIndex).Radius, intersectionNormal),
            intersectionNormal,
            LIGHT_BASE_MATERIAL_ID + lightIndex,
            normal,
            materialID);
    }

    
    UpdateIfIntersected(
        t,
        PlaneIntersection(rayOrigin, rayDirection, vec3(0, 0, 0), vec3(0, 1, 0), intersectionNormal),
        intersectionNormal,
        CHECKER_FLOOR_MATERIAL_ID,
        normal,
        materialID);

    
    return t;
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

void CalculateLighting(vec3 position, vec3 normal, vec3 reflectionDirection, Material material, inout vec3 color)
{
    for(int lightIndex = 0; lightIndex < NUM_LIGHTS; lightIndex++)
    {
        vec3 lightDirection = (GetLight(lightIndex).Position - position);
        float lightDistance = length(lightDirection);
        lightDirection /= lightDistance;

        vec3 lightColor = GetLight(lightIndex).LightColor * GetLightAttenuation(lightDistance); 

        float lightVisiblity = 1.0;
        #if CAST_VOLUME_SHADOW_ON_OPAQUES
        if(!IsColorInsignificant(lightColor))
        {
            float shadowMarchSize = 0.65f * u_MarchMultiplier;
            lightVisiblity = GetLightVisiblity(position, lightDirection, lightDistance, u_MaxOpaqueShadowMarchSteps, shadowMarchSize); 
        }
        #endif
        
        color += lightVisiblity * lightColor * pow(max(dot(reflectionDirection, lightDirection), 0.0), 4.0);
        color += lightVisiblity * lightColor * Diffuse(normal, lightDirection, material.albedo);
    
    }
    color += GetAmbientLight() * material.albedo;
}

vec3 Render( in vec3 rayOrigin, in vec3 rayDirection)
{
    float depth = LARGE_NUMBER;
    vec3 opaqueColor = vec3(0.0f);
    
    vec3 normal;
    float t;
    int materialID = INVALID_MATERIAL_ID;
    t = IntersectOpaqueScene(rayOrigin, rayDirection, materialID, normal);
    if( materialID != INVALID_MATERIAL_ID )
    {
        // Defer lighting calculations after volume lighting so we can 
        // avoid doing shadow tracing on opaque objects that aren't visible anyways
        depth = t;
    }
    
    float volumeDepth = IntersectVolumetric(rayOrigin, rayDirection, depth);
    float opaqueVisiblity = 1.0f;
    vec3 volumetricColor = vec3(0.0f);
    if(volumeDepth > 0.0)
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
    
    if( materialID != INVALID_MATERIAL_ID && opaqueVisiblity > u_AbsorptionCutoff)
    {
        vec3 position = rayOrigin + t*rayDirection;
        Material material = GetMaterial(materialID, position);
		if(IsLightSource(material))
        {
            opaqueColor = min(material.albedo, vec3(1.0));
        }       
        else
        {
    	    vec3 reflectionDirection = reflect( rayDirection, normal);
	        CalculateLighting(position, normal, reflectionDirection, material, opaqueColor);
        }
    }
    
    return min(volumetricColor, 1.0f) + opaqueVisiblity * opaqueColor;
}

mat3 GetViewMatrix(float xRotationFactor)
{ 
   float xRotation = ((1.0 - xRotationFactor) - 0.5) * PI * 0.4 + PI * 0.25;
   return mat3( cos(xRotation), 0.0, sin(xRotation),
                0.0,           1.0, 0.0,    
                -sin(xRotation),0.0, cos(xRotation));
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
    gl_FragColor = vec4( GammaCorrect(clamp(color, 0.0, 1.0)), 1.0 );
}