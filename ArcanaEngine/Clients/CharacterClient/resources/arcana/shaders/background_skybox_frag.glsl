#version 400

layout(location = 0) out vec4 fs_FragColor;
layout(location = 1) out vec4 fs_EmissiveColor;

in vec3 fs_TexCoords;

uniform samplerCube u_SkyboxTexture;
uniform sampler2D u_SkyboxTextureEquirectangular;
uniform float u_EmissiveThreshold;
uniform bool u_Equirectangular;

const vec2 invAtan = vec2(0.1591, 0.3183);
vec2 sampleSphericalMap(vec3 v)
{
    vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
    uv *= invAtan;
    uv += 0.5;
    return uv;
}

void main()
{
	vec4 color;

	if(u_Equirectangular)
	{
		vec2 uv = sampleSphericalMap(normalize(fs_TexCoords));
		color = texture(u_SkyboxTextureEquirectangular, uv);
	}
	else
	{
		color = texture(u_SkyboxTexture, fs_TexCoords);
	}
	
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