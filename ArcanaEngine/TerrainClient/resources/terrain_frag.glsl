#version 400

layout(location = 0) out vec4 fs_FragColor;

in vec4 fs_Position;
in vec2 fs_TexCoord;
in float fs_Height;
in vec3 fs_TerrainPosition;
in vec3 fs_Normal;

in float fs_HALF_FCOEF;
in float fs_LogZ;

#include "resources/test4.txt"

uniform struct {
	vec4 offset;
	vec4 camera;
	vec2 blending;
	float radius;
	mat3 localToWorld;
	mat4 screenQuadCorners;
	mat4 screenQuadVerticals;
	vec4 screenQuadCornerNorms;
	mat3 tangentFrameToWorld;
} deformation;

struct samplerTile 
{
	sampler2DArray tilePool;
	float tileLayer;
	vec3 tileCoords;
	int childIndex;
	int parentLevel;
};

vec4 textureTile(samplerTile tex, vec2 uv) 
{
	return texture(tex.tilePool, vec3(uv, tex.tileLayer));
}

uniform samplerTile surfaceSampler;

vec3 getSurface(vec2 coords)
{
	if(surfaceSampler.childIndex == 0)
		coords = coords * 0.5;
	if(surfaceSampler.childIndex == 1)
		coords = coords * 0.5 + vec2(0.5, 0.0);
	if(surfaceSampler.childIndex == 2)
		coords = coords * 0.5 + vec2(0.0, 0.5);
	if(surfaceSampler.childIndex == 3)
		coords = coords * 0.5 + vec2(0.5, 0.5);

	return textureTile(surfaceSampler, coords).xyz;
}

uniform samplerTile elevationSampler;

float getHeight(vec2 coords, float blend)
{
	float factor = 1.0 / pow(2.0, float(elevationSampler.parentLevel));

	if(elevationSampler.childIndex == 0)
		coords = coords * factor;
	if(elevationSampler.childIndex == 1)
		coords = coords * factor + vec2(factor, 0.0);
	if(elevationSampler.childIndex == 2)
		coords = coords * factor + vec2(0.0, factor);
	if(elevationSampler.childIndex == 3)
		coords = coords * factor + vec2(factor, factor);

	return textureTile(elevationSampler, coords).x;
}

float planetTerrain(vec3 position)
{
	position = position / 1000.0;

	float tinyDetail = noise(position, 9, 0.15, 0.8, 0, 50.0);
	float smallDetail = noise(position, 6, 0.05, 0.8, 0, 100.0);
	float largeDetail = noise(position, 8, 0.003, 0.8, -2000, 3000.0);
	float n = 10 + tinyDetail + smallDetail + largeDetail;
	float mountains = clamp(noiseCubed(position, 7, 0.002, 0.7, -13, 13), 0, 1)
		* (cellularSquared(position.xyz, 3, 0.05, 0.6, -25000.0, 25000.0)
			+ ridgedNoise(position, 11, 0.03, 0.5, 0.0, 7500.0));
	float plateaus = clamp(noiseCubed(position, 6, 0.003, 0.6, -25, 25), 0, 1)
		* (clamp(noise(position, 5, 0.08, 0.55, -13000.0, 8000.0), 0.0, 1500.0)
			+ clamp(noise(position, 5, 0.1, 0.6, -7000.0, 5000.0), 0.0, 750.0));
	float oceans = clamp(noise(position, 6, 0.00015, 0.75, -5.0, 7.0), 0.0, 1.0)
		* noise(position, 6, 0.002, 0.9, 10000.0, 20000.0);

	n += mountains;
	n += plateaus;
	n -= oceans;

	return n;
}

void main()
{
	//vec3 terrain = getSurface(fs_TexCoord);

	vec3 normal = vec3(0.0);//fs_Normal;

	float height = planetTerrain(fs_TerrainPosition);//getHeight(fs_TexCoord, 0.0);

	vec4 reflectance;

	if (height <= 0.0) 
	{
		reflectance = vec4(0.0, 0.0, 1.0, 0.0);
	}
	else 
	{
		reflectance = mix(vec4(0.02, 0.05, 0.0, 0.0), vec4(0.01, 0.01, 0.0, 0.0), smoothstep(0.0, 4000.0, height));
		reflectance = mix(reflectance, vec4(vec3(0.5), 0.0), smoothstep(5000.0, 6000.0, height));
	}

	float rocks = 0.0;//deformation.offset.z > 500000.0 ? 0.0 : smoothstep(-0.92, -0.85, -normal.z);
	reflectance.rgb = rocks * vec3(0.08) + (1.0 - rocks) * reflectance.rgb;

	fs_FragColor = vec4(reflectance.rgb, 1.0);//vec4(vec3(0.2 + 0.2 * sin(0.1 * length(fs_Position.xy))), 1.0);

	#ifdef DEBUG_QUADTREE
    fs_FragColor.r += mod(dot(floor(deformation.offset.xy / deformation.offset.z + 0.5), vec2(1.0)), 2.0);
    #endif

    gl_FragDepth = log2(fs_LogZ) * fs_HALF_FCOEF;
}