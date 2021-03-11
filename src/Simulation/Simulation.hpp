#pragma once

#include "glwrappers.hpp"

class Simulation
{
  public:
    Simulation();
    ~Simulation() = default;

  private:
    GlContext _context;
    ComputeShader _computeShader;
};