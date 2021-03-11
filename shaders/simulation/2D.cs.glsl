#version 430

uniform int NumberOfComputations; 

layout(std430, binding = 0) buffer Buffer 
{
    float density[];
};

layout(local_size_x = 256, local_size_y = 1, local_size_z = 1) in;
void main()
{
    uint id = gl_GlobalInvocationID.x;
    if (id < NumberOfComputations) {
        density[id] = fract(density[id] + 0.005);
    }
}
