#include "MyQuickFBORenderer.hpp"

#include "MyQuickFBO.hpp"
#include <QtGui/QOpenGLFramebufferObject>
#include <QtGui/QOpenGLFunctions>
#include <QtQuick/QQuickWindow>

MyQuickFBORenderer::MyQuickFBORenderer(QQuickWindow &window) : _window(window)
{}

void
MyQuickFBORenderer::synchronize(QQuickFramebufferObject *item)
{
    MyQuickFBO *quickFBO = reinterpret_cast<MyQuickFBO *>(item);

    _sdfRendererParams = quickFBO->sdfRenderer_Params();
    quickFBO->camera().update();
    _cameraInfos = quickFBO->camera().getInfos();
}

void
MyQuickFBORenderer::render()
{
    QOpenGLFunctions *functions
        = QOpenGLContext::currentContext()->functions();
    functions->glDisable(GL_DEPTH_TEST);

    //_sdfRenderer.render(_cameraInfos, _sdfRendererParams);
    //_sdfRenderer2D.render(_simulation.gridSize());
    
    unsigned char data[] = {
        0, 255, 255, 255,   // Pixel at (0, 0)
        255, 255, 255, 255, // Pixel at (1, 0)
        255, 255, 0, 255,   // Pixel at (2, 0)
        255, 255, 255, 255, // Pixel at (0, 1)
        255, 255, 255, 255, // Pixel at (1, 1)
        255, 255, 255, 255  // Pixel at (2, 1)
    }; 
    _imageRenderer.render(3, 2, data);
    _simulation.update();
    update();
    _window.resetOpenGLState();
}

QOpenGLFramebufferObject *
MyQuickFBORenderer::createFramebufferObject(const QSize &size)
{
    QOpenGLFramebufferObjectFormat format;
    return new QOpenGLFramebufferObject(size, format);
}