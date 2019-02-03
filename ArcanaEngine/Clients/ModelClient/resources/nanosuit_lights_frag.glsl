#version 400

layout(location = 0) out vec4 fs_PositionAO;
layout(location = 1) out vec4 fs_NormalRoughness;
layout(location = 2) out vec4 fs_AlbedoSpecular;
layout(location = 3) out vec4 fs_EmissiveMetallic;

in vec3 fs_Position;
in vec3 fs_Normal;
in vec2 fs_TexCoord;

uniform vec3 baseColor;
uniform vec3 emissive;
uniform float roughness;
uniform float metallic;

void main()
{
	fs_PositionAO = vec4(fs_Position, 1.0);
	fs_NormalRoughness = vec4(fs_Normal, 0.5);
	fs_AlbedoSpecular = vec4(baseColor, 1.0);
	fs_EmissiveMetallic = vec4(emissive, 0.0);
}