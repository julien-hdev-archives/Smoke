#pragma once

#include "Camera/CameraInfos.hpp"
#include "SdfRenderer.hpp"
#include "SdfRendererProperties.hpp"
#include "Simulation/Simulation.hpp"

#include <QtQuick/QQuickFramebufferObject>

class QOpenGLFramebufferObject;

class MyQuickFBORenderer : public QQuickFramebufferObject::Renderer
{

  public:
    MyQuickFBORenderer(QQuickWindow &window);
    ~MyQuickFBORenderer() = default;

    void synchronize(QQuickFramebufferObject *item) override;
    void render() override;
    QOpenGLFramebufferObject *
    createFramebufferObject(const QSize &size) override;

  private:
    SdfRenderer _sdfRenderer;
    SdfRenderer_Params _sdfRendererParams;
    Simulation _simulation;
    CameraInfos _cameraInfos;
    QQuickWindow &_window;
};