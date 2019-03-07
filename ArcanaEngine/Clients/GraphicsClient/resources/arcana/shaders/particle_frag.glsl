#version 400

out vec4 fs_FragColor;

in vec4 fs_Color;

void main()
{
	fs_FragColor = fs_Color;
}