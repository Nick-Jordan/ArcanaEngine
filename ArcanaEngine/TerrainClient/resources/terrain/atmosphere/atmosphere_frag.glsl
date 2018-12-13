#version 400

#include "resources/terrain/atmosphere/atmosphereShader.glsl"

uniform vec3 u_WorldSunDir;
uniform float u_Exposure;

float getHdrExposure() 
{
	return u_Exposure;
}

vec3 hdr(vec3 L) {
#ifndef NOHDR
	L = L * getHdrExposure();
	L.r = L.r < 1.413 ? pow(L.r * 0.38317, 1.0 / 2.2) : 1.0 - exp(-L.r);
	L.g = L.g < 1.413 ? pow(L.g * 0.38317, 1.0 / 2.2) : 1.0 - exp(-L.g);
	L.b = L.b < 1.413 ? pow(L.b * 0.38317, 1.0 / 2.2) : 1.0 - exp(-L.b);
#endif
	return L;
}

uniform vec3 origin;
uniform vec3 u_CameraPosition;

in vec3 fs_Direction;
in vec3 fs_RelativeDirection;
in vec2 test;

layout(location = 0) out vec4 fs_FragColor;

void main() {
	vec3 WSD = u_WorldSunDir;
	vec3 WCP = u_CameraPosition;

	vec3 d = normalize(fs_Direction);

	vec3 sunColor = outerSunRadiance(fs_RelativeDirection);

	vec3 extinction;
	vec3 inscatter = skyRadiance(WCP + origin, d, WSD, extinction, 0.0);

	vec3 finalColor = sunColor * extinction + inscatter;

	fs_FragColor.rgb = hdr(finalColor);
	fs_FragColor.a = 1.0;
	//data = vec4(sunColor, 1.0);
}
