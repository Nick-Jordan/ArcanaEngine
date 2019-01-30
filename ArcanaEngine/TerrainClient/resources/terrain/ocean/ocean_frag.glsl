#version 400

uniform float radius;
uniform mat4 cameraToOcean;
uniform mat4 screenToCamera;
uniform mat4 cameraToScreen;
uniform mat4 oceanToCamera;
uniform mat4 oceanToWorld;
uniform vec3 oceanCameraPos;
uniform vec3 oceanSunDir;
uniform vec3 horizon1;
uniform vec3 horizon2;

uniform float nbWaves; // number of waves
uniform sampler1D wavesSampler; // waves parameters (h, omega, kx, ky) in wind space
uniform float heightOffset; // so that surface height is centered around z = 0
uniform float seaRoughness; // total variance
uniform float time; // current time

// grid cell size in pixels, angle under which a grid cell is seen,
// and parameters of the geometric series used for wavelengths
uniform vec4 lods;

uniform vec3 u_CameraPosition;
uniform vec3 u_WorldSunDir;
uniform float u_Exposure;

float getHdrExposure()
 {
    return u_Exposure;
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

#define NYQUIST_MIN 0.5 // wavelengths below NYQUIST_MIN * sampling period are fully attenuated
#define NYQUIST_MAX 1.25 // wavelengths above NYQUIST_MAX * sampling period are not attenuated at all

uniform vec3 seaColor; // sea bottom color

const float PI = 3.141592657;
const float g = 9.81;

#include "resources/terrain/atmosphere/atmosphereShader.glsl"

in float oceanLod;
in vec2 oceanUv;
in vec3 oceanP;
in vec3 oceanDPdu;
in vec3 oceanDPdv;
in float oceanSigmaSq;

in vec3 test;

layout(location=0) out vec4 fs_FragColor;

// ---------------------------------------------------------------------------
// REFLECTED SUN RADIANCE
// ---------------------------------------------------------------------------

// assumes x>0
float erfc(float x) {
    return 2.0 * exp(-x * x) / (2.319 * x + sqrt(4.0 + 1.52 * x * x));
}

float Lambda(float cosTheta, float sigmaSq) {
    float v = cosTheta / sqrt((1.0 - cosTheta * cosTheta) * (2.0 * sigmaSq));
    return max(0.0, (exp(-v * v) - v * sqrt(PI) * erfc(v)) / (2.0 * v * sqrt(PI)));
    //return (exp(-v * v)) / (2.0 * v * sqrt(PI));
}

// L, V, N in world space
float reflectedSunRadiance(vec3 L, vec3 V, vec3 N, float sigmaSq) {
    vec3 H = normalize(L + V);
    vec3 Ty = normalize(cross(N, vec3(1.0, 0.0, 0.0)));
    vec3 Tx = cross(Ty, N);
    float zetax = dot(H, Tx) / dot(H, N);
    float zetay = dot(H, Ty) / dot(H, N);

    float zL = dot(L, N); // cos of source zenith angle
    float zV = dot(V, N); // cos of receiver zenith angle
    float zH = dot(H, N); // cos of facet normal zenith angle
    float zH2 = zH * zH;

    float p = exp(-0.5 * (zetax * zetax + zetay * zetay) / sigmaSq) / (2.0 * PI * sigmaSq);

    float fresnel = 0.02 + 0.98 * pow(1.0 - dot(V, H), 5.0);

    zL = max(zL,0.01);
    zV = max(zV,0.01);

    return fresnel * p / ((1.0 + Lambda(zL, sigmaSq) + Lambda(zV, sigmaSq)) * zV * zH2 * zH2 * 4.0);
}

// L, V, N in world space
float wardReflectedSunRadiance(vec3 L, vec3 V, vec3 N, float sigmaSq) {
    vec3 H = normalize(L + V);

    float hn = dot(H, N);
    float p = exp(-2.0 * ((1.0 - hn * hn) / sigmaSq) / (1.0 + hn)) / (4.0 * PI * sigmaSq);

    float c = 1.0 - dot(V, H);
    float c2 = c * c;
    float fresnel = 0.02 + 0.98 * c2 * c2 * c;

    float zL = dot(L, N);
    float zV = dot(V, N);
    zL = max(zL,0.01);
    zV = max(zV,0.01);

    // brdf times cos(thetaL)
    return zL <= 0.0 ? 0.0 : max(fresnel * p * sqrt(abs(zL / zV)), 0.0);
}

float meanFresnel(float cosThetaV, float sigmaV) {
    return pow(1.0 - cosThetaV, 5.0 * exp(-2.69 * sigmaV)) / (1.0 + 22.7 * pow(sigmaV, 1.5));
}

float meanFresnel(vec3 V, vec3 N, float sigmaSq) {
    return meanFresnel(dot(V, N), sqrt(sigmaSq));
}

vec2 U(vec2 zeta, vec3 V) {
    vec3 F = normalize(vec3(-zeta, 1.0));
    vec3 R = 2.0 * dot(F, V) * F - V;
    return -R.xy / (1.0 + R.z);
}

float refractedSeaRadiance(vec3 V, vec3 N, float sigmaSq) {
    return 0.98 * (1.0 - meanFresnel(V, N, sigmaSq));
}

void main() {
    vec3 WSD = u_WorldSunDir;
    vec3 WCP = u_CameraPosition;
    float lod = oceanLod;
    vec2 uv = oceanUv;
    vec3 dPdu = oceanDPdu;
    vec3 dPdv = oceanDPdv;
    float sigmaSq = oceanSigmaSq;

    float iMAX = min(ceil((log2(NYQUIST_MAX * lod) - lods.z) * lods.w), nbWaves - 1.0);
    float iMax = floor((log2(NYQUIST_MIN * lod) - lods.z) * lods.w);
    float iMin = max(0.0, floor((log2(NYQUIST_MIN * lod / lods.x) - lods.z) * lods.w));
    for (float i = iMin; i <= iMAX; i += 1.0) {
        vec4 wt = textureLod(wavesSampler, (i + 0.5) / nbWaves, 0.0);
        float phase = wt.y * time - dot(wt.zw, uv);
        float s = sin(phase);
        float c = cos(phase);
        float overk = g / (wt.y * wt.y);

        float wm = smoothstep(NYQUIST_MIN, NYQUIST_MAX, (2.0 * PI) * overk / lod);
        float wn = smoothstep(NYQUIST_MIN, NYQUIST_MAX, (2.0 * PI) * overk / lod * lods.x);

        vec3 factor = (1.0 - wm) * wn * wt.x * vec3(wt.zw * overk, 1.0);

        vec3 dPd = factor * vec3(c, c, -s);
        dPdu -= dPd * wt.z;
        dPdv -= dPd * wt.w;
        wt.zw *= overk;
        float kh = i < iMax ? wt.x / overk : 0.0;
        float wkh = (1.0 - wn) * kh;
        sigmaSq -= wt.z * wt.z * (sqrt(1.0 - wkh * wkh) - sqrt(1.0 - kh * kh));
    }

    vec3 earthCamera = vec3(0.0, 0.0, oceanCameraPos.z + radius);
    vec3 earthP = radius > 0.0 ? normalize(oceanP + vec3(0.0, 0.0, radius)) * (radius + 10.0) : oceanP;
    vec3 oceanCamera = vec3(0.0, 0.0, oceanCameraPos.z);
    vec3 V = normalize(oceanCamera - oceanP);
    vec3 N = normalize(cross(dPdu, dPdv));
    if (dot(V, N) < 0.0) {
        N = reflect(N, V); // reflects backfacing normals
    }

    vec3 sunL;
    vec3 skyE;
    vec3 extinction;
    sunRadianceAndSkyIrradiance(earthP, N, oceanSunDir, sunL, skyE);

    //vec3 Lsun = reflectedSunRadiance(oceanSunDir, V, N, sigmaSq) * sunL;
    //vec3 Lsky = reflectedSkyRadiance(V, N, sigmaSq, oceanSunDir);
    vec3 Lsun = wardReflectedSunRadiance(oceanSunDir, V, N, sigmaSq) * sunL;
    vec3 Lsky = meanFresnel(V, N, sigmaSq) * skyE / PI;
    vec3 Lsea = refractedSeaRadiance(V, N, sigmaSq) * seaColor * skyE / PI;

    vec3 surfaceColor = Lsun + Lsky + Lsea;

    // aerial perspective
    vec3 inscatter = inScattering(earthCamera, earthP, oceanSunDir, extinction, 0.0);
    vec3 finalColor = surfaceColor * extinction + inscatter;

    fs_FragColor.rgb = hdr(finalColor);//vec3(1.0, 0.0, 0.0);
    fs_FragColor.a = 1.0;
}