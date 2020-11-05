#include "CameraState_Idle.hpp"
#include "CameraState_Rotation.hpp"
#include "CameraState_Translation.hpp"
#include "Camera.hpp"

#include <qguiapplication.h>

CameraState_Idle::CameraState_Idle(Camera& camera)
	: CameraState(camera)
{
	qDebug("Idle");
}

void CameraState_Idle::onWheelDown() {
	if (QGuiApplication::keyboardModifiers().testFlag(Qt::ShiftModifier)){
		m_camera.setState<CameraState_Translation>();
	}
	else {
		m_camera.setState<CameraState_Rotation>();
	}
}

void CameraState_Idle::onWheelScroll(float dl){
	m_camera.m_distToLookAt = std::max(m_camera.m_distToLookAt - dl*3.f, 0.00001f);
	m_camera.onTransformChanged();
}