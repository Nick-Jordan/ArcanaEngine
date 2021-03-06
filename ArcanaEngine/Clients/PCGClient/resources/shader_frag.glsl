#version 400

layout(location = 0) out vec4 fs_FragColor;

in vec2 fs_TexCoord;

#include "resources/test4.txt"

void main()
{
	float n = noise(vec3(fs_TexCoord, 0.0), 20, 0.2, 0.9, 0.0, 1.0);
	fs_FragColor = vec4(vec3(n), 1.0);
}