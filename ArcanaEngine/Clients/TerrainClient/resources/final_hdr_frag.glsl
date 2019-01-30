#version 400

layout(location = 0) out vec4 fs_FragColor;

in vec2 fs_TexCoord;

uniform sampler2D u_FinalColor;
uniform sampler2D u_EmissiveColor;
uniform float u_Exposure;

void main()
{             
	const float gamma = 2.2;
	vec3 hdrColor = texture(u_FinalColor, fs_TexCoord).rgb;
	vec3 emissive = texture(u_EmissiveColor, fs_TexCoord).rgb;
	hdrColor += emissive;

	//vec3 result = hdrColor / (hdrColor + vec3(1.0));

	vec3 result = vec3(1.0) - exp(-hdrColor * u_Exposure);

	result = pow(result, vec3(1.0 / gamma));
	fs_FragColor = vec4(result, 1.0);
}