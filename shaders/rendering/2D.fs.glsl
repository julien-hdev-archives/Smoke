#version 430

in vec2 vUV;
uniform float u_AspectRatio;
uniform int u_GridSize;
out vec4 fragColor;

layout(std430, binding = 0) buffer Buffer
{
    float densities[];
};

void main()
{
    vec2 uv = vUV;
    //uv.x *= u_AspectRatio;

    ivec2 idx = ivec2(floor(uv * u_GridSize));

    vec3 col = vec3(densities[idx.x + idx.y * u_GridSize]);
    fragColor = vec4(col, 1.);
}