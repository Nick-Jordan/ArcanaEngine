#version 400

layout(location = 0) in vec3 vs_Position;

out vec3 fs_Position;
out vec2 fs_TexCoord;

out float h;

out float fs_HALF_FCOEF;
out float fs_LogZ;

uniform mat4 u_ModelMatrix;

uniform mat4 gridMatrix;
uniform mat4 quadMVP;

uniform struct {
	vec4 offset;
	vec4 camera;
	vec2 blending;
	float radius;
} deformation;

#include "resources/test4.txt"

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

void main()
{
	fs_TexCoord = vs_Position.xy;

	fs_Position = (gridMatrix * vec4(vs_Position, 1.0)).xyz;

	h = planetTerrain(fs_Position);

	gl_Position = quadMVP * vec4(vs_Position, 1.0);

	//gl_Position.y += h;

	//fs_Position = (deformation.radius + h) * normalize(mat3(u_ModelMatrix) * FinalPosition);
	//fs_Normal = getNormal(fs_TerrainPosition);

	float u_ZFar = pow(10.0, 10.0);

	float FCOEF = 2.0 / log2(u_ZFar + 1.0);
	fs_HALF_FCOEF = 0.5 * FCOEF;
	gl_Position.z = (log2(max(0.000001, 1.0 + gl_Position.w)) * FCOEF - 1.0) * gl_Position.w;
	fs_LogZ = 1.0 + gl_Position.w;
}
