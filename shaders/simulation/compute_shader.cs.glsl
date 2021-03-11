#version 430

uniform int NumberOfComputations;
//Example of uniform parameter 
uniform float coeff=1;    

//Buffers for processing
layout(std430, binding = 0) buffer Input_Buf 
{
    float input_buf[];
};

layout(std430, binding = 1) buffer Output_Buf
{
    float output_buf[];
};

layout(local_size_x = 256, local_size_y = 1, local_size_z = 1) in;
void main(void)
{
    uint id = gl_GlobalInvocationID.x;
    if (id < NumberOfComputations) {
        output_buf[id] = input_buf[id] + id*coeff;
    }
}
