#pragma once

#include "Camera/Camera.hpp"
#include "SdfRenderer_Params.hpp"
#include <QtQuick/QQuickFramebufferObject>

class MyQuickFBO : public QQuickFramebufferObject
{
    Q_OBJECT
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

  private:
    Camera _camera;
    SdfRenderer_Params _sdfRendererParams;
};
