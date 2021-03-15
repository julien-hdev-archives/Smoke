#version 430

in vec2 vUV;
out vec4 fragColor;

uniform sampler2D u_Texture;

void main()
{
    vec2 uv = vUV;
    vec3 col = texture(u_Texture, uv).xyz;
    fragColor = vec4(col, 1.);
}