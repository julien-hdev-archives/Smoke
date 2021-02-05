#pragma once

#include <QtQuick/QQuickFramebufferObject>
#include "SdfRenderer.hpp"
#include "SdfRenderer_Params.hpp"
#include "Camera/CameraInfos.hpp"
class QOpenGLFramebufferObject;

class MyQuickFBORenderer : public QQuickFramebufferObject::Renderer {

public:
    MyQuickFBORenderer() = default;
    ~MyQuickFBORenderer() = default;

    void synchronize(QQuickFramebufferObject* item) override;
    void render() override;
    QOpenGLFramebufferObject* createFramebufferObject(const QSize& size) override;

private:
    SdfRenderer _sdfRenderer;
    SdfRenderer_Params _sdfRendererParams;
    CameraInfos _cameraInfos;
    QQuickWindow* _window;
};