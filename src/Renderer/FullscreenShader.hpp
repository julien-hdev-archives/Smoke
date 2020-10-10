#pragma once

#include <QtGui/QOpenGLShaderProgram>

class FullscreenShader {
public:
	FullscreenShader(const std::string& fragmentShaderSourceCode);
	void draw();

private:
	QOpenGLShaderProgram _shaderProgram;
};