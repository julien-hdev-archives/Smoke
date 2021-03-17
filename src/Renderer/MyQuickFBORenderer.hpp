#pragma once

#include "Camera/CameraInfos.hpp"
#include "SdfRenderer2D.hpp"
#include "SdfRenderer3D.hpp"
#include "ImageRenderer.hpp"
#include "SdfRendererProperties.hpp"
#include "Simulation/Simulation.hpp"

#include <QtQuick/QQuickFramebufferObject>

#include "hybrid_simulator.hpp"


const int GRID_SIZE = 64;
const int CLUST_SIZE = 8;
const int DENSITY_GRID_SIZE = 256;

using Simulator = Simulator2D<GRID_SIZE, CLUST_SIZE, DENSITY_GRID_SIZE>;

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

    void resetSimulation();

    Simulator simulator = Simulator(0.5f, 2.0f, 0.4f);

  private:
    ImageRenderer _imageRenderer;
    SdfRenderer2D _sdfRenderer2D;
    SdfRenderer _sdfRenderer;
    SdfRenderer_Params _sdfRendererParams;
    Simulation _simulation;
    CameraInfos _cameraInfos;
    QQuickWindow &_window;
};
