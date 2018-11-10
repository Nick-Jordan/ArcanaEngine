#version 400

layout(location = 0) out vec4 fs_FragColor;

in vec4 fs_Color;
in vec3 fs_Position;

uniform vec4 albedo;
uniform vec4 roughness;

void main()
{
	fs_FragColor = albedo * roughness;
}