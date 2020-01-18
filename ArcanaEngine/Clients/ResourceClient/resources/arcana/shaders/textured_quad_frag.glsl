#version 400

layout(location = 0) out vec4 fs_FragColor;

in vec2 fs_TexCoord;

uniform sampler2D u_Texture;

void main()
{
	fs_FragColor = texture(u_Texture, fs_TexCoord);
}