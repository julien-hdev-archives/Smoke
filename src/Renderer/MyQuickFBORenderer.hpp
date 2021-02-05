#pragma once

#include <QtQuick/QQuickFramebufferObject>
#include <QtQuick/QQuickWindow>
#include <QtGui/QOpenGLFunctions>
// #include <QtGui/QOpenGLContext>
#include <QtGui/QOpenGLFramebufferObject>
#include "SdfRenderer.hpp"
#include "SdfRenderer_Params.hpp"

class MyQuickFBORenderer : public QQuickFramebufferObject::Renderer {

public:
    MyQuickFBORenderer() = default;
    ~MyQuickFBORenderer() = default;

    void synchronize(QQuickFramebufferObject* item) override {
        MyQuickFBO* quickFBO = static_cast<MyQuickFBO*>(item);
        _window = item->window();
        quickFBO->camera().update();
        _cameraInfos = quickFBO->camera().getInfos();
        _sdfRendererParams = quickFBO->sdfRenderer_Params();
    }

    void render() override {
        QOpenGLFunctions* functions = QOpenGLContext::currentContext()->functions();
        functions->glClearColor(1.f, 0.f, 0.f, 1.f);
        functions->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        _sdfRenderer.render(_cameraInfos, _sdfRendererParams);
        update();
        _window->resetOpenGLState();
    }

    QOpenGLFramebufferObject* createFramebufferObject(const QSize& size) override {
        QOpenGLFramebufferObjectFormat format;
        format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
        return new QOpenGLFramebufferObject(size, format);
    }

private:
    SdfRenderer _sdfRenderer;
    CameraInfos _cameraInfos;
    SdfRenderer_Params _sdfRendererParams;
    QQuickWindow* _window;
};