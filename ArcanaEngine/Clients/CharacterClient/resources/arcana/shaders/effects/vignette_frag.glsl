#version 400

layout(location = 0) out vec4 fs_FragColor;

in vec2 fs_TexCoord;

uniform sampler2D u_Texture;
uniform sampler2D u_ExtraTextures[1];

//uniform float u_VignetteScale;//test

void main()
{             
	float u_VignetteScale = 1.0;

	vec4 color = texture(u_Texture, fs_TexCoord);
	vec4 vignette = texture(u_ExtraTextures[0], fs_TexCoord);

	fs_FragColor = color * vignette * u_VignetteScale;
}