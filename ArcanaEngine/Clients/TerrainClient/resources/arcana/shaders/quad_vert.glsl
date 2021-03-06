#version 400

layout(location = 0) in vec2 vs_Position;
layout(location = 1) in vec2 vs_TexCoord;

out vec2 fs_TexCoord;

void main()
{
	fs_TexCoord = vs_TexCoord;
	gl_Position = vec4(vs_Position, 0.0, 1.0);
}