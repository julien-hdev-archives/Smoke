#include <QtGui/qvector3d.h>

#include <QVector>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QOpenGLBuffer>
#include <QtGui/QOpenGLVertexArrayObject>

class Triangle {
    public:
        Triangle() = default;
        ~Triangle();

        void render();
        void initialize();
        void invalidate();

    private:
        QOpenGLBuffer _positionsBuffer = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
        QOpenGLBuffer _colorsBuffer = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
        QOpenGLVertexArrayObject _vao;
        QOpenGLShaderProgram _program;
};