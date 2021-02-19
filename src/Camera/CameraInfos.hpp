#pragma once

#include <QVector3D>

struct CameraInfos
{
    QVector3D xAxis;
    QVector3D yAxis;
    QVector3D zAxis;
    QVector3D position;
    float focalLength;
};