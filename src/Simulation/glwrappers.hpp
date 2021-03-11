#ifndef COMPUTESURFACE_H
#define COMPUTESURFACE_H

#include <QOffscreenSurface>
#include <QtGui/QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLFunctions_4_3_Core>

QT_BEGIN_NAMESPACE
class QOpenGLContext;
QT_END_NAMESPACE


class GlContext;

//---------------------------------------------------------------------
//GlCommon
//Class storing GL context and QSurface and manages errors handling.
//It's inherited by ShaderBuffer and ComputeShader
//---------------------------------------------------------------------
class GlCommon {
protected:
    QOpenGLFunctions_4_3_Core *gl();
};

//Class for warping GPU buffer, will it with values from CPU and load to CPU after computations
class ShaderBuffer: public GlCommon {
public:
    void setup();
    void allocate(void *data, int size_bytes);
    void read_to_cpu(void *data, int size_bytes);
    void clear();

    //Bind buffer to shader by specifying its binding index:
    //Shader:
    //    layout(std430, binding = 0) buffer Buf
    //    { float buf[]; };
    void bind_for_shader(int binding_index);

protected:
    QOpenGLBuffer shader_buffer_;

    //We must always bind/unbind buffer for the most operations - it's not made by Qt!
    //See the details at Qt code:
    //https://code.woboq.org/qt5/qtbase/src/gui/opengl/qopenglbuffer.cpp.html
    //Khronos about shader buffers:
    //https://www.khronos.org/opengl/wiki/Shader_Storage_Buffer_Object

    void bind();
    void unbind();
};

//---------------------------------------------------------------------
//ComputeShader
//Class for maintaining compute shaders
//---------------------------------------------------------------------
class ComputeShader: public GlCommon {
public:
    void setup(QString shader_file);

    //Call this to set up uniforms
    void begin();

    //Access this to set up uniforms
    QOpenGLShaderProgram &program();

    //Perform computing with compute shader.
    //Note: it calls glFinish(), so waits until computing will be finished
    void compute(int NX, int NY = 1, int NZ = 1);   //NX,NY,NZ - number of groups in X,Y,Z dimensions

    //Call after computing end
    void end();

protected:
    QOpenGLShaderProgram program_;
};

#endif // COMPUTESURFACE_H
