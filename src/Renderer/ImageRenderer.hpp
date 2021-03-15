#pragma once

#include "FullscreenShader.hpp"

class ImageRenderer
{
  public:
    ImageRenderer();
    ~ImageRenderer() = default;

    void render();

  private:
    FullscreenShader _shader;
};