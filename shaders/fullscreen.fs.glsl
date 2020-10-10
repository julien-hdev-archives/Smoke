#version 330

varying vec2 vUV;

out vec4 fFragColor;

uniform float iTime;

void main() {
    fFragColor = vec4(fract(vUV + vec2(iTime, 0.)), 0., 1.);
}