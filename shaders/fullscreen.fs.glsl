#version 330

varying vec2 vUV;

out vec4 fFragColor;

void main() {
    fFragColor = vec4(vUV, 0., 1.);
}