#include "glwrappers.hpp"

#include <QtGui/QOpenGLContext>
#include <QOpenGLExtraFunctions>
#include <QDebug>

//---------------------------------------------------------------------
//GlCommon
//---------------------------------------------------------------------
void GlCommon::setup_surface(GlContext *surface) {
    surface_ = surface;
    activate_context();
}

//---------------------------------------------------------------------
void GlCommon::gl_assert(QString message) { //Check openGL error
    if (surface_) surface_->gl_assert(message);
}

//---------------------------------------------------------------------
void GlCommon::xassert(bool condition, QString message) { //Check Qt wrapper error
    if (surface_) surface_->xassert(condition, message);
}

//---------------------------------------------------------------------
void GlCommon::activate_context() {
    if (surface_) surface_->activate_context();
}

//---------------------------------------------------------------------
QOpenGLFunctions_4_3_Core *GlCommon::gl() {
    return surface_->gl();
}

//---------------------------------------------------------------------
//ShaderBuffer
//---------------------------------------------------------------------
void ShaderBuffer::setup(GlContext *surface) {
    setup_surface(surface);

    activate_context();
    xassert(shader_buffer_.create(), "Error at shader_buffer_.create()");

    shader_buffer_.setUsagePattern(QOpenGLBuffer::DynamicCopy);
}

//---------------------------------------------------------------------
void ShaderBuffer::bind() {
    xassert(shader_buffer_.bind(), "Error at shader_buffer_.bind()");
}

//---------------------------------------------------------------------
void ShaderBuffer::unbind() {
    shader_buffer_.release();
}

//---------------------------------------------------------------------
void ShaderBuffer::allocate(void *data, int size_bytes) {
    activate_context();
    //we must always bind/unbind buffer for the most operations - it's not made by Qt!
    bind();
    shader_buffer_.allocate(data, size_bytes);
    unbind();
}

//---------------------------------------------------------------------
void ShaderBuffer::clear() {
    activate_context();
    shader_buffer_.destroy();
}

//---------------------------------------------------------------------
void ShaderBuffer::read_to_cpu(void *data, int size_bytes) {
    activate_context();
        
    bind();
    xassert(shader_buffer_.read(0, data, size_bytes), "ShaderBuffer::read_to_cpu error");
    unbind();
}

//---------------------------------------------------------------------
//Bind buffer to shader by specifying its binding index:
//Shader:
//    layout(std430, binding = 0) buffer Buf
//    { float buf[]; };
void ShaderBuffer::bind_for_shader(int binding_index) {
    activate_context();
    surface_->gl()->glBindBufferBase(GL_SHADER_STORAGE_BUFFER, binding_index, shader_buffer_.bufferId());
    gl_assert("Error at glBindBufferBase");
}

//---------------------------------------------------------------------
//ComputeShader
//---------------------------------------------------------------------
//Initialize OpenGL context and load shader, must be called before computing
void ComputeShader::setup(QString shader_file, GlContext *surface) {
    setup_surface(surface);

    activate_context();
    //Load compute shader
    xassert(program_.addShaderFromSourceFile(QOpenGLShader::Compute, shader_file),
            "Can't load shader file " + shader_file);

    //compute_shader->addShaderFromSourceCode(QOpenGLShader::Compute, text);
    xassert(program_.link(), "Can't link compute shader");
}

//---------------------------------------------------------------------
//Call this to set up uniforms
void ComputeShader::begin() {
    activate_context();
    xassert(program_.bind(), "Can't bind compute shader");
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
    gl()->glDispatchCompute(NX, NY, NZ);		//Run computation
    gl_assert("glDispatchCompute error");
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

//---------------------------------------------------------------------
//GlContext
//---------------------------------------------------------------------
//Note: QOffscreenSurface can work in non-main thread,
//but its "create" must be called from main thread

GlContext::GlContext()
    : QOffscreenSurface()
    , m_context(0)
{

}

//---------------------------------------------------------------------
GlContext::~GlContext()
{

}

//---------------------------------------------------------------------
//Check openGL error
void GlContext::gl_assert(QString message) {
    GLenum error = GL_NO_ERROR;
    do {
        error = gl43->glGetError();
        if (error != GL_NO_ERROR) {
            qDebug() << message << ", OpenGL error code " << error;
        }
    } while (error != GL_NO_ERROR);

}

//---------------------------------------------------------------------
//Check Qt wrapper error
void GlContext::xassert(bool condition, QString message) {
    if (!condition) {
        qDebug() << message;
    }
}

//---------------------------------------------------------------------
//Initialize OpenGL context and load shader, must be called before computing
void GlContext::setup() {
    //Initialize OpenGL context
    if (!m_context) {
        QSurfaceFormat format;
        //Request OpenGL 4.3
        format.setMajorVersion(4);
        format.setMinorVersion(3);

        //format.setSamples(16); //antialiasing samples
        //format.setProfile(QSurfaceFormat::CoreProfile);
        //format.setOption(QSurfaceFormat::DebugContext);
        setFormat(format);

        //Create Qt surface
        create();

        //Create OpenGL context
        m_context = new QOpenGLContext(this);
        m_context->setFormat(format); //requestedFormat());  //it's our "format"
        m_context->create();

        //Switch to OpenGL context
        activate_context();

        //Initialize OpenGL functions
        initializeOpenGLFunctions();

        //Get pointer to extra functions of 4.3
        gl43 = m_context->versionFunctions<QOpenGLFunctions_4_3_Core>();
        gl_assert("Error creating gl43");
    }
}

//---------------------------------------------------------------------
//Switch to OpenGL context - required before most operations
void GlContext::activate_context() {
    xassert(m_context, "OpenGL context is not inited");
    if (!m_context) return;
    m_context->makeCurrent(this);
}

//---------------------------------------------------------------------