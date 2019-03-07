#version 400

layout(location = 0) out vec4 fs_FragColor;
layout(location = 1) out vec4 fs_EmissiveColor;

in vec2 fs_TexCoord;

uniform sampler2D u_PositionAO;
uniform sampler2D u_NormalRoughness;
uniform sampler2D u_AlbedoSpecular;
uniform sampler2D u_EmissiveMetallic;

uniform vec3 u_CameraPosition;

const float PI = 3.14159265359;

struct Light
{
	vec3 position;
	vec3 color;	
};

uniform Light u_Lights[MAX_LIGHTS];
uniform int u_NumLights;

vec3 processLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 albedo)
{
    vec3 lightDir = normalize(light.position - fragPos);

    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 14.0);

    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (1.0 + 0.09 * distance + 
  			     0.032 * (distance * distance));    

	vec3 ambient  = 0.04 * albedo;
    vec3 diffuse  = diff * albedo;
    vec3 specular = vec3(spec);
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
} 

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
	
	vec3 color = vec3(0.0);
	
	for(int i = 0; i < u_NumLights; i++) 
    {
    	color += processLight(u_Lights[i], normal, position, normalize(u_CameraPosition - position), albedo);
    } 

    fs_FragColor = vec4(color, 1.0);
    fs_EmissiveColor = vec4(emissive, 1.0);
}