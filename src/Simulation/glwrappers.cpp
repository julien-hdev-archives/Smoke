#include "glwrappers.hpp"

#include <QtGui/QOpenGLContext>
#include <QOpenGLExtraFunctions>
#include <QDebug>


//---------------------------------------------------------------------
QOpenGLFunctions_4_3_Core *GlCommon::gl() {
    return QOpenGLContext::currentContext()
                           ->versionFunctions<QOpenGLFunctions_4_3_Core>();
    ;
}

//---------------------------------------------------------------------
//ShaderBuffer
//---------------------------------------------------------------------
void ShaderBuffer::setup() {
    shader_buffer_.create();

    shader_buffer_.setUsagePattern(QOpenGLBuffer::DynamicCopy);
}

//---------------------------------------------------------------------
void ShaderBuffer::bind() {
    shader_buffer_.bind();
}

//---------------------------------------------------------------------
void ShaderBuffer::unbind() {
    shader_buffer_.release();
}

//---------------------------------------------------------------------
void ShaderBuffer::allocate(void *data, int size_bytes) {
    bind();
    shader_buffer_.allocate(data, size_bytes);
    unbind();
}

//---------------------------------------------------------------------
void ShaderBuffer::clear() {
    shader_buffer_.destroy();
}

//---------------------------------------------------------------------
void ShaderBuffer::read_to_cpu(void *data, int size_bytes) {
    bind();
    shader_buffer_.read(0, data, size_bytes);
    unbind();
}

//---------------------------------------------------------------------
//Bind buffer to shader by specifying its binding index:
//Shader:
//    layout(std430, binding = 0) buffer Buf
//    { float buf[]; };
void ShaderBuffer::bind_for_shader(int binding_index) {
    gl()->glBindBufferBase(GL_SHADER_STORAGE_BUFFER, binding_index, shader_buffer_.bufferId());
}

//---------------------------------------------------------------------
//ComputeShader
//---------------------------------------------------------------------
//Initialize OpenGL context and load shader, must be called before computing
void ComputeShader::setup(QString shader_file) {
    //setup_surface(surface);

    //activate_context();
    //Load compute shader
    program_.addShaderFromSourceFile(QOpenGLShader::Compute, shader_file);

    //compute_shader->addShaderFromSourceCode(QOpenGLShader::Compute, text);
    program_.link();
}

//---------------------------------------------------------------------
//Call this to set up uniforms
void ComputeShader::begin() {
    program_.bind();
}

//---------------------------------------------------------------------
//Access this to set up uniforms
QOpenGLShaderProgram &ComputeShader::program() {
    return program_;
}

//---------------------------------------------------------------------
//Perform computing with compute shader.
//Note: it calls glFinish(), so waits until computing will be finished
void ComputeShader::compute(int NX, int NY, int NZ) {
    gl()->glDispatchCompute(NX, NY, NZ);
    //gl()->glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
    //gl_assert("glMemoryBarrier error");

    //Wait to complete the computing
    gl()->glFinish();
}

//---------------------------------------------------------------------
//Call after computing end
void ComputeShader::end() {
    program_.release();
}
