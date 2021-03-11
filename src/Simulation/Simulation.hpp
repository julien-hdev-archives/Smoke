#pragma once

#include "glwrappers.hpp"

class Simulation
{
  public:
    Simulation();
    ~Simulation() = default;

    void compute();

  private:
    ComputeShader _computeShader;
};