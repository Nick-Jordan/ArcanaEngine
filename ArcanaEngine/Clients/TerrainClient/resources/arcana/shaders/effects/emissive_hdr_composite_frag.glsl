#version 400

layout(location = 0) out vec4 fs_FragColor;

in vec2 fs_TexCoord;

uniform sampler2D u_Texture;
uniform sampler2D u_ExtraTextures[1];

void main()
{             
	vec3 hdrColor = texture(u_Texture, fs_TexCoord).rgb;
	vec3 emissive = texture(u_ExtraTextures[0], fs_TexCoord).rgb;
	hdrColor += emissive;

	fs_FragColor = vec4(hdrColor, 1.0);
}