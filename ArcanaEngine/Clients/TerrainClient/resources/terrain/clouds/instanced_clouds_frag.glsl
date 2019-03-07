#version 400

#extension GL_ARB_conservative_depth : enable 
layout(depth_greater) out float gl_FragDepth;

layout(location = 0) out vec4 fs_FragColor;
layout(location = 1) out vec4 fs_EmissiveColor;

in vec3 fs_Position;
in vec3 fs_Normal;
in float fs_HALF_FCOEF;
in float fs_LogZ;

#include "resources/test4.txt"
#include "resources/terrain/atmosphere/atmosphereShader.glsl"
#include "resources/rotate.glsl"
#include "resources/fbm.glsl"

const vec3 cloudColor = vec3(1.0);

uniform vec3 u_CameraPosition;
const float density = 0.5;

uniform vec3 u_WorldSunDir;
uniform float u_Time;

const vec3 scale = vec3(1.0);
const int cloudOctaves = 6;

uniform struct {
    vec2 blending;
    float radius;
} deformation;

vec3 Cell3NoiseVec(vec3 p)
{
    return vec3(cellular(p), snoise(p));
}

float swirlClouds(vec3 point)
{
    float cycloneFreq = 0.5;
    float tidalLock = 0.0;
    float twistMagn = 0.2;
    float twistZones = 0.5;
    float cloudsCoverage = 0.3;
    float cycloneSqrtDensity = sqrt(0.5);
    float cycloneMagn = 0.5;
    float cloudsFreq = 0.2;

	float zone = cos(point.y * twistZones);
    float ang = zone * twistMagn;
    vec3  twistedPoint = point;
    float coverage = cloudsCoverage;
    float weight = 1.0;

     vec3 Randomize = vec3(u_Time / 1000.0);

    // Compute cyclons
    vec3  cycloneCenter = (tidalLock > 0.0) ? vec3(0.0, 1.0, 0.0) : Cell3NoiseVec((point + Randomize) * cycloneFreq).xyz;
    float cycloneRadius = length(cycloneCenter - point) / cycloneSqrtDensity;
    float cycloneAmpl   = -tidalLock * cycloneMagn * sign(cycloneCenter.y);
    if (cycloneRadius < 1.0)
    {
        float dist = 1.0 - cycloneRadius;
        float fi = mix(log(cycloneRadius), pow(dist, 3.0), cycloneRadius);
        twistedPoint = rotate(cycloneAmpl*fi, cycloneCenter, point);
        weight = clamp(1.0 - cycloneRadius / 0.05, 0.0, 1.0);
        weight = (1.0 - weight * weight) * (1.0 + dist);
        coverage = mix(coverage, 1.0, dist);
    }

    // Compute the Coriolis effect
    float sina = sin(ang);
    float cosa = cos(ang);
    twistedPoint = vec3(cosa*twistedPoint.x-sina*twistedPoint.z, twistedPoint.y, sina*twistedPoint.x+cosa*twistedPoint.z);
    twistedPoint = twistedPoint * cloudsFreq + Randomize;

/*
    // Compute flow-like distortion
    noiseLacunarity = 2.0;
    noiseOctaves = 5;
    vec3 distort = Fbm3D(twistedPoint * 0.5) * 1.5;
    noiseOctaves = 6;
    float global = (Fbm(twistedPoint + distort) + cloudsCoverage);
    global *= weight;
/**/

    // Compute flow-like distortion
    vec3 p = twistedPoint * cloudsFreq * 6.37;
    vec3 q = p + fbm3D2(p, cloudOctaves);
    vec3 r = p + fbm3D2(q, cloudOctaves);
    float f = fbm2(r, cloudOctaves) * 0.7 + coverage - 0.3;
    float global = clamp(f, 0.0, 1.0) * weight;
/**/

    // Compute turbilence features
    float turbulence = fbm(point * 50.0 * cloudsFreq + Randomize, cloudOctaves) * 0.1;

    return global;// + turbulence * step(0.1, global);
}

float basicClouds()
{
	return 0.5;
}

#define SWIRL

void main (void)
{	
	float height = 0.0;
	
    vec3 position = fs_Position / deformation.radius;

	#ifdef BASIC
		height = basicClouds();
	#endif

	#ifdef SWIRL
		height = swirlClouds(position);
	#endif
	
    vec3 V = normalize(fs_Position);
    vec3 P = V * max(length(fs_Position), deformation.radius + 10.0);
    vec3 v = normalize(P - u_CameraPosition);

    vec3 sunL;
    vec3 skyE;
    sunRadianceAndSkyIrradiance(P, fs_Normal, u_WorldSunDir, sunL, skyE);

    vec3 color = 1.5 * cloudColor.rgb * skyE / M_PI;

    vec3 extinction;
    vec3 inscatter = inScattering(u_CameraPosition, P, u_WorldSunDir, extinction, 0.0);

    vec3 finalColor = color * extinction + inscatter;

	fs_FragColor.rgb = finalColor;
    fs_FragColor.a = height;

    fs_EmissiveColor = vec4(0.0);

	gl_FragDepth = log2(fs_LogZ) * fs_HALF_FCOEF;
}
