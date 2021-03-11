#include "Simulation.hpp"

Simulation::Simulation()
{
    _computeShader.setup("shaders/simulation/compute_shader.cs.glsl");
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

    ShaderBuffer input_buffer, output_buffer;

    input_buffer.setup();
    output_buffer.setup();

    input_buffer.allocate(input, sizeof(input));
    output_buffer.allocate(output, sizeof(output));

    // Bind buffers to shader
    input_buffer.bind_for_shader(0);
    output_buffer.bind_for_shader(1);

    // Compute
    _computeShader.begin();
    _computeShader.program().setUniformValue("coeff", 0.5f); // set uniforms
    _computeShader.compute(N);
    _computeShader.end();

    // Download input and resulted buffers to CPU
    // Clear to be sure we really read them
    for (int i = 0; i < N; i++)
    {
        input[i] = 0;
        output[i] = 0;
    }

    input_buffer.read_to_cpu(input, sizeof(input));
    output_buffer.read_to_cpu(output, sizeof(output));

    qDebug() << "Buffers: ";
    for (int i = 0; i < N; i++)
    {
        qDebug() << "  " << input[i] << " " << output[i];
    }
}