#pragma once

#include "CameraState.hpp"

#include <qcursor.h>

class CameraState_Rotation : public CameraState
{
  public:
    CameraState_Rotation(Camera &camera);
    ~CameraState_Rotation() = default;

    void update() override;
    void onWheelUp() override;

  private:
    float m_initialAngleGround;
    float m_initialAngleUp;
    QPoint m_initialMousePos;
};