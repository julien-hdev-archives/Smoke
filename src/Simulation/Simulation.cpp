#include "Simulation.hpp"

Simulation::Simulation()
{
    _computeShader.initialize("shaders/simulation/compute_shader.cs.glsl");
    _ssboIN.initialize(0);
    _ssboOUT.initialize(1);
    const int N = 23;
    float input[N];
    for (int i = 0; i < N; i++)
    {
        input[i] = i;
    }

    _ssboIN.uploadData(input, sizeof(input));
    _ssboOUT.uploadData(nullptr, sizeof(input));
}

void
Simulation::update()
{
    const int N = 23;
    _computeShader->bind();
    _computeShader->setUniformValue("coeff", 0.5f); // set uniforms
    _computeShader.compute(N);
}