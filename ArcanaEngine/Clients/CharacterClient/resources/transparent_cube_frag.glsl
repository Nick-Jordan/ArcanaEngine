#version 400

layout(location = 0) out vec4 fs_FragColor;
layout(location = 1) out vec4 fs_EmissiveColor;

in vec3 fs_Position;
in vec3 fs_Normal;
in vec2 fs_TexCoord;

uniform vec4 baseColor;

#include "resources/arcana/shaders/utils/math.glsl"
#include "resources/arcana/shaders/utils/indirectLighting.glsl"

void main()
{
	vec3 indirectLight = getIndirectLight(u_IndirectLightData, fs_Position);

	fs_FragColor = baseColor + vec4(indirectLight, 0.0);
	fs_EmissiveColor = vec4(0.0);
}