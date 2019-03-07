#version 400

layout(location = 0) in vec3 vs_Position;
layout(location = 1) in vec3 vs_Normal;
layout(location = 2) in vec2 vs_TexCoord0;

out vec4 fs_PositionLightSpace;
out vec3 fs_Normal;
out vec3 fs_Position;

uniform mat4 u_ModelMatrix;
uniform mat3 u_NormalMatrix;
uniform mat4 u_LightSpaceMatrix;

void main()
{
	fs_Position = (u_ModelMatrix * vec4(vs_Position, 1.0)).xyz;
	fs_Normal = u_NormalMatrix * vs_Normal;

	fs_PositionLightSpace = u_LightSpaceMatrix * vec4(fs_Position, 1.0);

	gl_Position = vec4(fs_Position, 1.0);
}