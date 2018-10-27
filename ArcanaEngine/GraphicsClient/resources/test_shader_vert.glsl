#version 400

layout(location = 0) in vec3 vs_Position;
layout(location = 1) in vec4 vs_Color;

out vec4 fs_Color;

uniform mat4 u_ProjectionMatrix;
uniform mat4 u_ViewMatrix;

void main()
{
	fs_Color = vs_Color;

	gl_Position = u_ProjectionMatrix * u_ViewMatrix * vec4(vs_Position, 1.0);
}