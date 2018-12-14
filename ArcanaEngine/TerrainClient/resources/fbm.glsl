float   noiseLacunarity  = 2.218281828459;
float   noiseH           = 0.5;
float   noiseOffset      = 0.8;
float   noiseRidgeSmooth = 0.0001;

float   fbm(vec3 point, int octaves)
{
    float summ = 0.0;
	float ampl = 1.0;
	float gain = pow(noiseLacunarity, -noiseH);
    for (int i=0; i<octaves; ++i)
    {
        summ  += snoise(point) * ampl;
		ampl  *= gain;
        point *= noiseLacunarity;
    }
    return summ;
}

float fbm2(vec3 point, int octaves)
{
    float summ = 0.0;
	float ampl = 1.0;
    for (int i=0; i < octaves; ++i)
    {
        summ  += snoise(point) * ampl;
		ampl  *= 0.333;
        point *= 3.1416;
    }
    return summ;
}

const vec3 vyd = vec3(3.33, 5.71, 1.96);
const vec3 vzd = vec3(7.77, 2.65, 4.37);
const vec3 vwd = vec3(1.13, 2.73, 6.37);

vec3 fbm3D2(vec3 point, int octaves)
{
    vec3  summ = vec3(0.0);
	float ampl = 1.0;
    for (int i=0; i < octaves; ++i)
    {
        summ  += vec3(snoise(point), snoise(point + vyd), snoise(point + vzd)) * ampl;
		ampl  *= 0.333;
        point *= 3.1416;
    }
    return summ;
}