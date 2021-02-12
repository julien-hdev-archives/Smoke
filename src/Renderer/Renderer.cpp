#include "Renderer.hpp"

#include <QtGui/QOpenGLFunctions>
#include <QtGui/QOpenGLShaderProgram>
#include <vector>

QOpenGLBuffer Renderer::s_fullscreenPosAndUV_VBO
    = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
QOpenGLVertexArrayObject Renderer::s_fullscreenPosAndUV_VAO;

void
Renderer::Initialize()
{
    static bool bInitialized = false;
    if (!bInitialized)
    {
        // Vertex array
        if (!s_fullscreenPosAndUV_VAO.create())
            qFatal("Unable to create VAO.");
        s_fullscreenPosAndUV_VAO.bind();
        // Vertex buffer
        float vertices[]
            = { // Pos       UV
                -1.f, 1.f, 0.f, 0.f, 3.f, 1.f, 2.f, 0.f, -1.f, -3.f, 0.f, 2.f
              };

        if (!s_fullscreenPosAndUV_VBO.create())
            qFatal("Unable to create vertex buffer.");
        s_fullscreenPosAndUV_VBO.bind();
        s_fullscreenPosAndUV_VBO.setUsagePattern(QOpenGLBuffer::StaticDraw);
        s_fullscreenPosAndUV_VBO.allocate(vertices, sizeof(vertices));
        //
        s_fullscreenPosAndUV_VAO.release();
        bInitialized = true;
    }
}

void
Renderer::SetupShaderForFullScreen(QOpenGLShaderProgram &shader)
{
    s_fullscreenPosAndUV_VAO.bind();
    //
    const int VERTEX_ATTR_POS = 0;
    const int VERTEX_ATTR_UV = 1;
    shader.bind();
    shader.enableAttributeArray(VERTEX_ATTR_POS);
    shader.enableAttributeArray(VERTEX_ATTR_UV);
    shader.setAttributeArray(VERTEX_ATTR_POS, GL_FLOAT, (void *)0, 2,
                             4 * sizeof(float));
    shader.setAttributeArray(VERTEX_ATTR_UV, GL_FLOAT,
                             (void *)(2 * sizeof(float)), 2,
                             4 * sizeof(float));
    //
    s_fullscreenPosAndUV_VAO.release();
}

void
Renderer::DrawFullScreen(QOpenGLShaderProgram &shader)
{
    s_fullscreenPosAndUV_VAO.bind();
    shader.bind();
    QOpenGLContext::currentContext()->functions()->glDrawArrays(GL_TRIANGLES,
                                                                0, 3);
    s_fullscreenPosAndUV_VAO.release();
}