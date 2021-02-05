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
    functions->glClearColor(1.f, 0.f, 0.f, 1.f);
    functions->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    _sdfRenderer.render(_cameraInfos, _sdfRendererParams);
    update();
    _window.resetOpenGLState();
}

QOpenGLFramebufferObject* MyQuickFBORenderer::createFramebufferObject(const QSize& size) {
    QOpenGLFramebufferObjectFormat format;
    format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
    return new QOpenGLFramebufferObject(size, format);
}