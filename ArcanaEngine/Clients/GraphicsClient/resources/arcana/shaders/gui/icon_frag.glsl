#version 400

out vec4 fs_FragColor;

in vec2 fs_TexCoord;

uniform sampler2D u_IconTexture;

uniform vec4 u_Color;

void main()
{
	fs_FragColor = texture(u_IconTexture, fs_TexCoord) * u_Color;
}