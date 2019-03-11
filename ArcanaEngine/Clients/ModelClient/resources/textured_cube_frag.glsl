#version 400

layout(location = 0) out vec4 fs_PositionAO;
layout(location = 1) out vec4 fs_NormalRoughness;
layout(location = 2) out vec4 fs_AlbedoSpecular;
layout(location = 3) out vec4 fs_EmissiveMetallic;

in vec3 fs_Position;
in vec3 fs_Normal;
in vec2 fs_TexCoord;

uniform sampler2D baseColor;
uniform sampler2D roughness;
uniform sampler2D metallic;
uniform sampler2D normals;
uniform sampler2D ao;

void main()
{
	fs_PositionAO = vec4(fs_Position, texture(ao, fs_TexCoord).r);
	fs_NormalRoughness = vec4(fs_Normal, texture(roughness, fs_TexCoord).r);
	fs_AlbedoSpecular = vec4(texture(baseColor, fs_TexCoord).rgb, 1.0);
	fs_EmissiveMetallic = vec4(0.0, 0.0, 0.0, texture(metallic, fs_TexCoord).r);
}