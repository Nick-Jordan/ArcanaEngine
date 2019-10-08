#version 450 core

#define TSQRT2 2.828427
#define SQRT2 1.414213
#define ISQRT2 0.707106

#define MIPMAP_HARDCAP 5.4f
#define VOXEL_SIZE (1/64.0)
#define SHADOWS 1
#define DIFFUSE_INDIRECT_FACTOR 0.52f

#define SPECULAR_MODE 1
#define SPECULAR_FACTOR 4.0f
#define SPECULAR_POWER 65.0f
#define DIRECT_LIGHT_INTENSITY 0.96f

#define DIST_FACTOR 1.1f
#define CONSTANT 1
#define LINEAR 0
#define QUADRATIC 1

layout(location = 0) out vec4 fs_FragColor;
layout(location = 1) out vec4 fs_EmissiveColor;

in vec2 fs_TexCoord;

uniform sampler2D u_PositionAO;
uniform sampler2D u_NormalRoughness;
uniform sampler2D u_AlbedoSpecular;
uniform sampler2D u_EmissiveMetallic;

uniform sampler3D voxelTexture;

uniform vec3 u_CameraPosition;

const float PI = 3.14159265359;

struct Light
{
	vec3 position;
	vec3 color;	
};

uniform Light u_Lights[MAX_LIGHTS];
uniform int u_NumLights;

// ----------------------------------------------------------------------------
void getValues(sampler2D sampler, out vec3 vec, out float f)
{
	vec4 s = texture(sampler, fs_TexCoord);

	vec = s.rgb;
	f = s.a;
}

float attenuate(float dist){ dist *= DIST_FACTOR; return 1.0f / (CONSTANT + LINEAR * dist + QUADRATIC * dist * dist); }

vec3 orthogonal(vec3 u){
	u = normalize(u);
	vec3 v = vec3(0.99146, 0.11664, 0.05832);
	return abs(dot(u, v)) > 0.99999f ? cross(u, vec3(0, 1, 0)) : cross(u, v);
}

vec3 scaleAndBias(const vec3 p) { return 0.5f * p + vec3(0.5f); }

bool isInsideCube(const vec3 p, float e) { return abs(p.x) < 1 + e && abs(p.y) < 1 + e && abs(p.z) < 1 + e; }

float traceShadowCone(vec3 from, vec3 direction, float targetDistance, vec3 normal)
{
	from += normal * 0.05f;

	float acc = 0;

	float dist = 3 * VOXEL_SIZE;

	const float STOP = targetDistance - 16 * VOXEL_SIZE;

	while(dist < STOP && acc < 1)
	{	
		vec3 c = from + dist * direction;
		if(!isInsideCube(c, 0)) break;
		c = scaleAndBias(c);
		float l = pow(dist, 2);
		float s1 = 0.062 * textureLod(voxelTexture, c, 1 + 0.75 * l).a;
		float s2 = 0.135 * textureLod(voxelTexture, c, 4.5 * l).a;
		float s = s1 + s2;
		acc += (1 - acc) * s;
		dist += 0.9 * VOXEL_SIZE * (1 + 0.05 * l);
	}
	return 1 - pow(smoothstep(0, 1, acc * 1.4), 1.0 / 1.4);
}	

vec3 traceDiffuseVoxelCone(const vec3 from, vec3 direction)
{
	direction = normalize(direction);
	
	const float CONE_SPREAD = 0.325;

	vec4 acc = vec4(0.0f);

	float dist = 0.1953125;

	while(dist < SQRT2 && acc.a < 1)
	{
		vec3 c = from + dist * direction;
		c = scaleAndBias(from + dist * direction);
		float l = (1 + CONE_SPREAD * dist / VOXEL_SIZE);
		float level = log2(l);
		float ll = (level + 1) * (level + 1);
		vec4 voxel = textureLod(voxelTexture, c, min(MIPMAP_HARDCAP, level));
		acc += 0.075 * ll * voxel * pow(1 - voxel.a, 2);
		dist += ll * VOXEL_SIZE * 2;
	}
	return pow(acc.rgb * 2.0, vec3(1.5));
}

vec3 traceSpecularVoxelCone(vec3 from, vec3 direction, vec3 normal, float maxDistance)
{
	direction = normalize(direction);

	const float OFFSET = 8 * VOXEL_SIZE;
	const float STEP = VOXEL_SIZE;

	from += OFFSET * normal;
	
	vec4 acc = vec4(0.0f);
	float dist = OFFSET;
	
	float specularDiffusion = 0.5;

	while(dist < maxDistance && acc.a < 1)
	{ 
		vec3 c = from + dist * direction;
		if(!isInsideCube(c, 0)) break;
		c = scaleAndBias(c); 
		
		float level = 0.1 * specularDiffusion * log2(1 + dist / VOXEL_SIZE);
		vec4 voxel = textureLod(voxelTexture, c, min(level, MIPMAP_HARDCAP));
		float f = 1 - acc.a;
		acc.rgb += 0.25 * (1 + specularDiffusion) * voxel.rgb * voxel.a * f;
		acc.a += 0.25 * voxel.a * f;
		dist += STEP * (1.0f + 0.125f * level);
	}
	return 1.0 * pow(specularDiffusion + 1, 0.8) * acc.rgb;
}

vec3 indirectDiffuseLight(vec3 normal, vec3 position, vec3 albedo)
{
	float diffuseReflectivity = 1.0;

	const float ANGLE_MIX = 0.5f; // Angle mix (1.0f => orthogonal direction, 0.0f => direction of normal).

	const float w[3] = {1.0, 1.0, 1.0}; // Cone weights.

	const vec3 ortho = normalize(orthogonal(normal));
	const vec3 ortho2 = normalize(cross(ortho, normal));

	const vec3 corner = 0.5f * (ortho + ortho2);
	const vec3 corner2 = 0.5f * (ortho - ortho2);

	const vec3 N_OFFSET = normal * (1 + 4 * ISQRT2) * VOXEL_SIZE;
	const vec3 C_ORIGIN = position + N_OFFSET;

	vec3 acc = vec3(0);

	const float CONE_OFFSET = -0.01;

	acc += w[0] * traceDiffuseVoxelCone(C_ORIGIN + CONE_OFFSET * normal, normal);

	const vec3 s1 = mix(normal, ortho, ANGLE_MIX);
	const vec3 s2 = mix(normal, -ortho, ANGLE_MIX);
	const vec3 s3 = mix(normal, ortho2, ANGLE_MIX);
	const vec3 s4 = mix(normal, -ortho2, ANGLE_MIX);

	acc += w[1] * traceDiffuseVoxelCone(C_ORIGIN + CONE_OFFSET * ortho, s1);
	acc += w[1] * traceDiffuseVoxelCone(C_ORIGIN - CONE_OFFSET * ortho, s2);
	acc += w[1] * traceDiffuseVoxelCone(C_ORIGIN + CONE_OFFSET * ortho2, s3);
	acc += w[1] * traceDiffuseVoxelCone(C_ORIGIN - CONE_OFFSET * ortho2, s4);

	const vec3 c1 = mix(normal, corner, ANGLE_MIX);
	const vec3 c2 = mix(normal, -corner, ANGLE_MIX);
	const vec3 c3 = mix(normal, corner2, ANGLE_MIX);
	const vec3 c4 = mix(normal, -corner2, ANGLE_MIX);

	acc += w[2] * traceDiffuseVoxelCone(C_ORIGIN + CONE_OFFSET * corner, c1);
	acc += w[2] * traceDiffuseVoxelCone(C_ORIGIN - CONE_OFFSET * corner, c2);
	acc += w[2] * traceDiffuseVoxelCone(C_ORIGIN + CONE_OFFSET * corner2, c3);
	acc += w[2] * traceDiffuseVoxelCone(C_ORIGIN - CONE_OFFSET * corner2, c4);

	return DIFFUSE_INDIRECT_FACTOR * diffuseReflectivity * acc * (albedo + vec3(0.001f));
}

vec3 indirectSpecularLight(vec3 viewDirection, vec3 normal, vec3 position, float maxDistance)
{
	float specularReflectivity = 0.5;
	vec3 specularColor = vec3(1.0);

	const vec3 reflection = normalize(reflect(viewDirection, normal));
	return specularReflectivity * specularColor * traceSpecularVoxelCone(position, reflection, normal, maxDistance);
}

vec3 indirectRefractiveLight(vec3 viewDirection, vec3 normal, vec3 position, float maxDistance)
{
	float refractiveIndex = 1.0;///////////////
	float transparency = 0.0;
	vec3 specularColor = vec3(1.0);

	const vec3 refraction = refract(viewDirection, normal, 1.0 / refractiveIndex);
	const vec3 cmix = mix(specularColor, 0.5 * (specularColor + vec3(1)), transparency);
	return cmix * traceSpecularVoxelCone(position, refraction, normal, maxDistance);
}

vec3 calculateDirectLight(const Light light, vec3 position, vec3 normal, vec3 albedo, const vec3 viewDirection)
{
	vec3 lightDirection = light.position - position;
	const float distanceToLight = length(lightDirection);
	lightDirection = lightDirection / distanceToLight;
	const float lightAngle = dot(normal, lightDirection);
	
	float diffuseAngle = max(lightAngle, 0.0f);	
	
	const vec3 reflection = normalize(reflect(viewDirection, normal));
	float specularAngle = max(0, dot(reflection, lightDirection));

	float refractiveAngle = 0;
	/*if(material.transparency > 0.01)
	{
		vec3 refraction = refract(viewDirection, normal, 1.0 / material.refractiveIndex);
		refractiveAngle = max(0, material.transparency * dot(refraction, lightDirection));
	}*/

	float shadowBlend = 1;
	float transparency = 0.0;//////////////////////////////////////////
#if (SHADOWS == 1)
	if(diffuseAngle * (1.0f - transparency) > 0)
		shadowBlend = traceShadowCone(position, lightDirection, distanceToLight, normal);
#endif

	float specularDiffusion = 0.5;

	diffuseAngle = min(shadowBlend, diffuseAngle);
	specularAngle = min(shadowBlend, max(specularAngle, refractiveAngle));
	const float df = 1.0f / (1.0f + 0.25f * specularDiffusion); // Diffusion factor.
	const float specular = SPECULAR_FACTOR * pow(specularAngle, df * SPECULAR_POWER);
	const float diffuse = diffuseAngle * (1.0f - transparency);

	float diffuseReflectivity = 1.0;
	float specularReflectivity = 0.5;
	vec3 specularColor = vec3(1.0);
	
	const vec3 diff = diffuseReflectivity * albedo * diffuse;
	const vec3 spec = specularReflectivity * specularColor * specular;
	const vec3 total = light.color * (diff + spec);
	return attenuate(distanceToLight) * total;
};

vec3 processLight(Light light, vec3 position, vec3 N, vec3 V, vec3 F0, vec3 albedo, float roughness, float metallic);

vec3 directLight(vec3 viewDirection, vec3 position, vec3 normal, vec3 albedo, float roughness, float metallic)
{
	vec3 N = normalize(normal);
    vec3 V = normalize(u_CameraPosition - position);

    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, metallic);

    vec3 Lo = vec3(0.0);
    for(int i = 0; i < u_NumLights; i++) 
    {
    	Lo += processLight(u_Lights[i], position, N, V, F0, albedo, roughness, metallic);
    }   

    vec3 ambient = vec3(0.0);//vec3(0.03) * albedo * ao;

    vec3 color = ambient + Lo;
	return color;
}

void main()
{
	vec3 position;
	float ao;
	getValues(u_PositionAO, position, ao);
	vec3 normal;
	float roughness;
	getValues(u_NormalRoughness, normal, roughness);
	vec3 albedo;
	float specular;
	getValues(u_AlbedoSpecular, albedo, specular);
	vec3 emissive;
	float metallic;
	getValues(u_EmissiveMetallic, emissive, metallic);
	
	
	vec4 color = vec4(0, 0, 0, 1);
	const vec3 viewDirection = normalize(position - u_CameraPosition);

	//color.rgb += indirectDiffuseLight(normal, position, albedo);
	float maxDistance = distance(vec3(abs(position)), vec3(-1));
	//color.rgb += indirectSpecularLight(viewDirection, normal, position, maxDistance);

	//color.rgb = mix(color.rgb, indirectRefractiveLight(viewDirection), material.transparency);

	color.rgb += directLight(viewDirection, position, normal, albedo, roughness, metallic);
	
	color.rgb = texture(voxelTexture, position).rgb;
	
	fs_FragColor = color;
	fs_EmissiveColor = vec4(emissive, 1.0);
}


// ----------------------------------------------------------------------------
float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / max(denom, 0.001);
}
// ----------------------------------------------------------------------------
float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}
// ----------------------------------------------------------------------------
vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}
// ----------------------------------------------------------------------------

vec3 processLight(Light light, vec3 position, vec3 N, vec3 V, vec3 F0, vec3 albedo, float roughness, float metallic)
{
	vec3 L = normalize(light.position - position);
    vec3 H = normalize(V + L);
    float distance = length(light.position - position);
    float attenuation = 1.0 / (distance * distance);
    vec3 radiance = light.color * attenuation;

    float NDF = DistributionGGX(N, H, roughness);   
    float G   = GeometrySmith(N, V, L, roughness);    
    vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);        
        
    vec3 nominator    = NDF * G * F;
    float denominator = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001;
    vec3 specular = nominator / denominator;
        
    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - metallic;	                
            
    float NdotL = max(dot(N, L), 0.0);    
    
    return (kD * albedo / PI + specular) * radiance * NdotL;
}