#pragma once

#include "FullscreenShader.hpp"
#include <string>

class SdfRenderer2D
{
  public:
    SdfRenderer2D();
    ~SdfRenderer2D() = default;

    void render();

  private:
    FullscreenShader _shader;
};