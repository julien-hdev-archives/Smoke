#pragma once

#include "CameraState.hpp"

#include <qcursor.h>
#include <qvector3d.h>

class CameraState_Translation : public CameraState {
public:
	CameraState_Translation(Camera& camera);
	~CameraState_Translation() = default;

	void update() override;
	void onWheelUp() override;

private:
	QVector3D m_initialLookAt;
	QPoint m_initialMousePos;
};