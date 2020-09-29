#version 330

layout(location = 0) in vec4 aVertexPosition;

void main() {
    gl_Position = aVertexPosition;
}
