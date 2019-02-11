#version 450 core

#define POINT_LIGHT_INTENSITY 1

#define DIST_FACTOR 1.1f
#define CONSTANT 1
#define LINEAR 0
#define QUADRATIC 1

float attenuate(float dist){ dist *= DIST_FACTOR; return 1.0f / (CONSTANT + LINEAR * dist + QUADRATIC * dist * dist); }

struct Light 
{
	vec3 position;
	vec3 color;
};

uniform vec3 baseColor;
uniform float roughness;
uniform float metallic;
uniform vec3 emissive;

uniform Light u_Lights[MAX_LIGHTS];
uniform int u_NumLights;
uniform vec3 u_CameraPosition;
layout(rgba8) uniform image3D voxelTexture;

in vec3 worldPositionFrag;
in vec3 normalFrag;

vec3 calculatePointLight(const Light light)
{
	const vec3 direction = normalize(light.position - worldPositionFrag);
	const float distanceToLight = distance(light.position, worldPositionFrag);
	const float attenuation = attenuate(distanceToLight);
	const float d = max(dot(normalize(normalFrag), direction), 0.0f);
	return d * POINT_LIGHT_INTENSITY * attenuation * light.color;
};

vec3 scaleAndBias(vec3 p) { return 0.5f * p + vec3(0.5f); }

bool isInsideCube(const vec3 p, float e) { return abs(p.x) < 1 + e && abs(p.y) < 1 + e && abs(p.z) < 1 + e; }

void main()
{
	vec3 color = vec3(0.0f);
	if(!isInsideCube(worldPositionFrag, 0)) return;

	for(uint i = 0; i < u_NumLights; ++i)
	{
		color += calculatePointLight(u_Lights[i]);
	}
	
	float specularReflectivity = 0.5;
	float diffuseReflectivity = 1.0;
	vec3 specularColor = vec3(1.0);
	float transparency = 0.0;
	
	vec3 spec = specularReflectivity * specularColor;
	vec3 diff = diffuseReflectivity * baseColor;
	
	color = (diff + spec) * color + emissive;

	vec3 voxel = scaleAndBias(worldPositionFrag);
	ivec3 dim = imageSize(voxelTexture);
	float alpha = pow(1 - transparency, 4);
	vec4 res = alpha * vec4(vec3(color), 1);
    imageStore(voxelTexture, ivec3(dim * voxel), res);
}