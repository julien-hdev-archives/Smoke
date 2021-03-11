#pragma once

#include <QOpenGLBuffer>
#include <QtGui/QOpenGLShaderProgram>

class SSBO  {
public:
    void initialize(int binding_index);
    void uploadData(void *data, int size_bytes);
    void downloadData(void *data, int size_bytes);

private:
    QOpenGLBuffer _qtBuffer;
};

class ComputeShader {
public:
    void initialize(QString shader_file);
    inline QOpenGLShaderProgram *operator->() { return &_qtProgram; }
    void compute(int NX = 256, int NY = 1, int NZ = 1);

protected:
    QOpenGLShaderProgram _qtProgram;
};