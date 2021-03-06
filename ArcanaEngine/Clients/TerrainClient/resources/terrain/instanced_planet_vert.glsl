#version 400

layout(location = 0) in vec3 vs_Position;

//deformation
layout(location = 1) in vec4 deformation_Offset;
layout(location = 2) in vec4 deformation_Camera;
layout(location = 3) in mat4 deformation_ScreenQuadCorners;
layout(location = 7) in mat4 deformation_ScreenQuadVerticals;
layout(location = 11) in vec4 deformation_ScreenQuadCornerNorms;
//layout(location = 12) in mat4 deformation_TangentFrameToWorld;
layout(location = 12) in vec4 vs_TileIDs;

out vec3 fs_Position;
out vec2 fs_TexCoord;
out vec4 fs_Offset;
out vec3 fs_TerrainPosition;
out vec3 fs_SphereNormal;
out float fs_Temperature;
out float fs_Humidity;

out float fs_TileID;

uniform struct {
	vec2 blending;
	float radius;
} deformation;

out float fs_HALF_FCOEF;
out float fs_LogZ;

uniform mat4 u_ProjectionMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_ModelMatrix;

uniform sampler2DArray u_ElevationSampler;

void main()
{
	fs_TexCoord = vs_Position.xy;
	
	fs_TileID = vs_TileIDs.x;

	vec2 v = abs(deformation_Camera.xy - vs_Position.xy);
	float d = max(max(v.x, v.y), deformation_Camera.z);
	float blend = clamp((d - deformation.blending.x) / deformation.blending.y, 0.0, 1.0);

	float R = deformation.radius;
	mat4 C = deformation_ScreenQuadCorners;
	mat4 N = deformation_ScreenQuadVerticals;
	vec4 L = deformation_ScreenQuadCornerNorms;
	vec3 P = vec3(vs_Position.xy * deformation_Offset.z + deformation_Offset.xy, R);

	vec4 uvUV = vec4(vs_Position.xy, vec2(1.0) - vs_Position.xy);
	vec4 alpha = uvUV.zxzx * uvUV.wwyy;
	vec4 alphaPrime = alpha * L / dot(alpha, L);

	fs_TerrainPosition = vec3(deformation.radius * normalize(mat3(u_ModelMatrix) * P));
	float h = texture(u_ElevationSampler, vec3(fs_TexCoord, fs_TileID)).r;

	float k = min(length(P) / dot(alpha, L) * 1.0000003, 1.0);
	float hPrime = (h + R * (1.0 - k)) / k;

#ifdef CUBE_PROJECTION
	gl_Position = u_ProjectionMatrix * u_ViewMatrix * u_ModelMatrix * vec4(P + vec3(0.0, 0.0, h), 1.0);
#else
	gl_Position = (C + hPrime * N) * alphaPrime;
#endif

	fs_Position = (deformation.radius + h) * normalize(mat3(u_ModelMatrix) * P);
	fs_SphereNormal = normalize(fs_TerrainPosition);
	fs_Offset = deformation_Offset;

	fs_Temperature = 0.0;
	fs_Humidity = 0.0;

	float u_ZFar = pow(10.0, 10.0);

	float FCOEF = 2.0 / log2(u_ZFar + 1.0);
	fs_HALF_FCOEF = 0.5 * FCOEF;
	gl_Position.z = (log2(max(0.000001, 1.0 + gl_Position.w)) * FCOEF - 1.0) * gl_Position.w;
	fs_LogZ = 1.0 + gl_Position.w;
}
