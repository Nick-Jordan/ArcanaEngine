#version 450 core

layout(location = 0) in vec3 vs_Position;
layout(location = 1) in vec3 vs_Normal;

uniform mat4 u_ModelMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_ProjectionMatrix;
uniform mat3 u_NormalMatrix;

out vec3 worldPositionGeom;
out vec3 normalGeom;

void main()
{
	worldPositionGeom = (u_ModelMatrix * vec4(vs_Position, 1)).xyz;
	normalGeom = normalize(u_NormalMatrix * vs_Normal);
	gl_Position = u_ProjectionMatrix * u_ViewMatrix * vec4(worldPositionGeom, 1);
}