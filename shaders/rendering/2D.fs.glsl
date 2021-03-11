#version 430

in vec2 vUV;
uniform float u_AspectRatio;
out vec4 fragColor;

void main()
{
    vec3 col = vec3(0.5);
    fragColor = vec4(col, 1.);
}