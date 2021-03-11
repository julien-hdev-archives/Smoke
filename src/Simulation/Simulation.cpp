#include "Simulation.hpp"

Simulation::Simulation()
{
    _computeShader.initialize("shaders/simulation/compute_shader.cs.glsl");
    _ssboIN.initialize(0);
    _ssboOUT.initialize(1);
    std::vector<float> input(_gridSize * _gridSize);
    for (int x = 0; x < _gridSize; x++)
    {
        for (int y = 0; y < _gridSize; y++)
        {
            input[x + y * _gridSize] = (x + y) / 2.f / _gridSize;
        }
    }

    _ssboIN.uploadData(input.data(), input.size() * sizeof(float));
    _ssboOUT.uploadData(nullptr,     input.size() * sizeof(float));
}

void
Simulation::update()
{
    _computeShader->bind();
    _computeShader->setUniformValue("coeff", 0.5f); // set uniforms
    _computeShader.compute(_gridSize * _gridSize);
}