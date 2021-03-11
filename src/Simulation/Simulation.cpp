#include "Simulation.hpp"

Simulation::Simulation()
{
    _computeShader.initialize("shaders/simulation/compute_shader.cs.glsl");
    compute();
}

void
Simulation::compute()
{
    const int N = 23;
    float input[N];
    float output[N];
    for (int i = 0; i < N; i++)
    {
        input[i] = i;
        output[i] = -i;
    }

    SSBO input_buffer, output_buffer;

    input_buffer.initialize(0);
    output_buffer.initialize(1);

    input_buffer.uploadData(input, sizeof(input));
    output_buffer.uploadData(output, sizeof(output));

    // Compute
    _computeShader->bind();
    _computeShader->setUniformValue("coeff", 0.5f); // set uniforms
    _computeShader.compute(N);

    // Download input and resulted buffers to CPU
    // Clear to be sure we really read them
    for (int i = 0; i < N; i++)
    {
        input[i] = 0;
        output[i] = 0;
    }

    input_buffer.downloadData(input, sizeof(input));
    output_buffer.downloadData(output, sizeof(output));

    qDebug() << "Buffers: ";
    for (int i = 0; i < N; i++)
    {
        qDebug() << "  " << input[i] << " " << output[i];
    }
}