#include "FullscreenShader.hpp"

#include "Renderer/Renderer.hpp"

FullscreenShader::FullscreenShader() {
    Renderer::Initialize(); // TODO move me elsewhere
}

void FullscreenShader::compile(const std::string& fragmentShaderSourceCode) {
    if (!_shaderProgram.create())
        qFatal("Unable to create shader program.");
    if (!_shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, "shaders/fullscreen.vs.glsl"))
        qFatal("Vertex shader compilation failed.");
    if (!_shaderProgram.addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSourceCode.c_str()))
        qFatal("Fragment shader compilation failed.");
    if (!_shaderProgram.link())
        qFatal("Shader program not linked.");
    Renderer::SetupShaderForFullScreen(_shaderProgram);
}

void FullscreenShader::draw() {
    Renderer::DrawFullScreen(_shaderProgram);
}