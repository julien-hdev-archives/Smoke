#include "ImageRenderer.hpp"

#include "Utility/File.hpp"

ImageRenderer::ImageRenderer() {
    _shader.compile(MyFile::LoadToString("shaders/rendering/texture.fs.glsl"));
}

void
ImageRenderer::render() {
    _shader.get().bind();
    //_shader.get().setUniformValue("u_GridSize", gridSize);
    _shader.draw();
}