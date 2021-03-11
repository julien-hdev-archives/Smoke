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
    SSBO _ssboIN;
    SSBO _ssboOUT;
    int _gridSize = 100;
};