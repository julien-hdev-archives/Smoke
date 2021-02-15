#pragma once

#include "Camera/Camera.hpp"
#include "Core/FloatAttribute.hpp"
#include "SdfRenderer_Params.hpp"
#include <QtQuick/QQuickFramebufferObject>

class MyQuickFBO : public QQuickFramebufferObject
{
    Q_OBJECT
    Q_PROPERTY(SdfRendererProperties *sdfRendererProperties MEMBER
                   _sdfRendererProperties NOTIFY sdfRendererPropertiesChanged)
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

    inline const SdfRenderer_Params &
    sdfRenderer_Params() const
    {
        return _sdfRendererParams;
    }

  public:
    Q_SIGNAL void sdfRendererPropertiesChanged();

  private:
    Camera _camera;
    SdfRenderer_Params _sdfRendererParams;

    SdfRendererProperties *_sdfRendererProperties;
};
