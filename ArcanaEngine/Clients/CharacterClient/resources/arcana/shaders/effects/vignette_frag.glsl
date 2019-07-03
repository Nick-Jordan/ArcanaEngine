#version 400

layout(location = 0) out vec4 fs_FragColor;

in vec2 fs_TexCoord;

uniform sampler2D u_Texture;

uniform float u_VignetteScale;
uniform sampler2D u_VignetteTexture;

void main()
{             
	vec4 color = texture(u_Texture, fs_TexCoord);
	vec4 vignette = texture(u_VignetteTexture, fs_TexCoord);

	fs_FragColor = mix(color, vignette * u_VignetteScale, vignette.a * u_VignetteScale);
}