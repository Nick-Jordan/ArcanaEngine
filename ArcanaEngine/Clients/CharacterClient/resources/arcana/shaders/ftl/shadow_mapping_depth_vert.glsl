#version 400

layout (location = 0) in vec3 vs_Position;

uniform mat4 u_LightSpaceMatrix;
uniform mat4 u_ModelMatrix;

void main()
{
    gl_Position = u_LightSpaceMatrix * u_ModelMatrix * vec4(vs_Position, 1.0);
}