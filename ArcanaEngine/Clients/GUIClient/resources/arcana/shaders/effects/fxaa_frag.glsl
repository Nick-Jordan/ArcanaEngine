#version 400

layout(location = 0) out vec4 fs_FragColor;

in vec2 fs_TexCoord;

uniform sampler2D u_Texture;

const vec2 texelStep = vec2(0.01);

const float mulReduce = 1.0f;
const float minReduce = 1.0f;
const float maxSpan = 1.0f;
const float lumaThreshold = 0.01f;

void main(void)
{
    vec3 rgbM = texture(u_Texture, fs_TexCoord).rgb;

	vec3 rgbNW = textureOffset(u_Texture, fs_TexCoord, ivec2(-1, 1)).rgb;
    vec3 rgbNE = textureOffset(u_Texture, fs_TexCoord, ivec2(1, 1)).rgb;
    vec3 rgbSW = textureOffset(u_Texture, fs_TexCoord, ivec2(-1, -1)).rgb;
    vec3 rgbSE = textureOffset(u_Texture, fs_TexCoord, ivec2(1, -1)).rgb;

	const vec3 toLuma = vec3(0.299, 0.587, 0.114);
	
	float lumaNW = dot(rgbNW, toLuma);
	float lumaNE = dot(rgbNE, toLuma);
	float lumaSW = dot(rgbSW, toLuma);
	float lumaSE = dot(rgbSE, toLuma);
	float lumaM = dot(rgbM, toLuma);

	float lumaMin = min(lumaM, min(min(lumaNW, lumaNE), min(lumaSW, lumaSE)));
	float lumaMax = max(lumaM, max(max(lumaNW, lumaNE), max(lumaSW, lumaSE)));
	
	if (lumaMax - lumaMin <= lumaMax * lumaThreshold)
	{
		fs_FragColor = vec4(rgbM, 1.0);	
		return;
	}  
	
	vec2 samplingDirection;	
	samplingDirection.x = -((lumaNW + lumaNE) - (lumaSW + lumaSE));
    samplingDirection.y =  ((lumaNW + lumaSW) - (lumaNE + lumaSE));
    
    float samplingDirectionReduce = max((lumaNW + lumaNE + lumaSW + lumaSE) * 0.25 * mulReduce, minReduce);

	float minSamplingDirectionFactor = 1.0 / (min(abs(samplingDirection.x), abs(samplingDirection.y)) + samplingDirectionReduce);
    
    samplingDirection = clamp(samplingDirection * minSamplingDirectionFactor, vec2(-maxSpan), vec2(maxSpan)) * texelStep;
	
	vec3 rgbSampleNeg = texture(u_Texture, fs_TexCoord + samplingDirection * (1.0/3.0 - 0.5)).rgb;
	vec3 rgbSamplePos = texture(u_Texture, fs_TexCoord + samplingDirection * (2.0/3.0 - 0.5)).rgb;

	vec3 rgbTwoTab = (rgbSamplePos + rgbSampleNeg) * 0.5;  

	vec3 rgbSampleNegOuter = texture(u_Texture, fs_TexCoord + samplingDirection * (0.0/3.0 - 0.5)).rgb;
	vec3 rgbSamplePosOuter = texture(u_Texture, fs_TexCoord + samplingDirection * (3.0/3.0 - 0.5)).rgb;
	
	vec3 rgbFourTab = (rgbSamplePosOuter + rgbSampleNegOuter) * 0.25 + rgbTwoTab * 0.5;   
	
	float lumaFourTab = dot(rgbFourTab, toLuma);
	
	if (lumaFourTab < lumaMin || lumaFourTab > lumaMax)
	{
		fs_FragColor = vec4(rgbTwoTab, 1.0); 
	}
	else
	{
		fs_FragColor = vec4(rgbFourTab, 1.0);
	}
}