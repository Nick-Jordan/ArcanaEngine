#version 400

layout(location = 0) out vec4 fs_FragColor;

in vec2 fs_TexCoord;

uniform sampler2D u_Texture;
uniform float u_Exposure;


const float A = 0.15;
const float B = 0.50;
const float C = 0.10;
const float D = 0.20;
const float E = 0.02;
const float F = 0.30;
const float W = 11.2;

vec3 tonemapper(vec3 x)
{
	return ((x * (A * x + C * B) + D * E) / (x * (A * x + B) + D * F)) - E / F;
}

void main()
{
	vec3 color = texture(u_Texture, fs_TexCoord).rgb;
	//color *= 16;  // Hardcoded Exposure Adjustment

	vec3 map = tonemapper(u_Exposure * color);

	vec3 whiteScale = 1.0f / tonemapper(vec3(1.0));
	vec3 result = map * whiteScale;
      
	result = pow(result, vec3(1 / 2.2));
	
	fs_FragColor = vec4(result, 1.0);
}