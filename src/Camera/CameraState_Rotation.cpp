#include "CameraState_Rotation.hpp"
#include "CameraState_Idle.hpp"
#include "Camera.hpp"

CameraState_Rotation::CameraState_Rotation(Camera& camera)
	: CameraState(camera),
	  m_initialAngleGround(camera.m_angleGround),
	  m_initialAngleUp    (camera.m_angleUp),
	  m_initialMousePos(QCursor::pos())
{
	qDebug("Rotation");
}

void CameraState_Rotation::update() {
	QPoint delta = QCursor::pos() - m_initialMousePos;
	m_camera.m_angleGround = m_initialAngleGround + delta.x() * 0.01f;
	m_camera.m_angleUp     = m_initialAngleUp     + delta.y() * 0.01f;
	m_camera.onTransformChanged();
}

void CameraState_Rotation::onWheelUp() {
	m_camera.setState<CameraState_Idle>();
}