#version 400

layout(location = 0) in vec4 vs_Position;

void main()
{
	gl_Position = vec4(vs_Position.xyz, 1.0);
}