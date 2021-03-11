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
    _sdfRenderer2D.render(_simulation.gridSize());
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