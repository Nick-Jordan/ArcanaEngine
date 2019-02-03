#version 400

layout (triangles) in;
layout (triangle_strip, max_vertices=18) out;

uniform mat4 u_ShadowMatrices[6];

out vec4 fs_Position; // FragPos from GS (output per emitvertex)

void main()
{
    for(int face = 0; face < 6; ++face)
    {
        gl_Layer = face; // built-in variable that specifies to which face we render.
        for(int i = 0; i < 3; ++i) // for each triangle's vertices
        {
            fs_Position = gl_in[i].gl_Position;
            gl_Position = u_ShadowMatrices[face] * fs_Position;
            EmitVertex();
        }    
        EndPrimitive();
    }
} 