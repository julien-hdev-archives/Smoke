#version 430

uniform int NumberOfComputations; 

layout(std430, binding = 0) buffer BufferIN 
{
    float bufferIN[];
};
layout(std430, binding = 1) buffer BufferOUT
{
    float bufferOUT[];
};

layout(local_size_x = 256, local_size_y = 1, local_size_z = 1) in;
void main()
{
    uint id = gl_GlobalInvocationID.x;
    if (id < NumberOfComputations) {
        bufferIN[id] = fract(bufferIN[id] + 0.005);
    }
}
