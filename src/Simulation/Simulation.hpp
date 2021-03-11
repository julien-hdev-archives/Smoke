#pragma once

#include "ComputeShader.hpp"

class Simulation
{
  public:
    Simulation();
    ~Simulation() = default;

    void update();

  private:
    ComputeShader _computeShader;
    SSBO _ssboIN;
    SSBO _ssboOUT;
};