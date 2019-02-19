#version 400

layout(location = 0) out vec4 fs_FragColor;
layout(location = 1) out vec4 fs_EmissiveColor;

in vec3 fs_Position;
in vec3 fs_Normal;
in vec2 fs_TexCoord;

uniform vec3 baseColor;
uniform float roughness;
uniform float metallic;

void main()
{
	fs_FragColor = vec4(fs_Normal, 1.0);
	fs_EmissiveColor = vec4(baseColor, 1.0);
}