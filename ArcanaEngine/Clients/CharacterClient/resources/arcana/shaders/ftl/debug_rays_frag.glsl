#version 400

layout(location = 0) out vec4 fs_FragColor;
layout(location = 1) out vec4 fs_EmissiveColor;

in vec3 fs_Color;

void main()
{
	fs_FragColor = vec4(fs_Color, 1.0);
	fs_EmissiveColor = vec4(0.0);
}