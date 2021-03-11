#pragma once

#include <QtGui/QOpenGLShaderProgram>
#include <QOpenGLBuffer>

class ShaderBuffer  {
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

    void bind();
    void unbind();
};

//---------------------------------------------------------------------
//ComputeShader
//Class for maintaining compute shaders
//---------------------------------------------------------------------
class ComputeShader {
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