#pragma once

#include <QtGui/QOpenGLShaderProgram>

class FullscreenShader {
public:
	FullscreenShader(const std::string& fragmentShaderSourceCode);
	void draw();

	inline const QOpenGLShaderProgram& get() const { return _shaderProgram; }
	inline       QOpenGLShaderProgram& get()       { return _shaderProgram; }

private:
	QOpenGLShaderProgram _shaderProgram;
};