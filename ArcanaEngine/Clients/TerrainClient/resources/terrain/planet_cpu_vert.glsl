#version 400

layout(location = 0) in vec3 vs_Position;

out vec3 fs_Position;
out vec2 fs_TexCoord;
out float fs_Height;
out vec3 fs_TerrainPosition;
out vec3 fs_SphereNormal;
out vec3 fs_Normal;
out float fs_Temperature;
out float fs_Humidity;

out float fs_HALF_FCOEF;
out float fs_LogZ;

uniform mat4 u_ProjectionMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_ModelMatrix;

#include "resources/test4.txt"

uniform struct {
	vec4 offset;
	vec4 camera;
	vec2 blending;
	float radius;
	mat4 screenQuadCorners;
	mat4 screenQuadVerticals;
	vec4 screenQuadCornerNorms;
	mat3 tangentFrameToWorld;
} deformation;

void main()
{
	fs_TexCoord = vs_Position.xy;

	vec2 v = abs(deformation.camera.xy - vs_Position.xy);
	float d = max(max(v.x, v.y), deformation.camera.z);
	float blend = clamp((d - deformation.blending.x) / deformation.blending.y, 0.0, 1.0);

	float R = deformation.radius;
	mat4 C = deformation.screenQuadCorners;
	mat4 N = deformation.screenQuadVerticals;
	vec4 L = deformation.screenQuadCornerNorms;
	vec3 P = vec3(vs_Position.xy * deformation.offset.z + deformation.offset.xy, R);

	vec4 uvUV = vec4(vs_Position.xy, vec2(1.0) - vs_Position.xy);
	vec4 alpha = uvUV.zxzx * uvUV.wwyy;
	vec4 alphaPrime = alpha * L / dot(alpha, L);

	fs_TerrainPosition = vec3(deformation.radius * normalize(mat3(u_ModelMatrix) * P));
	fs_Height = 0.0;//planetTerrain(fs_TerrainPosition);

	float k = min(length(P) / dot(alpha, L) * 1.0000003, 1.0);
	float hPrime = (fs_Height + R * (1.0 - k)) / k;

#ifdef CUBE_PROJECTION
	gl_Position = u_ProjectionMatrix * u_ViewMatrix * u_ModelMatrix * vec4(P + vec3(0.0, 0.0, fs_Height), 1.0);
#else
	gl_Position = (C + hPrime * N) * alphaPrime;
#endif

	fs_Position = (deformation.radius + fs_Height) * normalize(mat3(u_ModelMatrix) * P);
	fs_SphereNormal = normalize(fs_TerrainPosition);
	//fs_Normal = getNormal(fs_TerrainPosition);


	//fs_Temperature = (172.0 + noise(fs_Position / 10000.0, 6, 0.0006, 0.9, -64.0, 64.0)) / 255.0;
	//fs_Humidity = (128.0 + noise(fs_Position / 10000.0, 6, 0.0008, 0.9, -128.0, 128.0)) / 255.0;


	float u_ZFar = pow(10.0, 10.0);

	float FCOEF = 2.0 / log2(u_ZFar + 1.0);
	fs_HALF_FCOEF = 0.5 * FCOEF;
	gl_Position.z = (log2(max(0.000001, 1.0 + gl_Position.w)) * FCOEF - 1.0) * gl_Position.w;
	fs_LogZ = 1.0 + gl_Position.w;
}
