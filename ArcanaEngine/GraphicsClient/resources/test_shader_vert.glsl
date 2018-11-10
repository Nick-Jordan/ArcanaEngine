#version 400

layout(location = 0) in vec3 vs_Position;
layout(location = 1) in vec4 vs_Color;

out vec4 fs_Color;
out vec3 fs_Position;
out vec3 fs_Normal;

uniform mat4 u_ProjectionMatrix;
uniform mat4 u_ViewMatrix;

void main()
{
	fs_Color = vs_Color;
	fs_Position = vs_Position;
	fs_Normal = vec3(0, 0, 1);

	gl_Position = u_ProjectionMatrix * u_ViewMatrix * vec4(vs_Position, 1.0);
}