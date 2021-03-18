#pragma once

#include "Camera/Camera.hpp"
#include "SdfRendererProperties.hpp"
#include "SimulatorProperties.hpp"
#include "UI/Attribute/FloatAttribute.hpp"
#include <QtQuick/QQuickFramebufferObject>

class MyQuickFBO : public QQuickFramebufferObject
{
    Q_OBJECT
    Q_PROPERTY(SdfRendererProperties *sdfRendererProperties MEMBER
                   _sdfRendererProperties NOTIFY sdfRendererPropertiesChanged)
    Q_PROPERTY(SimulatorProperties *simulatorProperties MEMBER
                   _simulatorProperties NOTIFY simulatorPropertiesChanged)
  public:
    explicit MyQuickFBO(QQuickItem *parent = nullptr);
    QQuickFramebufferObject::Renderer *createRenderer() const override;

    Q_INVOKABLE void onMousePress();
    Q_INVOKABLE void onMouseRelease();
    Q_INVOKABLE void onWheelScroll(float delta);

    inline Camera &
    camera()
    {
        return _camera;
    }

    inline const SdfRenderer_Params
    sdfRenderer_Params() const
    {
        return _sdfRendererProperties->sdfRenderer_Params();
    }

    inline const Simulator_Params
    simulator_Params() const
    {
        return _simulatorProperties->simulator_Params();
    }

    void
    setHaveToReset(int toReset) const
    {
        _simulatorProperties->findAttribute("haveToReset")->setValue(toReset);
    }

  public:
    Q_SIGNAL void sdfRendererPropertiesChanged();
    Q_SIGNAL void simulatorPropertiesChanged();

  private:
    Camera _camera;

    SdfRendererProperties *_sdfRendererProperties;
    SimulatorProperties *_simulatorProperties;
};
