#include "ImageRenderer.hpp"

#include "Utility/File.hpp"

ImageRenderer::ImageRenderer() : _texture(QOpenGLTexture::Target::Target2D)
{
    _shader.compile(MyFile::LoadToString("shaders/rendering/texture.fs.glsl"));
}

void
ImageRenderer::render(int width, int height, const unsigned char *data)
{
    QImage image(data, width, height, QImage::Format_RGBA8888);
    _texture.destroy();
    _texture.create();
    _texture.setMagnificationFilter(QOpenGLTexture::Filter::Nearest);
    _texture.setMinificationFilter(QOpenGLTexture::Filter::Nearest);
    _texture.setData(image);
    _shader.get().bind();
    _texture.bind(0);
    _shader.get().setUniformValue("u_Texture", 0);
    _shader.draw();
}