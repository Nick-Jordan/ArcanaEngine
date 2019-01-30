#version 400

#extension GL_ARB_conservative_depth : enable 
layout(depth_greater) out float gl_FragDepth;

layout(location = 0) out vec4 fs_FragColor;

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
uniform float u_Exposure;

float getHdrExposure()
 {
	return u_Exposure;
}

vec3 getSeaColor() 
{
	return vec3(0.00392, 0.01568, 0.04705);
}

float getSeaRoughness() 
{
	return 0.02;
}

vec3 hdr(vec3 L)
 {
#ifndef NOHDR
	L = L * getHdrExposure();
	L.r = L.r < 1.413 ? pow(L.r * 0.38317, 1.0 / 2.2) : 1.0 - exp(-L.r);
	L.g = L.g < 1.413 ? pow(L.g * 0.38317, 1.0 / 2.2) : 1.0 - exp(-L.g);
	L.b = L.b < 1.413 ? pow(L.b * 0.38317, 1.0 / 2.2) : 1.0 - exp(-L.b);
#endif
	return L;
}

#include "resources/test4.txt"

#include "resources/terrain/oceanBRDF.glsl"

uniform sampler2D u_TerrainHeight;

void main()
{
	//vec3 normal = fs_Normal;
	//normal.z = sqrt(max(0.0, 1.0 - dot(normal.xy, normal.xy)));
	//normal = deformation.tangentFrameToWorld * normal;

	float height = texture(u_TerrainHeight, fs_TexCoord).r;
  
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
  		color = texture(u_TerrainColor, vec2(temp, humid)).rgb;
  	}

	vec3 V = normalize(fs_Position);
	vec3 P = V * max(length(fs_Position), deformation.radius + 10.0);
	vec3 v = normalize(P - u_CameraPosition);

	float cTheta = 0.0;
	float vSun = dot(V, u_WorldSunDir);

	vec3 sunL;
	vec3 skyE;
	sunRadianceAndSkyIrradiance(P, fs_SphereNormal, u_WorldSunDir, sunL, skyE);

	vec3 groundColor = 4.0 * color.rgb * (sunL * max(cTheta, 0.0) + skyE) / PI;

	if (height <= 0.0)
	{
		groundColor = oceanRadiance(-v, V, u_WorldSunDir, getSeaRoughness(), sunL, skyE);
	}

	vec3 extinction;
	vec3 inscatter = inScattering(u_CameraPosition, P, u_WorldSunDir, extinction, 0.0);

	vec3 finalColor = groundColor * extinction + inscatter;
	fs_FragColor.rgb = hdr(finalColor);
	fs_FragColor.a = 1.0;

	#ifdef DEBUG_QUADTREE
    fs_FragColor.r += mod(dot(floor(deformation.offset.xy / deformation.offset.z + 0.5), vec2(1.0)), 2.0);
    #endif

    gl_FragDepth = log2(fs_LogZ) * fs_HALF_FCOEF;
}