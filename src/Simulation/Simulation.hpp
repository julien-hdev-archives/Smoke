#pragma once

#include "ComputeShader.hpp"

class Simulation
{
  public:
    Simulation();
    ~Simulation() = default;

    void update();
    inline int gridSize() const { return _gridSize; }

  private:
    ComputeShader _computeShader;
    SSBO _ssbo;
    int _gridSize = 101;
};