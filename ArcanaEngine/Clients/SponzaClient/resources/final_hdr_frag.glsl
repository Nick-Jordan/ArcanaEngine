#version 400

layout(location = 0) out vec4 fs_FragColor;

in vec2 fs_TexCoord;

uniform sampler2D u_FinalColor;
uniform sampler2D u_EmissiveColor;
uniform float u_Exposure;

const float A = 0.15;
const float B = 0.50;
const float C = 0.10;
const float D = 0.20;
const float E = 0.02;
const float F = 0.30;
const float W = 11.2;

vec3 tonemap(vec3 x)
{
   return ((x * (A * x + C * B) + D * E) / (x * (A * x + B) + D * F)) - E / F;
}

void main()
{             
	const float gamma = 2.2;
	vec3 hdrColor = texture(u_FinalColor, fs_TexCoord).rgb;
	vec3 emissive = texture(u_EmissiveColor, fs_TexCoord).rgb;
	hdrColor += emissive;

	//vec3 result = hdrColor / (hdrColor + vec3(1.0));

	//vec3 result = vec3(1.0) - exp(-hdrColor * u_Exposure);

	vec3 result = tonemap(u_Exposure * hdrColor);

	vec3 whiteScale = 1.0 / tonemap(vec3(W));
	result = result * whiteScale;

	result = pow(result, vec3(1.0 / gamma));
	fs_FragColor = vec4(result, 1.0);
}