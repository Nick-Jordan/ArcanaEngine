#version 400

layout(location = 0) in vec3 vs_Position;
layout(location = 1) in vec3 vs_Normal;

out vec3 fs_Position;
out vec3 fs_Normal;

uniform mat4 u_ProjectionMatrix;
uniform mat4 u_ViewMatrix;

void main()
{
	fs_Position = vs_Position;
	fs_Normal = vs_Normal;

	gl_Position = u_ProjectionMatrix * u_ViewMatrix * vec4(vs_Position, 1.0);
}