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

#define NYQUIST_MIN 0.5 // wavelengths below NYQUIST_MIN * sampling period are fully attenuated
#define NYQUIST_MAX 1.25 // wavelengths above NYQUIST_MAX * sampling period are not attenuated at all

uniform vec3 seaColor; // sea bottom color

const float PI = 3.141592657;
const float g = 9.81;

layout(location=0) in vec3 vertex;

out float oceanLod;
out vec2 oceanUv; // coordinates in wind space used to compute P(u)
out vec3 oceanP; // wave point P(u) in ocean space
out vec3 oceanDPdu; // dPdu in wind space, used to compute N
out vec3 oceanDPdv; // dPdv in wind space, used to compute N
out float oceanSigmaSq; // variance of unresolved waves in wind space

out vec3 test;

vec2 oceanPos(vec3 vertex, out float t, out vec3 cameraDir, out vec3 oceanDir) {
    float horizon = horizon1.x + horizon1.y * vertex.x - sqrt(horizon2.x + (horizon2.y + horizon2.z * vertex.x) * vertex.x);
    cameraDir = normalize((screenToCamera * vec4(vertex.x, min(vertex.y, horizon), 0.0, 1.0)).xyz);
    oceanDir = (cameraToOcean * vec4(cameraDir, 0.0)).xyz;
    float cz = oceanCameraPos.z;
    float dz = oceanDir.z;
    if (radius == 0.0) {
        t = (heightOffset + 5.0 - cz) / dz;
    } else {
        float b = dz * (cz + radius);
        float c = cz * (cz + 2.0 * radius);
        float tSphere = - b - sqrt(max(b * b - c, 0.0));
        float tApprox = - cz / dz * (1.0 + cz / (2.0 * radius) * (1.0 - dz * dz));
        t = abs((tApprox - tSphere) * dz) < 1.0 ? tApprox : tSphere;
    }
    return oceanCameraPos.xy + t * oceanDir.xy;
}

vec2 oceanPos(vec3 vertex) {
    float t;
    vec3 cameraDir;
    vec3 oceanDir;
    return oceanPos(vertex, t, cameraDir, oceanDir);
}

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void main() {
    float t;
    vec3 cameraDir;
    vec3 oceanDir;
    vec2 uv = oceanPos(vertex, t, cameraDir, oceanDir);
    float lod = - t / oceanDir.z * lods.y; // size in meters of one grid cell, projected on the sea surface
    vec2 duv = oceanPos(vertex + vec3(0.0, 0.01, 0.0)) - uv;
    vec3 dP = vec3(0.0, 0.0, heightOffset + (radius > 0.0 ? 0.0 : 5.0));
    vec3 dPdu = vec3(1.0, 0.0, 0.0);
    vec3 dPdv = vec3(0.0, 1.0, 0.0);
    float sigmaSq = seaRoughness;
    if (duv.x != 0.0 || duv.y != 0.0) {
        float iMin = max(floor((log2(NYQUIST_MIN * lod) - lods.z) * lods.w), 0.0);
        for (float i = iMin; i < nbWaves; ++i) {
            vec4 wt = textureLod(wavesSampler, (i + 0.5) / nbWaves, 0.0);
            float phase = wt.y * time - dot(wt.zw, uv);
            float s = sin(phase);
            float c = cos(phase);
            float overk = g / (wt.y * wt.y);

            float wm = smoothstep(NYQUIST_MIN, NYQUIST_MAX, (2.0 * PI) * overk / lod);
            vec3 factor = wm * wt.x * vec3(wt.zw * overk, 1.0);

            dP += factor * vec3(s, s, c);

            vec3 dPd = factor * vec3(c, c, -s);
            dPdu -= dPd * wt.z;
            dPdv -= dPd * wt.w;

            wt.zw *= overk;
            float kh = wt.x / overk;
            sigmaSq -= wt.z * wt.z * (1.0 - sqrt(1.0 - kh * kh));
        }
    }
    vec3 p = t * oceanDir + dP + vec3(0.0, 0.0, oceanCameraPos.z);
    if (radius > 0.0) {
        dPdu += vec3(0.0, 0.0, -p.x / (radius + p.z));
        dPdv += vec3(0.0, 0.0, -p.y / (radius + p.z));
    }

    test = dP;
    gl_Position = cameraToScreen * vec4(t * cameraDir + mat3(oceanToCamera) * dP, 1.0);
    oceanLod = lod;
    oceanUv = uv;
    oceanP = p;
    oceanDPdu = dPdu;
    oceanDPdv = dPdv;
    oceanSigmaSq = sigmaSq;
}