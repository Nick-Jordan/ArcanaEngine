#version 400

layout(location = 0) in vec3 vs_Position;

out vec4 fs_Position;
out vec2 fs_TexCoord;
out float fs_Height;
out vec3 fs_TerrainPosition;
out vec3 fs_Normal;

out float fs_HALF_FCOEF;
out float fs_LogZ;

uniform mat4 u_ProjectionMatrix;
uniform mat4 u_ViewMatrix;
//uniform mat4 u_ModelMatrix;

//uniform float u_ZFar;

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

vec3 getNormal(vec3 terrainPosition)
{
	vec3 off = vec3(1.0, 1.0, 0.0);
	float hL = planetTerrain(vec3(terrainPosition.xy - off.xz, terrainPosition.z));
	float hR = planetTerrain(vec3(terrainPosition.xy + off.xz, terrainPosition.z));
	float hD = planetTerrain(vec3(terrainPosition.xy - off.zy, terrainPosition.z));
	float hU = planetTerrain(vec3(terrainPosition.xy + off.zy, terrainPosition.z));

	vec3 N;

	N.x = hL - hR;
	N.y = hD - hU;
	N.z = 2.0;
	N = normalize(N);

	return N;
}

void main()
{
	fs_TexCoord = vs_Position.xy;

	vec2 v = abs(deformation.camera.xy - vs_Position.xy);
	float d = max(max(v.x, v.y), deformation.camera.z);
	float blend = clamp((d - deformation.blending.x) / deformation.blending.y, 0.0, 1.0);

	vec2 xyPosition = vs_Position.xy * deformation.offset.z + deformation.offset.xy;
	fs_TerrainPosition = vec3(xyPosition, 0.0);
	//fs_Normal = getNormal(fs_TerrainPosition);
	fs_Height = planetTerrain(fs_TerrainPosition);
	fs_Position = vec4(xyPosition, fs_Height, 1.0);

	gl_Position = u_ProjectionMatrix * u_ViewMatrix * fs_Position;


	float u_ZFar = pow(10.0, 10.0);

	float FCOEF = 2.0 / log2(u_ZFar + 1.0);
	fs_HALF_FCOEF = 0.5 * FCOEF;
	gl_Position.z = (log2(max(0.000001, 1.0 + gl_Position.w)) * FCOEF - 1.0) * gl_Position.w;
	fs_LogZ = 1.0 + gl_Position.w;
}
