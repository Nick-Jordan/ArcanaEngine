#version 400

layout(location = 0) in vec2 vs_Position;
layout(location = 1) in vec2 vs_TexCoord;

out vec3 fs_Position;

void main()
{
	fs_Position = vec3(vs_TexCoord.xy, 0.0);

	gl_Position = vec4(vec3(vs_Position.xy, 0.0), 1.0);
}