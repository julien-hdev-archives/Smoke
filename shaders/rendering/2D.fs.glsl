#version 430

uniform float u_AspectRatio;
uniform int u_GridSize;

in vec2 vUV;
out vec4 fragColor;

layout(std430, binding = 0) buffer Buffer
{
    float density[];
};

void main()
{
    vec2 uv = vUV;
    //uv.x *= u_AspectRatio;

    ivec2 idx = ivec2(floor(uv * u_GridSize));

    vec3 col = vec3(density[idx.x + idx.y * u_GridSize]);
    fragColor = vec4(col, 1.);
}