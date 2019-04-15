#version 400

layout(location = 0) out vec4 fs_FragColor;
layout(location = 1) out vec4 fs_EmissiveColor;

in vec2 fs_TexCoord;

uniform sampler2D u_PositionAO;
uniform sampler2D u_NormalRoughness;
uniform sampler2D u_AlbedoSpecular;
uniform sampler2D u_EmissiveMetallic;
uniform sampler2D u_IndirectLight;

uniform vec3 u_CameraPosition;

const float PI = 3.14159265359;

#define DIRECTIONAL 0
#define POINT 1

struct Light
{
	vec3 position;
	vec3 color;
	int type;	
};

uniform Light u_Lights[MAX_LIGHTS];
uniform int u_NumLights;

struct DirectionalShadow
{
	sampler2D depthMap;
	mat4 lightSpaceMatrix;
	vec3 position;//should be direction
};

uniform DirectionalShadow u_DirectionalShadows[MAX_LIGHTS];
uniform int u_NumDirectionalShadows;

struct PointShadow
{
    samplerCube depthMap;
    mat4 lightSpaceMatrix;
    vec3 position;
};

uniform PointShadow u_PointShadows[MAX_LIGHTS];
uniform int u_NumPointShadows;


vec3 processLight(Light light, vec3 position, vec3 N, vec3 V, vec3 F0, vec3 albedo, float roughness, float metallic);

float processDirectionalShadow(vec3 position, vec3 normal, DirectionalShadow directionalShadow);

float processPointShadow(vec3 position, PointShadow pointShadow);

// ----------------------------------------------------------------------------
void getValues(sampler2D sampler, out vec3 vec, out float f)
{
	vec4 s = texture(sampler, fs_TexCoord);

	vec = s.rgb;
	f = s.a;
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
    vec3 indirectLight;
    float temp;
    getValues(u_IndirectLight, indirectLight, temp);

    /*Light lights[4];
    lights[0].position = vec3(-10.0, 10.0, 10.0);
    lights[1].position = vec3(10.0, 10.0, 10.0);
    lights[2].position = vec3(-10.0, -10.0, 10.0);
    lights[3].position = vec3(10.0, -10.0, 10.0);

    lights[0].color = vec3(300.0f, 300.0f, 300.0f);
    lights[1].color = vec3(300.0f, 300.0f, 300.0f);
    lights[2].color = vec3(300.0f, 300.0f, 300.0f);
    lights[3].color = vec3(300.0f, 300.0f, 300.0f);*/
	
    vec3 N = normalize(normal);
    vec3 V = normalize(u_CameraPosition - position);

    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, metallic);

    vec3 Lo = vec3(0.0);
    for(int i = 0; i < u_NumLights; i++) 
    {
    	if(u_Lights[i].type == POINT)
    	{
    		Lo += processLight(u_Lights[i], position, N, V, F0, albedo, roughness, metallic);
    	}
    }

    vec3 ambient = vec3(0.0);//vec3(0.03) * albedo * ao;

    vec3 color = ambient + Lo;
	
    float shadow = (u_NumDirectionalShadows + u_NumPointShadows) > 0 ? 1.0 : 0.0;

    for(int i = 0; i < u_NumDirectionalShadows; i++)
    {
    	shadow = min(shadow, processDirectionalShadow(position, normal, u_DirectionalShadows[i]));
    }

    for(int i = 0; i < u_NumPointShadows; i++)
    {
        shadow = min(shadow, processPointShadow(position, u_PointShadows[i]));
    }

    color *= (1.0 - shadow);

    indirectLight *= 0.01;//0.1;

    fs_FragColor = vec4(color + indirectLight, 1.0);//color +
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

float processDirectionalShadow(vec3 position, vec3 normal, DirectionalShadow directionalShadow)
{
	vec4 fragPosLightSpace = directionalShadow.lightSpaceMatrix * vec4(position, 1.0);

    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    float closestDepth = texture(directionalShadow.depthMap, projCoords.xy).r; 
    float currentDepth = projCoords.z;

    vec3 lightDir = normalize(directionalShadow.position - position);//u_LightDirection;

    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);

    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(directionalShadow.depthMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(directionalShadow.depthMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
    
    if(projCoords.z > 1.0)
    {
        shadow = 0.0;
    }
        
    return shadow;
}

vec3 gridSamplingDisk[20] = vec3[]
(
   vec3(1, 1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1, 1,  1), 
   vec3(1, 1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
   vec3(1, 1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1, 1,  0),
   vec3(1, 0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1, 0, -1),
   vec3(0, 1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0, 1, -1)
);


float processPointShadow(vec3 position, PointShadow pointShadow)
{
    vec3 fragToLight = position - pointShadow.position;

    float currentDepth = length(fragToLight);
    // test for shadows
    // float bias = 0.05; // we use a much larger bias since depth is now in [near_plane, far_plane] range
    // float shadow = currentDepth -  bias > closestDepth ? 1.0 : 0.0;
    // PCF
    // float shadow = 0.0;
    // float bias = 0.05; 
    // float samples = 4.0;
    // float offset = 0.1;
    // for(float x = -offset; x < offset; x += offset / (samples * 0.5))
    // {
        // for(float y = -offset; y < offset; y += offset / (samples * 0.5))
        // {
            // for(float z = -offset; z < offset; z += offset / (samples * 0.5))
            // {
                // float closestDepth = texture(depthMap, fragToLight + vec3(x, y, z)).r; // use lightdir to lookup cubemap
                // closestDepth *= far_plane;   // Undo mapping [0;1]
                // if(currentDepth - bias > closestDepth)
                    // shadow += 1.0;
            // }
        // }
    // }
    // shadow /= (samples * samples * samples);

    float far_plane = 25.0;

    float shadow = 0.0;
    float bias = 0.15;
    int samples = 20;
    float viewDistance = length(u_CameraPosition - position);
    float diskRadius = (1.0 + (viewDistance / far_plane)) / 25.0;
    for(int i = 0; i < samples; ++i)
    {
        float closestDepth = texture(pointShadow.depthMap, fragToLight + gridSamplingDisk[i] * diskRadius).r;
        closestDepth *= far_plane;   // undo mapping [0;1]
        if(currentDepth - bias > closestDepth)
            shadow += 1.0;
    }
    shadow /= float(samples);
        
    // display closestDepth as debug (to visualize depth cubemap)
    // FragColor = vec4(vec3(closestDepth / far_plane), 1.0);    
    return shadow;
}