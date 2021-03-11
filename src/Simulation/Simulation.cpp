#include "Simulation.hpp"

Simulation::Simulation()
{
    _computeShader.initialize("shaders/simulation/2D.cs.glsl");
    _ssbo.initialize(0);
    std::vector<float> input(_gridSize * _gridSize);
    for (int x = 0; x < _gridSize; x++)
    {
        for (int y = 0; y < _gridSize; y++)
        {
            input[x + y * _gridSize] = (x + y) / 2.f / _gridSize;
        }
    }

    _ssbo.uploadData(input.data(), input.size() * sizeof(float));
}

void
Simulation::update()
{
    _computeShader->bind();
    _computeShader.compute(_gridSize * _gridSize);
}