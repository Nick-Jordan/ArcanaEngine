#version 400

#extension GL_ARB_conservative_depth : enable 
layout(depth_greater) out float gl_FragDepth;

layout(location = 0) out vec4 fs_FragColor;
layout(location = 1) out vec4 fs_EmissiveColor;

in vec3 fs_Position;
in vec2 fs_TexCoord;
in float fs_Height;
in vec3 fs_TerrainPosition;
in vec3 fs_SphereNormal;
in vec3 fs_Normal;
in float fs_Temperature;
in float fs_Humidity;

in float fs_HALF_FCOEF;
in float fs_LogZ;

#include "resources/test4.txt"
#include "resources/terrain/atmosphere/atmosphereShader.glsl"

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

uniform sampler2D u_TerrainSurface;
uniform sampler2D u_TerrainColor;
uniform vec3 u_CameraPosition;
uniform vec3 u_WorldSunDir;

vec3 getSeaColor() 
{
	return vec3(0.00392, 0.01568, 0.04705);
}

float getSeaRoughness() 
{
	return 0.02;
}

#include "resources/terrain/oceanBRDF.glsl"

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
	//vec3 normal = fs_Normal;
	//normal.z = sqrt(max(0.0, 1.0 - dot(normal.xy, normal.xy)));
	//normal = deformation.tangentFrameToWorld * normal;

	float height = planetTerrain(fs_TerrainPosition);
  
  	vec3 color = vec3(0.0);
  	if(height <= 0.0)
  	{
  		color = vec3(0.0);//vec3(0.0, 0.467, 0.745);
  	}
  	else
  	{
  		float temp = clamp(mix(fs_Temperature, fs_Temperature / 2, height / 15000.0), fs_Temperature / 2, fs_Temperature);
  		temp = clamp(mix(temp * 1.4, temp * 0.3, abs(fs_Position.y / deformation.radius) + noise(fs_Position, 3, 0.00001, 0.7, -0.2, 0.2)), temp * 0.3, temp * 1.1);
  		float humid = clamp(mix(fs_Humidity * 1.4, fs_Humidity, abs(fs_Position.y / deformation.radius)), fs_Humidity, fs_Humidity * 1.4);
  		color = texture(u_TerrainSurface, fs_TexCoord).rgb * texture(u_TerrainColor, vec2(temp, humid)).rgb;
  	}

	vec3 V = normalize(fs_Position);
	vec3 P = V * max(length(fs_Position), deformation.radius + 10.0);
	vec3 v = normalize(P - u_CameraPosition);

	float cTheta = 0.0;
	float vSun = dot(V, u_WorldSunDir);

	vec3 sunL;
	vec3 skyE;
	sunRadianceAndSkyIrradiance(P, fs_SphereNormal, u_WorldSunDir, sunL, skyE);

	vec3 groundColor = color.rgb * (sunL * max(cTheta, 0.0) + skyE) / PI;

	if (height <= 0.0)
	{
		groundColor = oceanRadiance(-v, V, u_WorldSunDir, getSeaRoughness(), sunL, skyE);
	}

	vec3 extinction;
	vec3 inscatter = inScattering(u_CameraPosition, P, u_WorldSunDir, extinction, 0.0);

	vec3 finalColor = groundColor * extinction + inscatter;
	fs_FragColor = vec4(finalColor, 1.0);
	
	fs_EmissiveColor = vec4(0.0);

	#ifdef DEBUG_QUADTREE
    fs_FragColor.r += mod(dot(floor(deformation.offset.xy / deformation.offset.z + 0.5), vec2(1.0)), 2.0);
    #endif

    gl_FragDepth = log2(fs_LogZ) * fs_HALF_FCOEF;

}