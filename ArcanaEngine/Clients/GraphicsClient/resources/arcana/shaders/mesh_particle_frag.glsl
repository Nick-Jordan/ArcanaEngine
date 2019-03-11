#version 400

out vec4 fs_FragColor;

in vec4 fs_Color;
in vec2 fs_TexCoord;

uniform sampler2D u_Texture;

void main()
{
	fs_FragColor = fs_Color;
}