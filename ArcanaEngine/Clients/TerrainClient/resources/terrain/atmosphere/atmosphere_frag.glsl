#version 400

#include "resources/terrain/atmosphere/atmosphereShader.glsl"

uniform vec3 u_WorldSunDir;

uniform vec3 origin;
uniform vec3 u_CameraPosition;

in vec3 fs_Direction;
in vec3 fs_RelativeDirection;
in vec2 test;

layout(location = 0) out vec4 fs_FragColor;
layout(location = 1) out vec4 fs_EmissiveColor;

void main() {
	vec3 WSD = u_WorldSunDir;
	vec3 WCP = u_CameraPosition;

	vec3 d = normalize(fs_Direction);

	vec3 sunColor = outerSunRadiance(fs_RelativeDirection);

	vec3 extinction;
	vec3 inscatter = skyRadiance(WCP + origin, d, WSD, extinction, 0.0);

	vec3 finalColor = sunColor * extinction + inscatter;

	fs_FragColor = vec4(finalColor, 1.0);
	fs_EmissiveColor = vec4(0.0);
}
