#ifndef NOISE_H_
#define NOISE_H_

#include <cmath>

namespace Arcana
{

	inline long lrandom(long *seed)
	{
		*seed = (*seed * 1103515245 + 12345) & 0x7FFFFFFF;
		return *seed;
	}

	inline float frandom(long *seed)
	{
		long r = lrandom(seed) >> (31 - 24);
		return r / (float)(1 << 24);
	}

	inline float grandom(float mean, float stdDeviation, long *seed)
	{
		float x1, x2, w, y1;
		static float y2;
		static int use_last = 0;

		if (use_last) {
			y1 = y2;
			use_last = 0;
		}
		else {
			do {
				x1 = 2.0f * frandom(seed) - 1.0f;
				x2 = 2.0f * frandom(seed) - 1.0f;
				w = x1 * x1 + x2 * x2;
			} while (w >= 1.0f);
			w = sqrt((-2.0f * log(w)) / w);
			y1 = x1 * w;
			y2 = x2 * w;
			use_last = 1;
		}
		return mean + y1 * stdDeviation;
	}

	float cnoise(float x, float y, int P = 0);

	float cnoise(float x, float y, float z, int P = 0);

	float snoise(float x, float y, int P = 0);

	float snoise(float x, float y, float z, int P = 0);

	float snoise(float x, float y, float z, float w, int P = 0);

	float *buildFbm4NoiseTexture2D(int size, int freq, int octaves, int lacunarity, float gain);

	float *buildFbm1NoiseTexture3D(int size, int freq, int octaves, int lacunarity, float gain);

}

#endif