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

uniform vec2 u_Resolution;
uniform float u_FarClip;
uniform vec3 u_DecalDirection;
uniform float u_AngleCutoff;
uniform sampler2D u_DepthSampler;
uniform mat4 u_InverseViewMatrix;
uniform mat4 u_InverseModelMatrix;
const float u_AngleFadeWidth = 0.17;//make uniform

vec3 calculatePixelWorldPosition(vec4 positionCS, vec4 positionVS, float depth);
vec2 calculateDecalTexCoord(vec3 worldPosition);
vec3 clipPixelNormal(vec3 worldPosition, out vec3 tangent, out vec3 binormal, out float alpha);

void main()
{
	vec3 worldPosition = calculatePixelWorldPosition(fs_PositionCS, fs_PositionVS, fs_Depth);
	vec2 decalTexCoord = calculateDecalTexCoord(worldPosition);

	vec3 pixelTangent;
	vec3 pixelBinormal;
	float alpha;
	vec3 pixelNormal = vec3(-1, 0, 0);// clipPixelNormal(worldPosition, pixelTangent, pixelBinormal, alpha);

	vec3 albedo = vec3(1.0);//albedoValue(decalTexCoord, alpha);*/
	vec3 normal = vec3(-1, 0, 0); //normalValue(decalTexCoord, pixelNormal, pixelTangent, pixelBinormal);
	
	fs_PositionAO = vec4(fs_WorldPosition, 0.0);
	fs_NormalRoughness = vec4(normal, 1.0);
	fs_AlbedoSpecular = vec4(worldPosition.xyz/10.0, 1.0);
	fs_EmissiveMetallic = vec4(0.0);
	fs_IndirectLight = vec4(0.0);
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

vec3 calculatePixelWorldPosition(vec4 positionCS, vec4 positionVS, float depth)
{
	vec2 screenPos = positionCS.xy / positionCS.w;
	vec2 texCoord = vec2(
		(1 + screenPos.x) / 2 + (0.5 / u_Resolution.x),
		(1 - screenPos.y) / 2 + (0.5 / u_Resolution.y)
	);
	float sampledDepth = texture(u_DepthSampler, texCoord).x;

	zTest(sampledDepth, depth);

	vec3 frustumRay = positionVS.xyz * (u_FarClip / -positionVS.z);
	return reconstructWorldPosition(frustumRay, sampledDepth, u_InverseViewMatrix);
}

vec3 clipPixelNormal(vec3 worldPosition, out vec3 tangent, out vec3 binormal, out float alpha)
{
	vec3 ddxWp = dFdx(worldPosition);
	vec3 ddyWp = dFdy(worldPosition);
	vec3 normal = normalize(cross(ddyWp, ddxWp));

	float angle = acos(dot(-normal, u_DecalDirection));

	float difference = u_AngleCutoff - angle;
	
	clip(difference);

	alpha = clamp(difference / u_AngleFadeWidth, 0.0, 1.0);

	binormal = normalize(ddxWp);
	tangent = normalize(ddyWp);

	return normal;
}

vec2 calculateDecalTexCoord(vec3 worldPosition)
{
	vec4 objectPosition = u_InverseModelMatrix * vec4(worldPosition, 1.0);
	
	//clip(vec3(0.5) - abs(objectPosition.xyz));

	return objectPosition.xz + 0.5;
}

vec3 albedoValue(vec2 decalTexCoord, float alpha)
{
	return vec3(1.0);//return tex2D(diffuseSampler, decalTexCoord) * DecalColor * vec4(1, 1, 1, alpha);
}

vec3 normalValue(vec2 decalTexCoord, vec3 pixelNormal, vec3 pixelTangent, vec3 pixelBinormal)
{
	/*//Sample normal value from normal map
	vec4 normalSample = tex2D(normalSampler, decalTexCoord);
		vec3 normal = normalize(normalSample.xyz * 2 - 1);

		//Construct a tangent space to view space conversion
		vec3x3 tangentToView;
	tangentToView[0] = mul(pixelTangent, View);
	tangentToView[1] = mul(pixelBinormal, View);
	tangentToView[2] = mul(pixelNormal, View);

	//Convert normal into view space
	normal = mul(normal, tangentToView);

	//Write out normal value with alpha preserved (this will be used in the decal mixing stage later)
	return vec4(EncodeNormal(normal), 0, normalSample.a);*/
	
	return vec4(-1.0, 0.0, 0.0, 1.0);
}