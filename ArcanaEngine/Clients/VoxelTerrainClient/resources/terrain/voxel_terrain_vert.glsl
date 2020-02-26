#version 400

layout(location = 0) in vec3 vs_Position;
layout(location = 1) in vec3 vs_Normal;

uniform mat4 u_ProjectionMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_ModelMatrix;

out vec3 fs_Normal;

void main()
{
	fs_Normal = normalize(vs_Normal);
	gl_Position = u_ProjectionMatrix * u_ViewMatrix * u_ModelMatrix * vec4(vs_Position, 1.0);
}