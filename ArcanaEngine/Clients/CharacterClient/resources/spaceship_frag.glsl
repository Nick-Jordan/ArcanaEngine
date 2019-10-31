#version 400

layout(location = 0) out vec4 fs_PositionAO;
layout(location = 1) out vec4 fs_NormalRoughness;
layout(location = 2) out vec4 fs_AlbedoSpecular;
layout(location = 3) out vec4 fs_EmissiveMetallic;
layout(location = 4) out vec4 fs_LightData;

in vec3 fs_Position;
in vec3 fs_Normal;
in vec2 fs_TexCoord;

uniform sampler2D baseColor;
uniform sampler2D normals;
uniform sampler2D ao;
uniform sampler2D roughness;
uniform sampler2D metallic;
uniform float metallicOffset;
uniform float normalScale;
uniform float materialScale;

uniform vec2 offset;

#include "resources/arcana/shaders/utils/math.glsl"

vec3 getTriPlanarBlend(vec3 normal)
{
	vec3 blending = abs(normal);
	blending = normalize(max(blending, 0.00001));
	float b = (blending.x + blending.y + blending.z);
	blending /= vec3(b, b, b);
	return blending;
}

vec4 triplanar(sampler2D sampler, vec3 normal, float scale)
{
	vec3 blending = getTriPlanarBlend(normal);
	vec4 xaxis = texture(sampler, fs_Position.yz * scale + offset);
	vec4 yaxis = texture(sampler, fs_Position.xz * scale + offset);
	vec4 zaxis = texture(sampler, fs_Position.xy * scale + offset);
	vec4 result = xaxis * blending.x + xaxis * blending.y + zaxis * blending.z;
	return result;
}

vec3 calculateNormal(vec2 texCoords, vec3 sampledNormal)
{
	vec3 tangentNormal = sampledNormal * 2.0 - 1.0;

	vec3 Q1  = dFdx(fs_Position);
    vec3 Q2  = dFdy(fs_Position);
    vec2 st1 = dFdx(texCoords);
    vec2 st2 = dFdy(texCoords);

    vec3 N   = normalize(fs_Normal);
    vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
    vec3 B  = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}

void main()
{	
	float scale = materialScale;

	vec3 normal = calculateNormal(fs_TexCoord * scale, triplanar(normals, fs_Normal, scale).xyz);

	fs_PositionAO = vec4(fs_Position, triplanar(ao, fs_Normal, scale).x);
	fs_NormalRoughness = vec4(normalize(mix(fs_Normal, normal, normalScale)), triplanar(roughness, fs_Normal, scale).x);
	fs_AlbedoSpecular = vec4(triplanar(baseColor, fs_Normal, scale).xyz, 1.0);
	fs_EmissiveMetallic = vec4(0.0, 0.0, 0.0, clamp(triplanar(metallic, fs_Normal, scale).x + metallicOffset, 0, 1));
	fs_LightData = vec4(0);
}

/*vec3 getNormal(vec2 texCoords)
{
    vec3 tangentNormal = texture(normals, texCoords).xyz * 2.0 - 1.0;

    vec3 Q1  = dFdx(fs_Position);
    vec3 Q2  = dFdy(fs_Position);
    vec2 st1 = dFdx(texCoords);
    vec2 st2 = dFdy(texCoords);

    vec3 N   = normalize(fs_Normal);
    vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
    vec3 B  = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}

void main()
{
	vec2 texCoords = fs_TexCoord * 4.0;

	fs_PositionAO = vec4(fs_Position, texture(ao, texCoords).x);
	fs_NormalRoughness = vec4(getNormal(texCoords), roughness);
	fs_AlbedoSpecular = vec4(texture(baseColor, texCoords).xyz, 1.0);
	fs_EmissiveMetallic = vec4(0.0, 0.0, 0.0, metallic);
	fs_LightData = vec4(0);
}*/