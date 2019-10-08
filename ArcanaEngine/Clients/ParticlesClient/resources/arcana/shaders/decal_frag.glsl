#version 400

layout(location = 0) out vec4 fs_PositionAO;
layout(location = 1) out vec4 fs_NormalRoughness;
layout(location = 2) out vec4 fs_AlbedoSpecular;
layout(location = 3) out vec4 fs_EmissiveMetallic;
layout(location = 4) out vec4 fs_IndirectLight;

in vec3 fs_WorldPosition;
in vec4 fs_Position;
in float fs_Depth;
in vec4 fs_PositionCS;
in vec4 fs_PositionVS;
in vec3 fs_Color;
in float fs_Opacity;
in vec4 fs_TexCoords;
in mat4 fs_InverseTransform;

uniform vec2 u_Resolution;
uniform float u_FarClip;
uniform sampler2D u_DepthSampler;
uniform mat4 u_InverseViewMatrix;
uniform mat4 u_InverseProjectionMatrix;
//uniform mat4 u_InverseModelMatrix; //keep actor transform??
uniform mat4 u_ViewMatrix;
const float u_AngleFadeWidth = 0.17;//make uniform


struct Decal
{
	float angleCutoff;
	vec3 direction;
	vec4 color;
	
	sampler2D albedo;
	float albedoBlendFactor;
	
	sampler2D normals;
	float normalsBlendFactor;
	
	sampler2D roughness;
	float roughnessBlendFactor;
	
	sampler2D specular;
	float specularBlendFactor;
	
	sampler2D emissive;
	float emissiveBlendFactor;
	
	sampler2D metallic;
	float metallicBlendFactor;
};

uniform Decal u_Decal;

uniform sampler2D u_AlbedoSpecular;
uniform sampler2D u_NormalRoughness;
uniform sampler2D u_EmissiveMetallic;
uniform sampler2D u_IndirectLight;

#include "resources/arcana/shaders/utils/math.glsl"

vec3 calculatePixelWorldPosition(vec4 positionCS, vec4 positionVS, float depth);
vec2 calculateDecalTexCoord(vec3 worldPosition);//vec2
vec3 clipPixelNormal(vec3 worldPosition, out vec3 tangent, out vec3 binormal, out float alpha);

vec4 albedoSpecValue(vec2 texCoord, vec2 decalTexCoord, float alpha);
vec4 normalRoughnessValue(vec2 texCoord, vec2 decalTexCoord, vec3 worldPosition, vec3 pixelNormal, vec3 pixelTangent, vec3 pixelBinormal, float alpha);
vec4 emissiveMetallicValue(vec2 texCoord, vec2 decalTexCoord, float alpha);

void main()
{
	vec3 worldPosition = calculatePixelWorldPosition(fs_PositionCS, fs_PositionVS, fs_Depth);
	vec2 decalTexCoord = calculateDecalTexCoord(worldPosition);

	vec3 pixelTangent;
	vec3 pixelBinormal;
	float alpha;
	vec3 pixelNormal = clipPixelNormal(worldPosition, pixelTangent, pixelBinormal, alpha);

	vec2 texCoord = gl_FragCoord.xy / (u_Resolution.xy);
	vec4 albedoSpecular = albedoSpecValue(texCoord, decalTexCoord, alpha);
	vec4 normalRoughness = normalRoughnessValue(texCoord, decalTexCoord, worldPosition, pixelNormal, pixelTangent, pixelBinormal, alpha);
	vec4 emissiveMetallic = emissiveMetallicValue(texCoord, decalTexCoord, alpha);
			
	fs_PositionAO = vec4(worldPosition, 0.0);
	fs_NormalRoughness = normalRoughness;
	fs_AlbedoSpecular = albedoSpecular;
	fs_EmissiveMetallic = emissiveMetallic;
	fs_IndirectLight = texture(u_IndirectLight, texCoord);
}

void clip(float x)
{
	if(x < 0) discard;
}

void clip(vec3 v)
{
	if(v.x < 0 || v.y < 0 || v.z < 0) discard;
}

void zTest(float zBufferDepth, float pixelDepth) 
{
	clip(zBufferDepth - pixelDepth);
}

vec3 reconstructWorldPosition(vec3 viewRay, float depth, mat4 inverseViewMatrix)
{
	vec3 viewPosition = viewRay * depth;

	return (inverseViewMatrix * vec4(viewPosition, 1)).xyz;
}

float linearizeDepth(float depth)
{
	float u_NearClip = 0.1;//temp
	depth = 2.0 * depth - 1.0;
    return 2.0 * u_NearClip * u_FarClip / (u_FarClip + u_NearClip - depth * (u_FarClip - u_NearClip));
}

vec3 calculatePixelWorldPosition(vec4 positionCS, vec4 positionVS, float depth)
{
	/*vec2 screenPos = positionCS.xy / positionCS.w;
	vec2 texCoord = vec2(
		(1 + screenPos.x) / 2 + (0.5 / u_Resolution.x),
		(1 - screenPos.y) / 2 + (0.5 / u_Resolution.y)
	);*/
	vec2 texCoord = gl_FragCoord.xy / (u_Resolution.xy);
	float sampledDepth = texture(u_DepthSampler, texCoord).x;
	
	sampledDepth = linearizeDepth(sampledDepth);
	depth = linearizeDepth(depth);
	
	#ifdef DECAL_ZTEST
	zTest(sampledDepth, depth);
	#endif
	
	vec3 frustumRay = positionVS.xyz * (1.0 / -positionVS.z);//* u_FarClip
	return reconstructWorldPosition(frustumRay, sampledDepth, u_InverseViewMatrix);
}

vec3 clipPixelNormal(vec3 worldPosition, out vec3 tangent, out vec3 binormal, out float alpha)
{
	vec3 ddxWp = dFdx(worldPosition);
	vec3 ddyWp = dFdy(worldPosition);
	vec3 normal = normalize(cross(ddyWp, ddxWp));

	float angle = acos(dot(-normal, u_Decal.direction));

	float difference = u_Decal.angleCutoff - angle;
	
	clip(difference);

	alpha = clamp(difference / u_AngleFadeWidth, 0.0, 1.0);

	binormal = normalize(ddxWp);
	tangent = normalize(ddyWp);

	return normal;
}

vec2 calculateDecalTexCoord(vec3 worldPosition)
{
	vec4 objectPosition = fs_InverseTransform * vec4(worldPosition, 1.0);
	
	clip(vec3(0.5) - abs(objectPosition.xyz));

	return range(objectPosition.xy + vec2(0.5), vec2(0.0), vec2(1.0), fs_TexCoords.xy, fs_TexCoords.zw);
}

vec4 albedoSpecValue(vec2 texCoord, vec2 decalTexCoord, float alpha)
{	
	vec4 s = texture(u_AlbedoSpecular, texCoord);

	if(u_Decal.albedoBlendFactor * alpha * fs_Opacity > 0.0f)
	{
		vec4 t = texture(u_Decal.albedo, decalTexCoord);
		s.xyz = mix(s.xyz, t.xyz * u_Decal.color.xyz, t.w * alpha * u_Decal.albedoBlendFactor * fs_Opacity); //fs_Color??
	}
	
	if(u_Decal.specularBlendFactor * alpha * fs_Opacity > 0.0f)
	{
		vec4 t = texture(u_Decal.specular, decalTexCoord);
		s.w = mix(s.w, t.x, t.w * alpha * u_Decal.specularBlendFactor * fs_Opacity);
	}
	
	return s;
}

vec4 emissiveMetallicValue(vec2 texCoord, vec2 decalTexCoord, float alpha)
{	
	vec4 s = texture(u_EmissiveMetallic, texCoord);

	if(u_Decal.emissiveBlendFactor * alpha * fs_Opacity > 0.0f)
	{
		vec4 t = texture(u_Decal.emissive, decalTexCoord);
		s.xyz = mix(s.xyz, t.xyz * u_Decal.color.xyz, t.w * alpha * u_Decal.emissiveBlendFactor * fs_Opacity);
	}
	
	if(u_Decal.metallicBlendFactor * alpha * fs_Opacity > 0.0f)
	{
		vec4 t = texture(u_Decal.metallic, decalTexCoord);
		s.w = mix(s.w, t.x, t.w * alpha * u_Decal.metallicBlendFactor * fs_Opacity);
	}
	
	return s;
}

vec4 normalRoughnessValue(vec2 texCoord, vec2 decalTexCoord, vec3 worldPosition, vec3 pixelNormal, vec3 pixelTangent, vec3 pixelBinormal, float alpha)
{
	vec4 s = texture(u_NormalRoughness, texCoord);
	
	if(u_Decal.normalsBlendFactor * alpha * fs_Opacity > 0.0f)
	{
		vec4 normalSample = texture(u_Decal.normals, decalTexCoord);
		vec3 tangentNormal = normalSample.xyz * 2.0 - 1.0;

		vec3 Q1  = dFdx(worldPosition);
		vec3 Q2  = dFdy(worldPosition);
		vec2 st1 = dFdx(decalTexCoord);
		vec2 st2 = dFdy(decalTexCoord);

		vec3 N   = normalize(s.xyz);
		vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
		vec3 B  = -normalize(cross(N, T));
		mat3 TBN = mat3(T, B, N);

		vec3 normal = normalize(TBN * tangentNormal);
		
		s.xyz = mix(s.xyz, normal, normalSample.w * u_Decal.normalsBlendFactor * alpha * fs_Opacity);
	}
	
	if(u_Decal.roughnessBlendFactor * alpha * fs_Opacity > 0.0f)
	{
		vec4 t = texture(u_Decal.roughness, decalTexCoord);
		s.w = mix(s.w, t.x, t.w * u_Decal.roughnessBlendFactor * alpha * fs_Opacity);
	}
	
	return s;
}