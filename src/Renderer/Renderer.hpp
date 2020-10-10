#pragma once

#include <QtGui/QOpenGLBuffer>
#include <QtGui/QOpenGLVertexArrayObject>

class QOpenGLShaderProgram;

class Renderer {
public:
	static void Initialize();
	static void DrawFullScreen(QOpenGLShaderProgram& shader);
	static void SetupShaderForFullScreen(QOpenGLShaderProgram& shader);

private:
	static QOpenGLBuffer s_fullscreenPosAndUV_VBO;
	static QOpenGLVertexArrayObject s_fullscreenPosAndUV_VAO;
};