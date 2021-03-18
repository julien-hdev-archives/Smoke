#version 430

uniform int NumberOfComputations;
uniform int GridSize;

layout(std430, binding = 0) buffer Buffer 
{
    float density[];
};

ivec2 coords2D(uint idx) {
    return ivec2(
        idx % GridSize,
        idx / GridSize
    );
}

int indexOf(ivec2 p) {
    return clamp(p.x, 0, GridSize-1) + clamp(p.y, 0, GridSize-1) * GridSize;
}

float densityAtOffset(int x, int y) {
    ivec2 myCoords = coords2D(gl_GlobalInvocationID.x);
    return density[indexOf(myCoords + ivec2(x, y))];
}

layout(local_size_x = 256, local_size_y = 1, local_size_z = 1) in;
void main()
{
    uint id = gl_GlobalInvocationID.x;
    if (id < NumberOfComputations) {
        density[id] = fract(densityAtOffset(0, 0) + 0.005);
    }
}
