#version 400

layout(location = 0) in vec3 vs_Position;
layout(location = 1) in vec3 vs_Normal;
layout(location = 2) in vec2 vs_TexCoord0;

out vec3 fs_Position;
out vec3 fs_Normal;
out vec2 fs_TexCoord0;

uniform mat4 u_ProjectionMatrix;
uniform mat4 u_ViewMatrix;

void main()
{
	fs_Position = vs_Position;
	fs_Normal = vs_Normal;
	fs_TexCoord0 = vs_TexCoord0;

	gl_Position = u_ProjectionMatrix * u_ViewMatrix * vec4(vs_Position, 1.0);
}