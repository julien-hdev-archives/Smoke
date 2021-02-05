#pragma once

#include "CameraState.hpp"
#include <QMatrix4x4>
#include <QVector3D>
#include "CameraInfos.hpp"

class Camera {
friend class CameraState_Idle;
friend class CameraState_Rotation;
friend class CameraState_Translation; 
public:
	Camera();
	~Camera() = default;

	inline const QMatrix4x4& transformMatrix() const { return m_transformMatrix; }
	inline const QMatrix4x4& viewMatrix() const { return m_viewMatrix; }
	QVector3D xAxis() const;
	QVector3D yAxis() const;
	QVector3D zAxis() const;
	QVector3D position() const;
	inline float focalLength() const { return m_focalLength; }
	CameraInfos getInfos() const;

	inline void update() { m_state->update(); }
	inline void onWheelDown() { m_state->onWheelDown(); }
	inline void onWheelUp()   { m_state->onWheelUp();   }
	inline void onWheelScroll(float dl) { m_state->onWheelScroll(dl); }

private:
	void onTransformChanged();

	template <typename T, typename ...Args>
	inline void setState(Args&& ...args) {
		m_state = std::make_unique<T>(*this, std::forward<Args>(args)...);
	}

private:
	QVector3D m_lookAt = QVector3D(0.f, 0.f, 0.f);
	float m_distToLookAt = 48.0f;
	float m_angleGround = 0.0f;
	float m_angleUp = 0.29f;

	QMatrix4x4 m_transformMatrix;
	QMatrix4x4 m_viewMatrix;

	float m_focalLength = 1.0f;

	std::unique_ptr<CameraState> m_state;
};