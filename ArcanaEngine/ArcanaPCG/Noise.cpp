#include "Noise.h"

namespace Arcana
{
	///////////////////////////REMOVE THESE///////////////////////
	//floor
	Vector3d floor(const Vector3d& point)
	{
		return Vector3d(::floor(point.x), ::floor(point.y), ::floor(point.z));
	}
	//mod
	Vector3d mod(const Vector3d& point, double m)
	{
		return Vector3d(::fmod(point.x, m), ::fmod(point.y, m), ::fmod(point.z, m));
	}
	//fract
	Vector3d fract(const Vector3d& point)
	{
		double i;
		return Vector3d(Math::fract(point.x, &i), Math::fract(point.y, &i), Math::fract(point.z, &i));
	}
	//min
	Vector3d min(const Vector3d& a, const Vector3d& b)
	{
		return Vector3d(Math::min(a.x, b.x), Math::min(a.y, b.y), Math::min(a.z, b.z));
	}
	//min2
	Vector2d min(const Vector2d& a, const Vector2d& b)
	{
		return Vector2d(Math::min(a.x, b.x), Math::min(a.y, b.y));
	}
	//max
	Vector3d max(const Vector3d& a, const Vector3d& b)
	{
		return Vector3d(Math::max(a.x, b.x), Math::max(a.y, b.y), Math::max(a.z, b.z));
	}
	//sqrt
	Vector3d sqrt(const Vector3d& point)
	{
		return Vector3d(::sqrt(point.x), ::sqrt(point.y), ::sqrt(point.z));
	}
	Vector2d sqrt(const Vector2d& point)
	{
		return Vector2d(::sqrt(point.x), ::sqrt(point.y));
	}
	//fast floor
	int32 fastFloor(const double x)
	{
		return x > 0 ? (int32)x : (int32)x - 1;
	}
	//dots
	double dot(const double* g, const double x, const double y) { return g[0] * x + g[1] * y; }
	double dot(const double* g, const double x, const double y, const double z) { return g[0] * x + g[1] * y + g[2] * z; }
	double dot(const double* g, const double x, const double y, const double z, const double w) { return g[0] * x + g[1] * y + g[2] * z + g[3] * w; }
	///////////////////////////REMOVE THESE///////////////////////

	Vector2d Noise::cellular(const Vector3d& point)
	{
#define K 0.142857142857 // 1/7
#define Ko 0.428571428571 // 1/2-K/2
#define K2 0.020408163265306 // 1/(7*7)
#define Kz 0.166666666667 // 1/6
#define Kzo 0.416666666667 // 1/2-1/6*2
#define jitter 1.0

		Vector3d Pi = mod(floor(point), 289.0);
		Vector3d Pf = fract(point) - Vector3d(0.5);

		Vector3d Pfx = Vector3d(Pf.x) + Vector3d(1.0, 0.0, -1.0);
		Vector3d Pfy = Vector3d(Pf.y) + Vector3d(1.0, 0.0, -1.0);
		Vector3d Pfz = Vector3d(Pf.z) + Vector3d(1.0, 0.0, -1.0);

		Vector3d p = permute(Vector3d(Pi.x) + Vector3d(-1.0, 0.0, 1.0));
		Vector3d p1 = permute(p + Pi.y - 1.0);
		Vector3d p2 = permute(p + Pi.y);
		Vector3d p3 = permute(p + Pi.y + 1.0);

		Vector3d p11 = permute(p1 + Pi.z - 1.0);
		Vector3d p12 = permute(p1 + Pi.z);
		Vector3d p13 = permute(p1 + Pi.z + 1.0);

		Vector3d p21 = permute(p2 + Pi.z - 1.0);
		Vector3d p22 = permute(p2 + Pi.z);
		Vector3d p23 = permute(p2 + Pi.z + 1.0);

		Vector3d p31 = permute(p3 + Pi.z - 1.0);
		Vector3d p32 = permute(p3 + Pi.z);
		Vector3d p33 = permute(p3 + Pi.z + 1.0);

		Vector3d ox11 = fract(p11*K) - Ko;
		Vector3d oy11 = mod(floor(p11*K), 7.0)*K - Ko;
		Vector3d oz11 = floor(p11*K2)*Kz - Kzo;

		Vector3d ox12 = fract(p12*K) - Ko;
		Vector3d oy12 = mod(floor(p12*K), 7.0)*K - Ko;
		Vector3d oz12 = floor(p12*K2)*Kz - Kzo;

		Vector3d ox13 = fract(p13*K) - Ko;
		Vector3d oy13 = mod(floor(p13*K), 7.0)*K - Ko;
		Vector3d oz13 = floor(p13*K2)*Kz - Kzo;

		Vector3d ox21 = fract(p21*K) - Ko;
		Vector3d oy21 = mod(floor(p21*K), 7.0)*K - Ko;
		Vector3d oz21 = floor(p21*K2)*Kz - Kzo;

		Vector3d ox22 = fract(p22*K) - Ko;
		Vector3d oy22 = mod(floor(p22*K), 7.0)*K - Ko;
		Vector3d oz22 = floor(p22*K2)*Kz - Kzo;

		Vector3d ox23 = fract(p23*K) - Ko;
		Vector3d oy23 = mod(floor(p23*K), 7.0)*K - Ko;
		Vector3d oz23 = floor(p23*K2)*Kz - Kzo;

		Vector3d ox31 = fract(p31*K) - Ko;
		Vector3d oy31 = mod(floor(p31*K), 7.0)*K - Ko;
		Vector3d oz31 = floor(p31*K2)*Kz - Kzo;

		Vector3d ox32 = fract(p32*K) - Ko;
		Vector3d oy32 = mod(floor(p32*K), 7.0)*K - Ko;
		Vector3d oz32 = floor(p32*K2)*Kz - Kzo;

		Vector3d ox33 = fract(p33*K) - Ko;
		Vector3d oy33 = mod(floor(p33*K), 7.0)*K - Ko;
		Vector3d oz33 = floor(p33*K2)*Kz - Kzo;

		Vector3d dx11 = Pfx + jitter * ox11;
		Vector3d dy11 = Vector3d(Pfy.x) + jitter * oy11;
		Vector3d dz11 = Vector3d(Pfz.x) + jitter * oz11;

		Vector3d dx12 = Pfx + jitter * ox12;
		Vector3d dy12 = Vector3d(Pfy.x) + jitter * oy12;
		Vector3d dz12 = Vector3d(Pfz.y) + jitter * oz12;

		Vector3d dx13 = Pfx + jitter * ox13;
		Vector3d dy13 = Vector3d(Pfy.x) + jitter * oy13;
		Vector3d dz13 = Vector3d(Pfz.z) + jitter * oz13;

		Vector3d dx21 = Pfx + jitter * ox21;
		Vector3d dy21 = Vector3d(Pfy.y) + jitter * oy21;
		Vector3d dz21 = Vector3d(Pfz.x) + jitter * oz21;

		Vector3d dx22 = Pfx + jitter * ox22;
		Vector3d dy22 = Vector3d(Pfy.y) + jitter * oy22;
		Vector3d dz22 = Vector3d(Pfz.y) + jitter * oz22;

		Vector3d dx23 = Pfx + jitter * ox23;
		Vector3d dy23 = Vector3d(Pfy.y) + jitter * oy23;
		Vector3d dz23 = Vector3d(Pfz.z) + jitter * oz23;

		Vector3d dx31 = Pfx + jitter * ox31;
		Vector3d dy31 = Vector3d(Pfy.z) + jitter * oy31;
		Vector3d dz31 = Vector3d(Pfz.x) + jitter * oz31;

		Vector3d dx32 = Pfx + jitter * ox32;
		Vector3d dy32 = Vector3d(Pfy.z) + jitter * oy32;
		Vector3d dz32 = Vector3d(Pfz.y) + jitter * oz32;

		Vector3d dx33 = Pfx + jitter * ox33;
		Vector3d dy33 = Vector3d(Pfy.z) + jitter * oy33;
		Vector3d dz33 = Vector3d(Pfz.z) + jitter * oz33;

		Vector3d d11 = dx11 * dx11 + dy11 * dy11 + dz11 * dz11;
		Vector3d d12 = dx12 * dx12 + dy12 * dy12 + dz12 * dz12;
		Vector3d d13 = dx13 * dx13 + dy13 * dy13 + dz13 * dz13;
		Vector3d d21 = dx21 * dx21 + dy21 * dy21 + dz21 * dz21;
		Vector3d d22 = dx22 * dx22 + dy22 * dy22 + dz22 * dz22;
		Vector3d d23 = dx23 * dx23 + dy23 * dy23 + dz23 * dz23;
		Vector3d d31 = dx31 * dx31 + dy31 * dy31 + dz31 * dz31;
		Vector3d d32 = dx32 * dx32 + dy32 * dy32 + dz32 * dz32;
		Vector3d d33 = dx33 * dx33 + dy33 * dy33 + dz33 * dz33;

#if 0
		// Cheat and sort out only F1
		Vector3d d1 = Math::min(Math::min(d11, d12), d13);
		Vector3d d2 = Math::min(Math::min(d21, d22), d23);
		Vector3d d3 = Math::min(Math::min(d31, d32), d33);
		Vector3d d = Math::min(Math::min(d1, d2), d3);
		d.x = Math::min(Math::min(d.x, d.y), d.z);
		return Math::sqrt(d.xx);
#else
		Vector3d d1a = min(d11, d12);
		d12 = max(d11, d12);
		d11 = min(d1a, d13);
		d13 = max(d1a, d13);
		d12 = min(d12, d13);
		Vector3d d2a = min(d21, d22);
		d22 = max(d21, d22);
		d21 = min(d2a, d23);
		d23 = max(d2a, d23);
		d22 = min(d22, d23);
		Vector3d d3a = min(d31, d32);
		d32 = max(d31, d32);
		d31 = min(d3a, d33);
		d33 = max(d3a, d33);
		d32 = min(d32, d33);
		Vector3d da = min(d11, d21);
		d21 = max(d11, d21);
		d11 = min(da, d31);
		d31 = max(da, d31);
		d11 = (d11.x < d11.y) ? d11 : Vector3d(d11.y, d11.x, d11.z);
		d11 = (d11.x < d11.z) ? d11 : Vector3d(d11.z, d11.y, d11.x);
		d12 = min(d12, d21);
		d12 = min(d12, d22);
		d12 = min(d12, d31);
		d12 = min(d12, d32);
		Vector2d vec = min(Vector2d(d11.y, d11.z), Vector2d(d12.x, d12.y));
		d11 = Vector3d(d11.x, vec.x, vec.y);
		d11.y = std::min(d11.y, d12.z);
		d11.y = std::min(d11.y, d11.z);
		return sqrt(Vector2d(d11.x, d11.y));
#endif
	}

	double Noise::noise(const int32 octaves,
		const double persistence,
		const double frequency,
		const double x,
		const double y)
	{
		double total = 0.0;
		double freq = frequency;
		double amplitude = 1.0;

		double maxAmplitude = 0.0;

		for (int32 i = 0; i < octaves; i++)
		{
			total += simplexNoise(x * freq, y * freq) * amplitude;

			freq *= 2.0;
			maxAmplitude += amplitude;
			amplitude *= persistence;
		}

		return total / maxAmplitude;
	}

	double Noise::noise(const int32 octaves,
		const double persistence,
		const double frequency,
		const double x,
		const double y,
		const double z)
	{
		double total = 0.0;
		double freq = frequency;
		double amplitude = 1.0;

		double maxAmplitude = 0.0;

		for (int32 i = 0; i < octaves; i++)
		{
			total += simplexNoise(x * freq, y * freq, z * freq) * amplitude;

			freq *= 2.0;
			maxAmplitude += amplitude;
			amplitude *= persistence;
		}

		return total / maxAmplitude;
	}

	double Noise::noise(const int32 octaves,
		const double persistence,
		const double frequency,
		const double x,
		const double y,
		const double z,
		const double w)
	{
		double total = 0.0;
		double freq = frequency;
		double amplitude = 1.0;

		double maxAmplitude = 0.0;

		for (int32 i = 0; i < octaves; i++)
		{
			total += simplexNoise(x * freq, y * freq, z * freq, w * freq) * amplitude;

			freq *= 2.0;
			maxAmplitude += amplitude;
			amplitude *= persistence;
		}

		return total / maxAmplitude;
	}

	double Noise::simplexNoise(const double x, const double y)
	{
		double n0, n1, n2;

		double F2 = 0.5 * (sqrtf(3.0) - 1.0);
		double s = (x + y) * F2;
		int32 i = fastFloor(x + s);
		int32 j = fastFloor(y + s);

		double G2 = (3.0 - sqrtf(3.0)) / 6.0;
		double t = (i + j) * G2;
		double X0 = i - t;
		double Y0 = j - t;
		double x0 = x - X0;
		double y0 = y - Y0;

		int32 i1, j1;
		if (x0 > y0)
		{
			i1 = 1;
			j1 = 0;
		}
		else
		{
			i1 = 0;
			j1 = 1;
		}

		double x1 = x0 - i1 + G2;
		double y1 = y0 - j1 + G2;
		double x2 = x0 - 1.0 + 2.0 * G2;
		double y2 = y0 - 1.0 + 2.0 * G2;

		int32 ii = i & 255;
		int32 jj = j & 255;
		int32 gi0 = perm[ii + perm[jj]] % 12;
		int32 gi1 = perm[ii + i1 + perm[jj + j1]] % 12;
		int32 gi2 = perm[ii + 1 + perm[jj + 1]] % 12;

		double t0 = 0.5 - x0 * x0 - y0 * y0;
		if (t0 < 0)
		{
			n0 = 0.0;
		}
		else
		{
			t0 *= t0;
			n0 = t0 * t0 * dot(grad3[gi0], x0, y0);
		}

		double t1 = 0.5 - x1 * x1 - y1 * y1;
		if (t1 < 0)
		{
			n1 = 0.0;
		}
		else
		{
			t1 *= t1;
			n1 = t1 * t1 * dot(grad3[gi1], x1, y1);
		}

		double t2 = 0.5 - x2 * x2 - y2 * y2;
		if (t2 < 0)
		{
			n2 = 0.0;
		}
		else
		{
			t2 *= t2;
			n2 = t2 * t2 * dot(grad3[gi2], x2, y2);
		}

		return 70.0 * (n0 + n1 + n2);
	}

	double Noise::simplexNoise(const double x, const double y, const double z)
	{
		double n0, n1, n2, n3;

		const double F3 = 1.0 / 3.0;
		double s = (x + y + z)*F3;
		int32 i = fastFloor(x + s);
		int32 j = fastFloor(y + s);
		int32 k = fastFloor(z + s);

		const double G3 = 1.0 / 6.0;
		double t = (i + j + k)*G3;
		double X0 = i - t;
		double Y0 = j - t;
		double Z0 = k - t;
		double x0 = x - X0;
		double y0 = y - Y0;
		double z0 = z - Z0;

		int32 i1, j1, k1;
		int32 i2, j2, k2;

		if (x0 >= y0)
		{
			if (y0 >= z0)
			{
				i1 = 1;
				j1 = 0;
				k1 = 0;
				i2 = 1;
				j2 = 1;
				k2 = 0;
			}
			else if (x0 >= z0)
			{
				i1 = 1;
				j1 = 0;
				k1 = 0;
				i2 = 1;
				j2 = 0;
				k2 = 1;
			}
			else
			{
				i1 = 0;
				j1 = 0;
				k1 = 1;
				i2 = 1;
				j2 = 0;
				k2 = 1;
			}
		}
		else
		{
			if (y0 < z0)
			{
				i1 = 0;
				j1 = 0;
				k1 = 1;
				i2 = 0;
				j2 = 1;
				k2 = 1;
			}
			else if (x0 < z0)
			{
				i1 = 0;
				j1 = 1;
				k1 = 0;
				i2 = 0;
				j2 = 1;
				k2 = 1;
			}
			else
			{
				i1 = 0;
				j1 = 1;
				k1 = 0;
				i2 = 1;
				j2 = 1;
				k2 = 0;
			}
		}

		double x1 = x0 - i1 + G3;
		double y1 = y0 - j1 + G3;
		double z1 = z0 - k1 + G3;
		double x2 = x0 - i2 + 2.0*G3;
		double y2 = y0 - j2 + 2.0*G3;
		double z2 = z0 - k2 + 2.0*G3;
		double x3 = x0 - 1.0 + 3.0*G3;
		double y3 = y0 - 1.0 + 3.0*G3;
		double z3 = z0 - 1.0 + 3.0*G3;

		int32 ii = i & 255;
		int32 jj = j & 255;
		int32 kk = k & 255;
		int32 gi0 = perm[ii + perm[jj + perm[kk]]] % 12;
		int32 gi1 = perm[ii + i1 + perm[jj + j1 + perm[kk + k1]]] % 12;
		int32 gi2 = perm[ii + i2 + perm[jj + j2 + perm[kk + k2]]] % 12;
		int32 gi3 = perm[ii + 1 + perm[jj + 1 + perm[kk + 1]]] % 12;

		double t0 = 0.6 - x0 * x0 - y0 * y0 - z0 * z0;
		if (t0 < 0)
		{
			n0 = 0.0;
		}
		else
		{
			t0 *= t0;
			n0 = t0 * t0 * dot(grad3[gi0], x0, y0, z0);
		}

		double t1 = 0.6 - x1 * x1 - y1 * y1 - z1 * z1;
		if (t1 < 0)
		{
			n1 = 0.0;
		}
		else 
		{
			t1 *= t1;
			n1 = t1 * t1 * dot(grad3[gi1], x1, y1, z1);
		}

		double t2 = 0.6 - x2 * x2 - y2 * y2 - z2 * z2;
		if (t2 < 0)
		{
			n2 = 0.0;
		}
		else
		{
			t2 *= t2;
			n2 = t2 * t2 * dot(grad3[gi2], x2, y2, z2);
		}

		double t3 = 0.6 - x3 * x3 - y3 * y3 - z3 * z3;
		if (t3 < 0)
		{
			n3 = 0.0;
		}
		else 
		{
			t3 *= t3;
			n3 = t3 * t3 * dot(grad3[gi3], x3, y3, z3);
		}

		return 32.0 * (n0 + n1 + n2 + n3);
	}

	double Noise::simplexNoise(const double x, const double y, const double z, const double w)
	{
		double F4 = (sqrtf(5.0) - 1.0) / 4.0;
		double G4 = (5.0 - sqrtf(5.0)) / 20.0;
		double n0, n1, n2, n3, n4;

		double s = (x + y + z + w) * F4;
		int32 i = fastFloor(x + s);
		int32 j = fastFloor(y + s);
		int32 k = fastFloor(z + s);
		int32 l = fastFloor(w + s);
		double t = (i + j + k + l) * G4;
		double X0 = i - t;
		double Y0 = j - t;
		double Z0 = k - t;
		double W0 = l - t;

		double x0 = x - X0;
		double y0 = y - Y0;
		double z0 = z - Z0;
		double w0 = w - W0;

		int32 c1 = (x0 > y0) ? 32 : 0;
		int32 c2 = (x0 > z0) ? 16 : 0;
		int32 c3 = (y0 > z0) ? 8 : 0;
		int32 c4 = (x0 > w0) ? 4 : 0;
		int32 c5 = (y0 > w0) ? 2 : 0;
		int32 c6 = (z0 > w0) ? 1 : 0;
		int32 c = c1 + c2 + c3 + c4 + c5 + c6;

		int32 i1, j1, k1, l1;
		int32 i2, j2, k2, l2;
		int32 i3, j3, k3, l3;

		i1 = simplex[c][0] >= 3 ? 1 : 0;
		j1 = simplex[c][1] >= 3 ? 1 : 0;
		k1 = simplex[c][2] >= 3 ? 1 : 0;
		l1 = simplex[c][3] >= 3 ? 1 : 0;

		i2 = simplex[c][0] >= 2 ? 1 : 0;
		j2 = simplex[c][1] >= 2 ? 1 : 0;
		k2 = simplex[c][2] >= 2 ? 1 : 0;
		l2 = simplex[c][3] >= 2 ? 1 : 0;

		i3 = simplex[c][0] >= 1 ? 1 : 0;
		j3 = simplex[c][1] >= 1 ? 1 : 0;
		k3 = simplex[c][2] >= 1 ? 1 : 0;
		l3 = simplex[c][3] >= 1 ? 1 : 0;

		double x1 = x0 - i1 + G4;
		double y1 = y0 - j1 + G4;
		double z1 = z0 - k1 + G4;
		double w1 = w0 - l1 + G4;
		double x2 = x0 - i2 + 2.0*G4;
		double y2 = y0 - j2 + 2.0*G4;
		double z2 = z0 - k2 + 2.0*G4;
		double w2 = w0 - l2 + 2.0*G4;
		double x3 = x0 - i3 + 3.0*G4;
		double y3 = y0 - j3 + 3.0*G4;
		double z3 = z0 - k3 + 3.0*G4;
		double w3 = w0 - l3 + 3.0*G4;
		double x4 = x0 - 1.0 + 4.0*G4;
		double y4 = y0 - 1.0 + 4.0*G4;
		double z4 = z0 - 1.0 + 4.0*G4;
		double w4 = w0 - 1.0 + 4.0*G4;

		int32 ii = i & 255;
		int32 jj = j & 255;
		int32 kk = k & 255;
		int32 ll = l & 255;
		int32 gi0 = perm[ii + perm[jj + perm[kk + perm[ll]]]] % 32;
		int32 gi1 = perm[ii + i1 + perm[jj + j1 + perm[kk + k1 + perm[ll + l1]]]] % 32;
		int32 gi2 = perm[ii + i2 + perm[jj + j2 + perm[kk + k2 + perm[ll + l2]]]] % 32;
		int32 gi3 = perm[ii + i3 + perm[jj + j3 + perm[kk + k3 + perm[ll + l3]]]] % 32;
		int32 gi4 = perm[ii + 1 + perm[jj + 1 + perm[kk + 1 + perm[ll + 1]]]] % 32;

		double t0 = 0.6 - x0 * x0 - y0 * y0 - z0 * z0 - w0 * w0;
		if (t0 < 0)
		{
			n0 = 0.0;
		}
		else 
		{
			t0 *= t0;
			n0 = t0 * t0 * dot(grad4[gi0], x0, y0, z0, w0);
		}

		double t1 = 0.6 - x1 * x1 - y1 * y1 - z1 * z1 - w1 * w1;
		if (t1 < 0)
		{
			n1 = 0.0;
		}
		else
		{
			t1 *= t1;
			n1 = t1 * t1 * dot(grad4[gi1], x1, y1, z1, w1);
		}

		double t2 = 0.6 - x2 * x2 - y2 * y2 - z2 * z2 - w2 * w2;
		if (t2 < 0)
		{
			n2 = 0.0;
		}
		else
		{
			t2 *= t2;
			n2 = t2 * t2 * dot(grad4[gi2], x2, y2, z2, w2);
		}

		double t3 = 0.6 - x3 * x3 - y3 * y3 - z3 * z3 - w3 * w3;
		if (t3 < 0)
		{
			n3 = 0.0;
		}
		else
		{
			t3 *= t3;
			n3 = t3 * t3 * dot(grad4[gi3], x3, y3, z3, w3);
		}

		double t4 = 0.6 - x4 * x4 - y4 * y4 - z4 * z4 - w4 * w4;
		if (t4 < 0)
		{
			n4 = 0.0;
		}
		else
		{
			t4 *= t4;
			n4 = t4 * t4 * dot(grad4[gi4], x4, y4, z4, w4);
		}

		return 27.0 * (n0 + n1 + n2 + n3 + n4);
	}


	double Noise::noiseScaled(const int32 octaves, const double persistence, const double frequency, const double x, const double y, const double low, const double high)
	{
		return Math::range(noise(octaves, persistence, frequency, x, y), -1.0, 1.0, low, high);
	}

	double Noise::noiseScaled(const int32 octaves, const double persistence, const double frequency, const double x, const double y, const double z, const double low, const double high)
	{
		return Math::range(noise(octaves, persistence, frequency, x, y, z), -1.0, 1.0, low, high);
	}

	double Noise::noiseScaled(const int32 octaves, const double persistence, const double frequency, const double x, const double y, const double z, const double w, const double low, const double high)
	{
		return Math::range(noise(octaves, persistence, frequency, x, y, z, w), -1.0, 1.0, low, high);
	}


	double Noise::simplexScaled(const double x, const double y, const double low, const double high)
	{
		return Math::range(simplexNoise(x, y), -1.0, 1.0, low, high);
	}

	double Noise::simplexScaled(const double x, const double y, const double z, const double low, const double high)
	{
		return Math::range(simplexNoise(x, y, z), -1.0, 1.0, low, high);
	}

	double Noise::simplexScaled(const double x, const double y, const double z, const double w, const double low, const double high)
	{
		return Math::range(simplexNoise(x, y, z, w), -1.0, 1.0, low, high);
	}


	Vector3d Noise::permute(const Vector3d& x)
	{
		return mod((34.0 * x + 1.0) * x, 289.0);
	}


	double Noise::performOperation(const Operation& operation, double a, double b)
	{
		switch (operation)
		{
		case Operation::Add:
			return a + b;
		case Operation::Subtract:
			return a - b;
		case Operation::Multiply:
			return a * b;
		case Operation::Divide:
			return a / b;
		}

		return 0.0;
	}

	void Noise::evaluateNoise(const Vector3d& position,
		const Array<FunctionProperties>& functions,
		double* modifier,
		const Operation& operation,
		double& height)
	{

		for (auto iter = functions.createConstIterator(); iter; iter++)
		{
			auto& fn = *iter;

			double h = 0.0;
			double* nextModifier;
			Operation nextOperation;

			if (fn.function == Type::Constant)
			{
				nextModifier = &h;
				h = fn.low;

				if (modifier)
				{
					h = performOperation(operation, h, *modifier);
				}

				if (fn.clamp.x != 0.0 || fn.clamp.y != 0.0)
				{
					h = Math::clamp(*modifier, (double)fn.clamp.x, (double)fn.clamp.y);
				}
				nextOperation = fn.operation;
			}
			else if (fn.function == Type::PassThrough)
			{
				nextModifier = modifier;

				if (modifier)
				{
					h = performOperation(operation, *modifier, fn.low);

					if (fn.clamp.x != 0.0 || fn.clamp.y != 0.0)
					{
						h = Math::clamp(h, (double)fn.clamp.x, (double)fn.clamp.y);
					}
				}
				nextOperation = operation;
			}
			else if (fn.function == Type::Squared)
			{
				nextModifier = modifier;

				if (modifier)
				{
					*modifier = (*modifier) * (*modifier);

					if (fn.clamp.x != 0.0 || fn.clamp.y != 0.0)
					{
						h = Math::clamp(h, (double)fn.clamp.x, (double)fn.clamp.y);
					}
				}
				nextOperation = operation;
			}
			else if (fn.function == Type::Cubed)
			{
				nextModifier = modifier;

				if (modifier)
				{
					*modifier = (*modifier) * (*modifier) * (*modifier);

					if (fn.clamp.x != 0.0 || fn.clamp.y != 0.0)
					{
						h = Math::clamp(h, (double)fn.clamp.x, (double)fn.clamp.y);
					}
				}
				nextOperation = operation;
			}
			else
			{
				nextModifier = &h;
				Vector2d ff;
				double tmp;
				double total = 0.0;
				double maxAmplitude = 0.0;
				double amplitude = 1.0;
				double frequency = fn.frequency;

				for (int32 i = 0; i < fn.octaves; i++)
				{
					switch (fn.function)
					{

					case Type::CubedNoise:
					case Type::SquaredNoise:
					case Type::BaseNoise:
						total += Noise::simplexNoise(position.x * frequency, position.y * frequency, position.z * frequency) * amplitude;
						break;
					case Type::RidgedNoise:
						total += ((1.0 - abs(Noise::simplexNoise(position.x * frequency, position.y * frequency, position.z * frequency))) * 2.0 - 1.0) * amplitude;
						break;
					case Type::AbsNoise:
						total += abs(Noise::simplexNoise(position.x * frequency, position.y * frequency, position.z * frequency)) * amplitude;
						break;
					case Type::CellularNoise:
						ff = Noise::cellular(position * (double)frequency);
						total += (ff.y - ff.x) * amplitude;
						break;
					case Type::CellularSquaredNoise:
						ff = Noise::cellular(position * (double)frequency);
						tmp = ff.y - ff.x;
						total += tmp * tmp * amplitude;
						break;
					case Type::CellularCubedNoise:
						ff = Noise::cellular(position * (double)frequency);
						tmp = ff.y - ff.x;
						total += tmp * tmp * tmp * amplitude;
						break;
					default:
						break;
					}

					frequency *= 2.0;
					maxAmplitude += amplitude;
					amplitude *= fn.persistence;
				}

				total = (total / maxAmplitude);

				switch (fn.function)
				{
				case Type::CubedNoise:
					total = total * total * total;
					break;
				case Type::SquaredNoise:
					total = total * total;
					break;
				default:
					break;
				}

				if (fn.low != -1.0 || fn.high != 1.0)
				{
					h = Math::range(total, -1.0, 1.0, fn.low, fn.high);
				}
				else
				{
					h = total;
				}

				if (fn.clamp.x != 0.0 || fn.clamp.y != 0.0)
				{
					h = Math::clamp(h, (double)fn.clamp.x, (double)fn.clamp.y);
				}

				if (modifier)
				{
					//LOGF(Info, CoreEngine, "h: %d, %f, %f", operation, h, *modifier);
					h = performOperation(operation, h, *modifier);
				}
				nextOperation = fn.operation;
			}

			if (fn.children.size())
			{
				if (!(nextOperation == Operation::Multiply && *nextModifier == 0.0))
				{
					evaluateNoise(position, fn.children, nextModifier, nextOperation, height);
				}
			}
			else
			{
				//LOGF(Info, CoreEngine, "height: %d, %f, %f", fn.operation, height, h);
				height = performOperation(fn.operation, height, h);
			}
		}
	}

	void Noise::evaluateNoise(const Vector3d& position,
		const Base& base,
		double& height)
	{
		height = base.base;

		evaluateNoise(position, base.functions, nullptr, Noise::Add, height);
	}


	const double Noise::grad3[12][3] = {
	   { 1, 1, 0 }, { -1, 1, 0 }, { 1, -1, 0 }, { -1, -1, 0 },
	   { 1, 0, 1 }, { -1, 0, 1 }, { 1, 0, -1 }, { -1, 0, -1 },
	   { 0, 1, 1 }, { 0, -1, 1 }, { 0, 1, -1 }, { 0, -1, -1 }
	};

	const double Noise::grad4[32][4] = {
	   { 0, 1, 1, 1 }, { 0, 1, 1, -1 }, { 0, 1, -1, 1 }, { 0, 1, -1, -1 },
	   { 0, -1, 1, 1 }, { 0, -1, 1, -1 }, { 0, -1, -1, 1 }, { 0, -1, -1, -1 },
	   { 1, 0, 1, 1 }, { 1, 0, 1, -1 }, { 1, 0, -1, 1 }, { 1, 0, -1, -1 },
	   { -1, 0, 1, 1 }, { -1, 0, 1, -1 }, { -1, 0, -1, 1 }, { -1, 0, -1, -1 },
	   { 1, 1, 0, 1 }, { 1, 1, 0, -1 }, { 1, -1, 0, 1 }, { 1, -1, 0, -1 },
	   { -1, 1, 0, 1 }, { -1, 1, 0, -1 }, { -1, -1, 0, 1 }, { -1, -1, 0, -1 },
	   { 1, 1, 1, 0 }, { 1, 1, -1, 0 }, { 1, -1, 1, 0 }, { 1, -1, -1, 0 },
	   { -1, 1, 1, 0 }, { -1, 1, -1, 0 }, { -1, -1, 1, 0 }, { -1, -1, -1, 0 }
	};


	const int32 Noise::perm[512] = 
	{
	   151, 160, 137, 91, 90, 15, 131, 13, 201, 95, 96, 53, 194, 233, 7, 225, 140, 36, 103, 30, 69, 142,
	   8, 99, 37, 240, 21, 10, 23, 190, 6, 148, 247, 120, 234, 75, 0, 26, 197, 62, 94, 252, 219, 203, 117,
	   35, 11, 32, 57, 177, 33, 88, 237, 149, 56, 87, 174, 20, 125, 136, 171, 168, 68, 175, 74, 165, 71,
	   134, 139, 48, 27, 166, 77, 146, 158, 231, 83, 111, 229, 122, 60, 211, 133, 230, 220, 105, 92, 41,
	   55, 46, 245, 40, 244, 102, 143, 54, 65, 25, 63, 161, 1, 216, 80, 73, 209, 76, 132, 187, 208, 89,
	   18, 169, 200, 196, 135, 130, 116, 188, 159, 86, 164, 100, 109, 198, 173, 186, 3, 64, 52, 217, 226,
	   250, 124, 123, 5, 202, 38, 147, 118, 126, 255, 82, 85, 212, 207, 206, 59, 227, 47, 16, 58, 17, 182,
	   189, 28, 42, 223, 183, 170, 213, 119, 248, 152, 2, 44, 154, 163, 70, 221, 153, 101, 155, 167, 43,
	   172, 9, 129, 22, 39, 253, 19, 98, 108, 110, 79, 113, 224, 232, 178, 185, 112, 104, 218, 246, 97,
	   228, 251, 34, 242, 193, 238, 210, 144, 12, 191, 179, 162, 241, 81, 51, 145, 235, 249, 14, 239,
	   107, 49, 192, 214, 31, 181, 199, 106, 157, 184, 84, 204, 176, 115, 121, 50, 45, 127, 4, 150, 254,
	   138, 236, 205, 93, 222, 114, 67, 29, 24, 72, 243, 141, 128, 195, 78, 66, 215, 61, 156, 180,

	   151, 160, 137, 91, 90, 15, 131, 13, 201, 95, 96, 53, 194, 233, 7, 225, 140, 36, 103, 30, 69, 142,
	   8, 99, 37, 240, 21, 10, 23, 190, 6, 148, 247, 120, 234, 75, 0, 26, 197, 62, 94, 252, 219, 203, 117,
	   35, 11, 32, 57, 177, 33, 88, 237, 149, 56, 87, 174, 20, 125, 136, 171, 168, 68, 175, 74, 165, 71,
	   134, 139, 48, 27, 166, 77, 146, 158, 231, 83, 111, 229, 122, 60, 211, 133, 230, 220, 105, 92, 41,
	   55, 46, 245, 40, 244, 102, 143, 54, 65, 25, 63, 161, 1, 216, 80, 73, 209, 76, 132, 187, 208, 89,
	   18, 169, 200, 196, 135, 130, 116, 188, 159, 86, 164, 100, 109, 198, 173, 186, 3, 64, 52, 217, 226,
	   250, 124, 123, 5, 202, 38, 147, 118, 126, 255, 82, 85, 212, 207, 206, 59, 227, 47, 16, 58, 17, 182,
	   189, 28, 42, 223, 183, 170, 213, 119, 248, 152, 2, 44, 154, 163, 70, 221, 153, 101, 155, 167, 43,
	   172, 9, 129, 22, 39, 253, 19, 98, 108, 110, 79, 113, 224, 232, 178, 185, 112, 104, 218, 246, 97,
	   228, 251, 34, 242, 193, 238, 210, 144, 12, 191, 179, 162, 241, 81, 51, 145, 235, 249, 14, 239,
	   107, 49, 192, 214, 31, 181, 199, 106, 157, 184, 84, 204, 176, 115, 121, 50, 45, 127, 4, 150, 254,
	   138, 236, 205, 93, 222, 114, 67, 29, 24, 72, 243, 141, 128, 195, 78, 66, 215, 61, 156, 180
	};


	const int32 Noise::simplex[64][4] = {
		{ 0, 1, 2, 3 }, { 0, 1, 3, 2 }, { 0, 0, 0, 0 }, { 0, 2, 3, 1 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 1, 2, 3, 0 },
		{ 0, 2, 1, 3 }, { 0, 0, 0, 0 }, { 0, 3, 1, 2 }, { 0, 3, 2, 1 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 1, 3, 2, 0 },
		{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
		{ 1, 2, 0, 3 }, { 0, 0, 0, 0 }, { 1, 3, 0, 2 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 2, 3, 0, 1 }, { 2, 3, 1, 0 },
		{ 1, 0, 2, 3 }, { 1, 0, 3, 2 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 2, 0, 3, 1 }, { 0, 0, 0, 0 }, { 2, 1, 3, 0 },
		{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
		{ 2, 0, 1, 3 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 3, 0, 1, 2 }, { 3, 0, 2, 1 }, { 0, 0, 0, 0 }, { 3, 1, 2, 0 },
		{ 2, 1, 0, 3 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 3, 1, 0, 2 }, { 0, 0, 0, 0 }, { 3, 2, 0, 1 }, { 3, 2, 1, 0 }
	};
}