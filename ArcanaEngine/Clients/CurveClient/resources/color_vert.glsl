#version 400

layout(location = 0) in vec2 vs_Position;
layout(location = 1) in vec3 vs_Color;

out vec3 fs_Color;

uniform mat4 u_ProjectionMatrix;

void main()
{
	gl_Position = u_ProjectionMatrix * vec4(vs_Position, 0.0, 1.0);
	
	fs_Color = vs_Color;
}