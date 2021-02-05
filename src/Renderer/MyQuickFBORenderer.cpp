#include "MyQuickFBORenderer.hpp"

#include "MyQuickFBO.hpp"
#include <QtQuick/QQuickWindow>
#include <QtGui/QOpenGLFunctions>
#include <QtGui/QOpenGLFramebufferObject>

MyQuickFBORenderer::MyQuickFBORenderer(QQuickWindow& window)
    : _window(window)
{}

void MyQuickFBORenderer::synchronize(QQuickFramebufferObject* item) {
    MyQuickFBO* quickFBO = reinterpret_cast<MyQuickFBO*>(item);

    _sdfRendererParams = quickFBO->sdfRenderer_Params();
    quickFBO->camera().update();
    _cameraInfos = quickFBO->camera().getInfos();
}

void MyQuickFBORenderer::render() {
    QOpenGLFunctions* functions = QOpenGLContext::currentContext()->functions();
    functions->glDisable(GL_DEPTH_TEST);

    _sdfRenderer.render(_cameraInfos, _sdfRendererParams);
    update();
    _window.resetOpenGLState();
}

QOpenGLFramebufferObject* MyQuickFBORenderer::createFramebufferObject(const QSize& size) {
    QOpenGLFramebufferObjectFormat format;
    format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
    return new QOpenGLFramebufferObject(size, format);
}