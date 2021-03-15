#version 430

in vec2 vUV;
out vec4 fragColor;

void main()
{
    vec2 uv = vUV;
    vec3 col = vec3(uv, 0.);
    fragColor = vec4(col, 1.);
}