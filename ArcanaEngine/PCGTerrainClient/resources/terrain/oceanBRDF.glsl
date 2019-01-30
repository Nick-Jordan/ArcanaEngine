#define PI 3.141592657

float meanFresnel(float cosThetaV, float sigmaV) {
	return pow(1.0 - cosThetaV, 5.0 * exp(-2.69 * sigmaV)) / (1.0 + 22.7 * pow(sigmaV, 1.5));
}

float meanFresnel(vec3 V, vec3 N, float sigmaSq) {
	return meanFresnel(dot(V, N), sqrt(sigmaSq));
}

float reflectedSunRadiance(vec3 V, vec3 N, vec3 L, float sigmaSq) {
	vec3 H = normalize(L + V);

	float hn = dot(H, N);
	float p = exp(-2.0 * ((1.0 - hn * hn) / sigmaSq) / (1.0 + hn)) / (4.0 * PI * sigmaSq);

	float c = 1.0 - dot(V, H);
	float c2 = c * c;
	float fresnel = 0.02 + 0.98 * c2 * c2 * c;

	float zL = dot(L, N);
	float zV = dot(V, N);
	zL = max(zL, 0.01);
	zV = max(zV, 0.01);

	// brdf times cos(thetaL)
	return zL <= 0.0 ? 0.0 : max(fresnel * p * sqrt(abs(zL / zV)), 0.0);
}

vec3 oceanRadiance(vec3 V, vec3 N, vec3 L, float seaRoughness, vec3 sunL, vec3 skyE) {
	float F = meanFresnel(V, N, seaRoughness);
	vec3 Lsun = reflectedSunRadiance(V, N, L, seaRoughness) * sunL;
	vec3 Lsky = skyE * F / PI;
	vec3 Lsea = (1.0 - F) * getSeaColor() * skyE / PI;
	return Lsun + Lsky + Lsea;
}