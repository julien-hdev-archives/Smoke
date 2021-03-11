#pragma once

#include "ComputeShader.hpp"

class Simulation
{
  public:
    Simulation();
    ~Simulation() = default;

    void compute();

  private:
    ComputeShader _computeShader;
};