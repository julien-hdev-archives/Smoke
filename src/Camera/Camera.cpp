#include "Camera.hpp"
#include "CameraState_Idle.hpp"
#include "Utility/Math.hpp"

#include <cmath>

Camera::Camera()
{
    setState<CameraState_Idle>();
    onTransformChanged();
}

void
Camera::onTransformChanged()
{
    QVector3D posRelToLookAt
        = QVector3D(std::cos(m_angleUp) * std::cos(m_angleGround), std::sin(m_angleUp),
                    std::cos(m_angleUp) * std::sin(m_angleGround))
          * m_distToLookAt;
    m_viewMatrix.setToIdentity();
    m_viewMatrix.lookAt(posRelToLookAt + m_lookAt, m_lookAt,
                        QVector3D(0.0f, Math::sign(std::cos(m_angleUp)), 0.0f));
    m_transformMatrix = m_viewMatrix.inverted();
}

QVector3D
Camera::xAxis() const
{
    return m_transformMatrix.column(0).toVector3D().normalized();
}
QVector3D
Camera::yAxis() const
{
    return m_transformMatrix.column(1).toVector3D().normalized();
}
QVector3D
Camera::zAxis() const
{
    return m_transformMatrix.column(2).toVector3D().normalized();
}
QVector3D
Camera::position() const
{
    return m_transformMatrix.column(3).toVector3D();
}

CameraInfos
Camera::getInfos() const
{
    return { xAxis(), yAxis(), zAxis(), position(), focalLength() };
}
