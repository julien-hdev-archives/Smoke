#version 430

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

//Example of using structure:
//struct ParticleData {
//    vec4 pos, dir, color;
//};
//layout (std430, binding = 0) coherent buffer Values
//{
//    ParticleData particles[];
//};

layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;

void main(void)
{
    uint id = gl_GlobalInvocationID.x;
    
    float value = input_buf[id];  //read input
    
    value += id*coeff;        //change 
    
    output_buf[id] = value;        //write output  

    /*vec3 p = pos[id].xyz;
    p.x += 1.f;
    p.y += 0.f;
    p.z += -1.f;
    pos[id].xyz = p;*/
}
