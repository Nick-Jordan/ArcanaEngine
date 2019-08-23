#version 400

layout(location = 0) out vec4 fs_PositionAO;
layout(location = 1) out vec4 fs_NormalRoughness;
layout(location = 2) out vec4 fs_AlbedoSpecular;
layout(location = 3) out vec4 fs_EmissiveMetallic;
layout(location = 4) out vec4 fs_IndirectLight;

in vec3 fs_Position;
in vec3 fs_Normal;
in vec2 fs_TexCoord;

#include "resources/arcana/shaders/utils/math.glsl"
#include "resources/arcana/shaders/utils/indirectLighting.glsl"

void main()
{
	vec3 indirectLight = getIndirectLight(u_IndirectLightData, fs_Position);

	fs_PositionAO = vec4(fs_Position, 1.0);
	fs_NormalRoughness = vec4(fs_Normal, 1.0);
	fs_AlbedoSpecular = vec4(1.0);
	fs_EmissiveMetallic = vec4(0.0, 0.0, 0.0, 0.0);
	fs_IndirectLight = vec4(indirectLight, 0.0);
}