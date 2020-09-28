#include "Triangle.hpp"

#include <QtGui/QOpenGLFunctions>

Triangle::~Triangle() {
    invalidate();
}

void Triangle::initialize() {
    if(_vao.isCreated())
        return;
    
    if(!_vao.create())
        qFatal("Unable to create VAO.");

    _vao.bind();

    QVector<QVector2D> vertices;
    vertices.push_back(QVector2D(-0.5f, -0.5f));
    vertices.push_back(QVector2D(0.5f, -0.5f));
    vertices.push_back(QVector2D(-0.f, 0.5f));

    if(!_positionsBuffer.create())
        qFatal("Unable to create positions buffer.");
    _positionsBuffer.bind();
    _positionsBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    _positionsBuffer.allocate(vertices.constData(), vertices.size() * sizeof(QVector2D));


    QVector<QVector3D> colors;
    colors.push_back(QVector3D(1.f, 0.f, 0.f));
    colors.push_back(QVector3D(0.f, 1.f, 0.f));
    colors.push_back(QVector3D(0.f, 0.f, 1.f));

    if(!_colorsBuffer.create())
        qFatal("Unable to create colors buffer.");
    _colorsBuffer.bind();
    _colorsBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    _colorsBuffer.allocate(colors.constData(), colors.size() * sizeof(QVector3D));


    if(!_program.create())
        qFatal("Unable to create shader program.");
    if(!_program.addShaderFromSourceFile(QOpenGLShader::Vertex, "shaders/triangle.vs.glsl"))
        qFatal("Vertex shader compilation failed.");
    if(!_program.addShaderFromSourceFile(QOpenGLShader::Fragment, "shaders/triangle.fs.glsl"))
        qFatal("Fragment shader compilation failed.");
    if(!_program.link())
        qFatal("Shader program not linked.");


    _program.bind();

    const int VERTEX_ATTR_POS = 0;
    _positionsBuffer.bind();
    _program.enableAttributeArray(VERTEX_ATTR_POS);
    _program.setAttributeArray(VERTEX_ATTR_POS, GL_FLOAT, 0, 2);

    const int VERTEX_ATTR_COL = 1;
    _colorsBuffer.bind();
    _program.enableAttributeArray(VERTEX_ATTR_COL);
    _program.setAttributeArray(VERTEX_ATTR_COL, GL_FLOAT, 0, 3);

    _vao.release();
}

void Triangle::render() {
    QOpenGLFunctions* functions = QOpenGLContext::currentContext()->functions();

    functions->glClearColor(0, 0, 0, 1.0);
    functions->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    _program.bind();

    _vao.bind();
    functions->glDrawArrays(GL_TRIANGLES, 0, 3);
    _vao.release();
}

void Triangle::invalidate() {
    _positionsBuffer.destroy();
    _colorsBuffer.destroy();
    _vao.destroy();
}