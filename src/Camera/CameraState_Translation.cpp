#include "CameraState_Translation.hpp"
#include "CameraState_Idle.hpp"
#include "Camera.hpp"

CameraState_Translation::CameraState_Translation(Camera& camera)
	: CameraState(camera),
	  m_initialLookAt(camera.m_lookAt),
	  m_initialMousePos(QCursor::pos())
{
	qDebug("Translation");
}

void CameraState_Translation::update() {
	QPoint delta = (m_initialMousePos - QCursor::pos()) * 0.0001f;
	m_camera.m_lookAt = m_initialLookAt + m_camera.xAxis() * delta.x() - m_camera.yAxis() * delta.y();
	m_camera.onTransformChanged();
}

void CameraState_Translation::onWheelUp() {
	m_camera.setState<CameraState_Idle>();
}