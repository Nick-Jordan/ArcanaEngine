#version 400

layout(location = 0) in vec3 vs_Position;
layout(location = 1) in vec2 vs_TexCoords;
layout(location = 2) in vec4 vs_Color;

uniform mat4 u_OrthographicProjection;

out vec4 fs_Color;
out vec2 fs_TexCoords;

void main()
{
	fs_Color = vs_Color;
	fs_TexCoords = vs_TexCoords;

	gl_Position = u_OrthographicProjection * vec4(vs_Position, 1.0);
}