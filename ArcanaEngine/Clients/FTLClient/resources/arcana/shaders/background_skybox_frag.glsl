#version 400

layout(location = 0) out vec4 fs_FragColor;
layout(location = 1) out vec4 fs_EmissiveColor;

in vec3 fs_TexCoords;

uniform samplerCube u_SkyboxTexture;
uniform float u_EmissiveThreshold;

void main()
{
	vec4 color = texture(u_SkyboxTexture, fs_TexCoords);
    fs_FragColor = pow(color, vec4(2.2));
	
	float luminance = 0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b;
	
	if(luminance > u_EmissiveThreshold)
	{
		fs_EmissiveColor = color;
	}
	else
	{
		fs_EmissiveColor = vec4(0.0);
	}
}