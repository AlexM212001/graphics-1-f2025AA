#version 430
layout (location = 0) in vec2 vPos;
layout (location = 1) in vec3 vCol;

out vec3 color;

uniform mat4 u_world;

void main()
{
    // "m rows by n columns" in order for matrix multiplication to work
    vec4 pos = vec4(vPos, 0.0, 1.0);

    // Apply model-to-world transformation to vertices
    pos = u_world * pos;

    // Output the input color (gets interpolated during rasterization which happens in-between vertex & fragment shaders)
    color = vCol;
    
    // Write to gl_Position to output final vertex position in clip-space (-1 to +1)
    gl_Position = pos;
}
