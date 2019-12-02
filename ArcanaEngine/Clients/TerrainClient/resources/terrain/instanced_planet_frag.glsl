#version 400

#extension GL_ARB_conservative_depth : enable 
layout(depth_greater) out float gl_FragDepth;

layout(location = 0) out vec4 fs_FragColor;
layout(location = 1) out vec4 fs_EmissiveColor;

in vec3 fs_Position;
in vec2 fs_TexCoord;
in vec4 fs_Offset;
in vec3 fs_TerrainPosition;
in vec3 fs_SphereNormal;
in float fs_Temperature;
in float fs_Humidity;

in float fs_HALF_FCOEF;
in float fs_LogZ;

#include "resources/terrain/atmosphere/atmosphereShader.glsl"

uniform struct {
	vec2 blending;
	float radius;
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

void main()
{
	float height = 0.0;
  
  	vec3 color = vec3(0.0);
  	if(height <= 0.0)
  	{
  		color = vec3(0.0);//vec3(0.0, 0.467, 0.745);
  	}
  	else
  	{
  		//float temp = clamp(mix(fs_Temperature, fs_Temperature / 2, height / 15000.0), fs_Temperature / 2, fs_Temperature);
  		//temp = clamp(mix(temp * 1.4, temp * 0.3, abs(fs_Position.y / deformation.radius) + noise(fs_Position, 3, 0.00001, 0.7, -0.2, 0.2)), temp * 0.3, temp * 1.1);
  		//float humid = clamp(mix(fs_Humidity * 1.4, fs_Humidity, abs(fs_Position.y / deformation.radius)), fs_Humidity, fs_Humidity * 1.4);
  		//color = texture(u_TerrainSurface, fs_TexCoord).rgb * texture(u_TerrainColor, vec2(temp, humid)).rgb;
  	}

	vec3 V = normalize(fs_Position);
	vec3 P = V * max(length(fs_Position), deformation.radius + 10.0);
	vec3 v = normalize(P - u_CameraPosition);

	float cTheta = 0.0;
	float vSun = dot(V, u_WorldSunDir);

	vec3 sunL;
	vec3 skyE;
	sunRadianceAndSkyIrradiance(P, fs_SphereNormal, u_WorldSunDir, sunL, skyE);

	vec3 groundColor = 1.5 * color.rgb * (sunL * max(cTheta, 0.0) + skyE) / PI;

	if (height <= 0.0)
	{
		groundColor = oceanRadiance(-v, V, u_WorldSunDir, getSeaRoughness(), sunL, skyE);
	}

	vec3 extinction;
	vec3 inscatter = inScattering(u_CameraPosition, P, u_WorldSunDir, extinction, 0.0);

	vec3 finalColor = groundColor * extinction + inscatter;
	finalColor = finalColor / 4.0;
	fs_FragColor = vec4(finalColor, 1.0);
	
	fs_EmissiveColor = vec4(0.0);

	#ifdef DEBUG_QUADTREE
    fs_FragColor.r += mod(dot(floor(fs_Offset.xy / fs_Offset.z + 0.5), vec2(1.0)), 2.0);
    #endif
	
    gl_FragDepth = log2(fs_LogZ) * fs_HALF_FCOEF;

}