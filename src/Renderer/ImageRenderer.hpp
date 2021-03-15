#pragma once

#include "FullscreenShader.hpp"
#include <QOpenGLTexture>

class ImageRenderer
{
  public:
    ImageRenderer();
    ~ImageRenderer() = default;

    void render(int width, int height, const unsigned char *data);

  private:
    FullscreenShader _shader;
    QOpenGLTexture _texture;
};