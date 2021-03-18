#include "SdfRenderer2D.hpp"

#include "Utility/File.hpp"

SdfRenderer2D::SdfRenderer2D()
{
    _shader.compile(MyFile::LoadToString("shaders/rendering/2D.fs.glsl"));
}

void
SdfRenderer2D::render(int gridSize)
{
    _shader.get().bind();
    _shader.get().setUniformValue("u_AspectRatio",
                                  1.0f); // TODO set me properly
    _shader.get().setUniformValue("u_GridSize",
                                  gridSize);
    _shader.draw();
}