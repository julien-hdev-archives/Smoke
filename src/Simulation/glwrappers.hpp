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
public:

protected:
    void setup_surface(GlContext *surface);
    GlContext *surface_ = nullptr;

    void gl_assert(QString message); //Check openGL error
    void xassert(bool condition, QString message); //Check Qt wrapper error
    void activate_context();

    QOpenGLFunctions_4_3_Core *gl();
};

//Class for warping GPU buffer, will it with values from CPU and load to CPU after computations
class ShaderBuffer: public GlCommon {
public:
    void setup(GlContext *surface);
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
    void setup(QString shader_file, GlContext *surface);

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

//---------------------------------------------------------------------
//GlContext
//Surface for maintaining OpenGL context
//Compute shaders and buffers will use it for enabling OpenGL context at operations
//It's subclass of QOffscreenSurface, it's required to have such thing by Qt work with OpenGL
//(Also can use QWindow, but in our case it's not needed, we want non-graphical computations)
//Note: QOffscreenSurface can work in non-main thread,
//but its "create" must be called from main thread
//---------------------------------------------------------------------
class GlContext: public QOffscreenSurface, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit GlContext();
    ~GlContext();

    //Initialize OpenGL context and load shader, must be called before computing
    void setup();

    //Switch to OpenGL context - required before most operations
    void activate_context();

    void gl_assert(QString message); //Check openGL error
    void xassert(bool condition, QString message); //Check Qt wrapper error

    QOpenGLFunctions_4_3_Core *gl() { return gl43; }
private:
    //OpenGL context
    QOpenGLContext *m_context = nullptr;        //will be deleted automatically
    //OpenGL extra functions
    QOpenGLFunctions_4_3_Core *gl43 = nullptr;  //should't delete this!
};

#endif // COMPUTESURFACE_H
