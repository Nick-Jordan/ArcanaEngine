#include "Noise.h"

#include <algorithm>

namespace Arcana
{

	static bool initialized = false;

	static int p[2 * 256 + 2];
	static float g1[2 * 256 + 2];
	static float g2[2 * 256 + 2][2];
	static float g3[2 * 256 + 2][3];

	static void init()
	{
		int i, j, k;
		long seed = 12345;
		for (i = 0; i < 256; ++i) {
			p[i] = i;
			g1[i] = (float)((lrandom(&seed) % (2 * 256)) - 256) / 256;
			for (j = 0; j < 2; ++j) {
				g2[i][j] = (float)((lrandom(&seed) % (2 * 256)) - 256) / 256;
			}
			float l = sqrt(g2[i][0] * g2[i][0] + g2[i][1] * g2[i][1]);
			g2[i][0] /= l;
			g2[i][1] /= l;
			for (j = 0; j < 3; ++j) {
				g3[i][j] = (float)((lrandom(&seed) % (2 * 256)) - 256) / 256;
			}
			l = sqrt(g3[i][0] * g3[i][0] + g3[i][1] * g3[i][1] + g3[i][2] * g3[i][2]);
			g3[i][0] /= l;
			g3[i][1] /= l;
			g3[i][2] /= l;
		}
		while (--i) {
			k = p[i];
			p[i] = p[j = lrandom(&seed) % 256];
			p[j] = k;
		}
		for (i = 0; i < 256 + 2; ++i) {
			p[256 + i] = p[i];
			g1[256 + i] = g1[i];
			for (j = 0; j < 2; ++j) {
				g2[256 + i][j] = g2[i][j];
			}
			for (j = 0; j < 3; ++j) {
				g3[256 + i][j] = g3[i][j];
			}
		}
	}

#define S_CURVE(t) ( t * t * (3.0f - 2.0f * t) )

#define LERP(t, a, b) ( a + t * (b - a) )

#define SETUP(x,b0,b1,r0,r1)\
    t = x + 0x1000;\
    b0 = ((int)t) & 0xFF;\
    b1 = (b0+1) & 0xFF;\
    r0 = t - (int)floor(t);\
    r1 = r0 - 1.0f;

	float cnoise(float x, float y, int period)
	{
		int bx0, bx1, by0, by1, b00, b10, b01, b11;
		float rx0, rx1, ry0, ry1, *q, sx, sy, a, b, t, u, v;
		register int i, j;

		if (!initialized) {
			initialized = true;
			init();
		}

		SETUP(x, bx0, bx1, rx0, rx1);
		SETUP(y, by0, by1, ry0, ry1);

		if (period != 0) {
			i = p[bx0 % period];
			j = p[bx1 % period];
			b00 = p[i + by0 % period];
			b10 = p[j + by0 % period];
			b01 = p[i + by1 % period];
			b11 = p[j + by1 % period];
		}
		else {
			i = p[bx0];
			j = p[bx1];
			b00 = p[i + by0];
			b10 = p[j + by0];
			b01 = p[i + by1];
			b11 = p[j + by1];
		}

		sx = S_CURVE(rx0);
		sy = S_CURVE(ry0);

#define AT2(rx,ry) ( rx * q[0] + ry * q[1] )

		q = g2[b00];
		u = AT2(rx0, ry0);
		q = g2[b10];
		v = AT2(rx1, ry0);
		a = LERP(sx, u, v);

		q = g2[b01];
		u = AT2(rx0, ry1);
		q = g2[b11];
		v = AT2(rx1, ry1);
		b = LERP(sx, u, v);

		return LERP(sy, a, b);
	}

	float cnoise(float x, float y, float z, int period)
	{
		int bx0, bx1, by0, by1, bz0, bz1, b00, b10, b01, b11;
		float rx0, rx1, ry0, ry1, rz0, rz1, *q, sy, sz, a, b, c, d, t, u, v;
		register int i, j;

		if (!initialized) {
			initialized = true;
			init();
		}

		SETUP(x, bx0, bx1, rx0, rx1);
		SETUP(y, by0, by1, ry0, ry1);
		SETUP(z, bz0, bz1, rz0, rz1);

		if (period != 0) {
			i = p[bx0 % period];
			j = p[bx1 % period];
			b00 = p[i + by0 % period];
			b10 = p[j + by0 % period];
			b01 = p[i + by1 % period];
			b11 = p[j + by1 % period];
			bz0 = bz0 % period;
			bz1 = bz1 % period;
		}
		else {
			i = p[bx0];
			j = p[bx1];
			b00 = p[i + by0];
			b10 = p[j + by0];
			b01 = p[i + by1];
			b11 = p[j + by1];
		}

		t = S_CURVE(rx0);
		sy = S_CURVE(ry0);
		sz = S_CURVE(rz0);

#define AT3(rx,ry,rz) ( rx * q[0] + ry * q[1] + rz * q[2] )

		q = g3[b00 + bz0];
		u = AT3(rx0, ry0, rz0);
		q = g3[b10 + bz0];
		v = AT3(rx1, ry0, rz0);
		a = LERP(t, u, v);

		q = g3[b01 + bz0];
		u = AT3(rx0, ry1, rz0);
		q = g3[b11 + bz0];
		v = AT3(rx1, ry1, rz0);
		b = LERP(t, u, v);

		c = LERP(sy, a, b);

		q = g3[b00 + bz1];
		u = AT3(rx0, ry0, rz1);
		q = g3[b10 + bz1];
		v = AT3(rx1, ry0, rz1);
		a = LERP(t, u, v);

		q = g3[b01 + bz1];
		u = AT3(rx0, ry1, rz1);
		q = g3[b11 + bz1];
		v = AT3(rx1, ry1, rz1);
		b = LERP(t, u, v);

		d = LERP(sy, a, b);

		return LERP(sz, c, d);
	}

	static int perm[] = { 151,160,137,91,90,15,
		131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
		190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
		88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
		77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
		102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
		135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
		5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
		223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
		129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
		251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
		49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
		138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180,
		151,160,137,91,90,15,
		131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
		190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
		88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
		77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
		102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
		135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
		5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
		223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
		129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
		251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
		49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
		138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180 };

	static int grad3[][3] = { { 1,1,0 },{ -1,1,0 },{ 1,-1,0 },{ -1,-1,0 },
	{ 1,0,1 },{ -1,0,1 },{ 1,0,-1 },{ -1,0,-1 },
	{ 0,1,1 },{ 0,-1,1 },{ 0,1,-1 },{ 0,-1,-1 } };

	static int grad4[][4] = { { 0,1,1,1 },{ 0,1,1,-1 },{ 0,1,-1,1 },{ 0,1,-1,-1 },
	{ 0,-1,1,1 },{ 0,-1,1,-1 },{ 0,-1,-1,1 },{ 0,-1,-1,-1 },
	{ 1,0,1,1 },{ 1,0,1,-1 },{ 1,0,-1,1 },{ 1,0,-1,-1 },
	{ -1,0,1,1 },{ -1,0,1,-1 },{ -1,0,-1,1 },{ -1,0,-1,-1 },
	{ 1,1,0,1 },{ 1,1,0,-1 },{ 1,-1,0,1 },{ 1,-1,0,-1 },
	{ -1,1,0,1 },{ -1,1,0,-1 },{ -1,-1,0,1 },{ -1,-1,0,-1 },
	{ 1,1,1,0 },{ 1,1,-1,0 },{ 1,-1,1,0 },{ 1,-1,-1,0 },
	{ -1,1,1,0 },{ -1,1,-1,0 },{ -1,-1,1,0 },{ -1,-1,-1,0 } };

	static int simplex[][4] = {
		{ 0,1,2,3 },{ 0,1,3,2 },{ 0,0,0,0 },{ 0,2,3,1 },{ 0,0,0,0 },{ 0,0,0,0 },{ 0,0,0,0 },{ 1,2,3,0 },
		{ 0,2,1,3 },{ 0,0,0,0 },{ 0,3,1,2 },{ 0,3,2,1 },{ 0,0,0,0 },{ 0,0,0,0 },{ 0,0,0,0 },{ 1,3,2,0 },
		{ 0,0,0,0 },{ 0,0,0,0 },{ 0,0,0,0 },{ 0,0,0,0 },{ 0,0,0,0 },{ 0,0,0,0 },{ 0,0,0,0 },{ 0,0,0,0 },
		{ 1,2,0,3 },{ 0,0,0,0 },{ 1,3,0,2 },{ 0,0,0,0 },{ 0,0,0,0 },{ 0,0,0,0 },{ 2,3,0,1 },{ 2,3,1,0 },
		{ 1,0,2,3 },{ 1,0,3,2 },{ 0,0,0,0 },{ 0,0,0,0 },{ 0,0,0,0 },{ 2,0,3,1 },{ 0,0,0,0 },{ 2,1,3,0 },
		{ 0,0,0,0 },{ 0,0,0,0 },{ 0,0,0,0 },{ 0,0,0,0 },{ 0,0,0,0 },{ 0,0,0,0 },{ 0,0,0,0 },{ 0,0,0,0 },
		{ 2,0,1,3 },{ 0,0,0,0 },{ 0,0,0,0 },{ 0,0,0,0 },{ 3,0,1,2 },{ 3,0,2,1 },{ 0,0,0,0 },{ 3,1,2,0 },
		{ 2,1,0,3 },{ 0,0,0,0 },{ 0,0,0,0 },{ 0,0,0,0 },{ 3,1,0,2 },{ 0,0,0,0 },{ 3,2,0,1 },{ 3,2,1,0 } };

	static int fastfloor(float x)
	{
		return x>0 ? (int)x : (int)x - 1;
	}

	static float dot(int g[], float x, float y)
	{
		return g[0] * x + g[1] * y;
	}

	static float dot(int g[], float x, float y, float z)
	{
		return g[0] * x + g[1] * y + g[2] * z;
	}

	static float dot(int g[], float x, float y, float z, float w)
	{
		return g[0] * x + g[1] * y + g[2] * z + g[3] * w;
	}

	float snoise(float xin, float yin, int P)
	{
		float n0, n1, n2;

		float F2 = 0.5f*(sqrt(3.0f) - 1.0f);
		float s = (xin + yin)*F2;
		int i = fastfloor(xin + s);
		int j = fastfloor(yin + s);
		float G2 = (3.0f - sqrt(3.0f)) / 6.0f;
		float t = (i + j)*G2;
		float X0 = i - t;
		float Y0 = j - t;
		float x0 = xin - X0;
		float y0 = yin - Y0;
	
		int i1, j1;
		if (x0>y0) { i1 = 1; j1 = 0; }
		else { i1 = 0; j1 = 1; }

		float x1 = x0 - i1 + G2;
		float y1 = y0 - j1 + G2;
		float x2 = x0 - 1.0f + 2.0f * G2;
		float y2 = y0 - 1.0f + 2.0f * G2;

		int ii, jj, gi0, gi1, gi2;
		if (P == 0) {
			ii = i & 255;
			jj = j & 255;
			gi0 = perm[ii + perm[jj]] % 12;
			gi1 = perm[ii + i1 + perm[jj + j1]] % 12;
			gi2 = perm[ii + 1 + perm[jj + 1]] % 12;
		}
		else {
			ii = (i%P) & 255;
			jj = (j%P) & 255;
			gi0 = perm[ii + perm[jj]] % 12;
			gi1 = perm[(ii + i1) % P + perm[(jj + j1) % P]] % 12;
			gi2 = perm[(ii + 1) % P + perm[(jj + 1) % P]] % 12;
		}

		float t0 = 0.5f - x0*x0 - y0*y0;
		if (t0<0) n0 = 0.0f;
		else {
			t0 *= t0;
			n0 = t0 * t0 * dot(grad3[gi0], x0, y0);
		}
		float t1 = 0.5f - x1*x1 - y1*y1;
		if (t1<0) n1 = 0.0f;
		else {
			t1 *= t1;
			n1 = t1 * t1 * dot(grad3[gi1], x1, y1);
		}
		float t2 = 0.5f - x2*x2 - y2*y2;
		if (t2<0) n2 = 0.0f;
		else {
			t2 *= t2;
			n2 = t2 * t2 * dot(grad3[gi2], x2, y2);
		}

		return 70.0f * (n0 + n1 + n2);
	}

	float snoise(float xin, float yin, float zin, int P)
	{
		float n0, n1, n2, n3;

		float F3 = 1.0f / 3.0f;
		float s = (xin + yin + zin)*F3;
		int i = fastfloor(xin + s);
		int j = fastfloor(yin + s);
		int k = fastfloor(zin + s);
		float G3 = 1.0f / 6.0f;
		float t = (i + j + k)*G3;
		float X0 = i - t;
		float Y0 = j - t;
		float Z0 = k - t;
		float x0 = xin - X0;
		float y0 = yin - Y0;
		float z0 = zin - Z0;

		int i1, j1, k1;
		int i2, j2, k2;
		if (x0 >= y0) {
			if (y0 >= z0)
			{
				i1 = 1; j1 = 0; k1 = 0; i2 = 1; j2 = 1; k2 = 0;
			}
			else if (x0 >= z0) { i1 = 1; j1 = 0; k1 = 0; i2 = 1; j2 = 0; k2 = 1; }
			else { i1 = 0; j1 = 0; k1 = 1; i2 = 1; j2 = 0; k2 = 1; }
		}
		else {
			if (y0<z0) { i1 = 0; j1 = 0; k1 = 1; i2 = 0; j2 = 1; k2 = 1; }
			else if (x0<z0) { i1 = 0; j1 = 1; k1 = 0; i2 = 0; j2 = 1; k2 = 1; }
			else { i1 = 0; j1 = 1; k1 = 0; i2 = 1; j2 = 1; k2 = 0; }
		}

		float x1 = x0 - i1 + G3;
		float y1 = y0 - j1 + G3;
		float z1 = z0 - k1 + G3;
		float x2 = x0 - i2 + 2.0f*G3;
		float y2 = y0 - j2 + 2.0f*G3;
		float z2 = z0 - k2 + 2.0f*G3;
		float x3 = x0 - 1.0f + 3.0f*G3;
		float y3 = y0 - 1.0f + 3.0f*G3;
		float z3 = z0 - 1.0f + 3.0f*G3;

		int ii, jj, kk, gi0, gi1, gi2, gi3;
		if (P == 0) {
			ii = i & 255;
			jj = j & 255;
			kk = k & 255;
			gi0 = perm[ii + perm[jj + perm[kk]]] % 12;
			gi1 = perm[ii + i1 + perm[jj + j1 + perm[kk + k1]]] % 12;
			gi2 = perm[ii + i2 + perm[jj + j2 + perm[kk + k2]]] % 12;
			gi3 = perm[ii + 1 + perm[jj + 1 + perm[kk + 1]]] % 12;
		}
		else {
			ii = (i%P) & 255;
			jj = (j%P) & 255;
			kk = (k%P) & 255;
			gi0 = perm[ii + perm[jj + perm[kk]]] % 12;
			gi1 = perm[(ii + i1) % P + perm[(jj + j1) % P + perm[(kk + k1) % P]]] % 12;
			gi2 = perm[(ii + i2) % P + perm[(jj + j2) % P + perm[(kk + k2) % P]]] % 12;
			gi3 = perm[(ii + 1) % P + perm[(jj + 1) % P + perm[(kk + 1) % P]]] % 12;
		}

		float t0 = 0.6f - x0*x0 - y0*y0 - z0*z0;
		if (t0<0) n0 = 0.0f;
		else {
			t0 *= t0;
			n0 = t0 * t0 * dot(grad3[gi0], x0, y0, z0);
		}
		float t1 = 0.6f - x1*x1 - y1*y1 - z1*z1;
		if (t1<0) n1 = 0.0f;
		else {
			t1 *= t1;
			n1 = t1 * t1 * dot(grad3[gi1], x1, y1, z1);
		}
		float t2 = 0.6f - x2*x2 - y2*y2 - z2*z2;
		if (t2<0) n2 = 0.0f;
		else {
			t2 *= t2;
			n2 = t2 * t2 * dot(grad3[gi2], x2, y2, z2);
		}
		float t3 = 0.6f - x3*x3 - y3*y3 - z3*z3;
		if (t3<0) n3 = 0.0f;
		else {
			t3 *= t3;
			n3 = t3 * t3 * dot(grad3[gi3], x3, y3, z3);
		}

		return 32.0f*(n0 + n1 + n2 + n3);
	}

	float snoise(float x, float y, float z, float w, int P)
	{
		float F4 = (sqrt(5.0f) - 1.0f) / 4.0f;
		float G4 = (5.0f - sqrt(5.0f)) / 20.0f;
		float n0, n1, n2, n3, n4;

		float s = (x + y + z + w) * F4;
		int i = fastfloor(x + s);
		int j = fastfloor(y + s);
		int k = fastfloor(z + s);
		int l = fastfloor(w + s);
		float t = (i + j + k + l) * G4;
		float X0 = i - t;
		float Y0 = j - t;
		float Z0 = k - t;
		float W0 = l - t;
		float x0 = x - X0;
		float y0 = y - Y0;
		float z0 = z - Z0;
		float w0 = w - W0;

		int c1 = (x0 > y0) ? 32 : 0;
		int c2 = (x0 > z0) ? 16 : 0;
		int c3 = (y0 > z0) ? 8 : 0;
		int c4 = (x0 > w0) ? 4 : 0;
		int c5 = (y0 > w0) ? 2 : 0;
		int c6 = (z0 > w0) ? 1 : 0;
		int c = c1 + c2 + c3 + c4 + c5 + c6;
		int i1, j1, k1, l1;
		int i2, j2, k2, l2;
		int i3, j3, k3, l3;

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

		float x1 = x0 - i1 + G4;
		float y1 = y0 - j1 + G4;
		float z1 = z0 - k1 + G4;
		float w1 = w0 - l1 + G4;
		float x2 = x0 - i2 + 2.0f*G4;
		float y2 = y0 - j2 + 2.0f*G4;
		float z2 = z0 - k2 + 2.0f*G4;
		float w2 = w0 - l2 + 2.0f*G4;
		float x3 = x0 - i3 + 3.0f*G4;
		float y3 = y0 - j3 + 3.0f*G4;
		float z3 = z0 - k3 + 3.0f*G4;
		float w3 = w0 - l3 + 3.0f*G4;
		float x4 = x0 - 1.0f + 4.0f*G4;
		float y4 = y0 - 1.0f + 4.0f*G4;
		float z4 = z0 - 1.0f + 4.0f*G4;
		float w4 = w0 - 1.0f + 4.0f*G4;

		int ii, jj, kk, ll, gi0, gi1, gi2, gi3, gi4;
		if (P == 0) {
			ii = i & 255;
			jj = j & 255;
			kk = k & 255;
			ll = l & 255;
			gi0 = perm[ii + perm[jj + perm[kk + perm[ll]]]] % 32;
			gi1 = perm[ii + i1 + perm[jj + j1 + perm[kk + k1 + perm[ll + l1]]]] % 32;
			gi2 = perm[ii + i2 + perm[jj + j2 + perm[kk + k2 + perm[ll + l2]]]] % 32;
			gi3 = perm[ii + i3 + perm[jj + j3 + perm[kk + k3 + perm[ll + l3]]]] % 32;
			gi4 = perm[ii + 1 + perm[jj + 1 + perm[kk + 1 + perm[ll + 1]]]] % 32;
		}
		else {
			ii = (i%P) & 255;
			jj = (j%P) & 255;
			kk = (k%P) & 255;
			ll = (l%P) & 255;
			gi0 = perm[ii + perm[jj + perm[kk + perm[ll]]]] % 32;
			gi1 = perm[(ii + i1) % P + perm[(jj + j1) % P + perm[(kk + k1) % P + perm[(ll + l1) % P]]]] % 32;
			gi2 = perm[(ii + i2) % P + perm[(jj + j2) % P + perm[(kk + k2) % P + perm[(ll + l2) % P]]]] % 32;
			gi3 = perm[(ii + i3) % P + perm[(jj + j3) % P + perm[(kk + k3) % P + perm[(ll + l3) % P]]]] % 32;
			gi4 = perm[(ii + 1) % P + perm[(jj + 1) % P + perm[(kk + 1) % P + perm[(ll + 1) % P]]]] % 32;
		}

		float t0 = 0.6f - x0*x0 - y0*y0 - z0*z0 - w0*w0;
		if (t0<0) n0 = 0.0f;
		else {
			t0 *= t0;
			n0 = t0 * t0 * dot(grad4[gi0], x0, y0, z0, w0);
		}
		float t1 = 0.6f - x1*x1 - y1*y1 - z1*z1 - w1*w1;
		if (t1<0) n1 = 0.0f;
		else {
			t1 *= t1;
			n1 = t1 * t1 * dot(grad4[gi1], x1, y1, z1, w1);
		}
		float t2 = 0.6f - x2*x2 - y2*y2 - z2*z2 - w2*w2;
		if (t2<0) n2 = 0.0f;
		else {
			t2 *= t2;
			n2 = t2 * t2 * dot(grad4[gi2], x2, y2, z2, w2);
		}
		float t3 = 0.6f - x3*x3 - y3*y3 - z3*z3 - w3*w3;
		if (t3<0) n3 = 0.0f;
		else {
			t3 *= t3;
			n3 = t3 * t3 * dot(grad4[gi3], x3, y3, z3, w3);
		}
		float t4 = 0.6f - x4*x4 - y4*y4 - z4*z4 - w4*w4;
		if (t4<0) n4 = 0.0f;
		else {
			t4 *= t4;
			n4 = t4 * t4 * dot(grad4[gi4], x4, y4, z4, w4);
		}

		return 27.0f * (n0 + n1 + n2 + n3 + n4);
	}

	float *buildFbm4NoiseTexture2D(int size, int freq, int octaves, int lacunarity, float gain)
	{
		int i, j, k, l;
		float *base = new float[4 * size * size];
		float *data = new float[4 * size * size];
		float c = ((float)freq) / size;

		for (j = 0; j < size; ++j) {
			for (i = 0; i < size; ++i) {
				base[4 * (i + j*size)] = cnoise(i*c + 0.33f, j*c + 0.33f, freq);
				base[4 * (i + j*size) + 1] = cnoise(-i*c + 0.33f, j*c + 0.33f, freq);
				base[4 * (i + j*size) + 2] = cnoise(i*c + 0.33f, -j*c + 0.33f, freq);
				base[4 * (i + j*size) + 3] = cnoise(-i*c + 0.33f, -j*c + 0.33f, freq);
			}
		}

		float max = 0;
		for (j = 0; j < size; ++j) {
			for (i = 0; i < size; ++i) {
				for (k = 0; k < 4; ++k) {
					int ip = i;
					int jp = j;
					float amp = 1;
					float f = 0;
					for (l = 0; l < octaves; ++l) {
						f += base[4 * (ip + jp * size) + k] * amp;
						ip = (ip * lacunarity) % size;
						jp = (jp * lacunarity) % size;
						amp *= gain;
					}
					data[4 * (i + j * size) + k] = f;
					max = std::max<float>(fabs(f), max);
				}
			}
		}

		for (j = 0; j < size; ++j) {
			for (i = 0; i < size; ++i) {
				for (k = 0; k < 4; ++k) {
					data[4 * (i + j*size) + k] = 0.5f + data[4 * (i + j*size) + k] / max * 0.5f;
				}
			}
		}

		delete[] base;
		return data;
	}

	float *buildFbm1NoiseTexture3D(int size, int freq, int octaves, int lacunarity, float gain)
	{
		int i, j, k, l;
		float *base = new float[size * size * size];
		float *data = new float[size * size * size];
		float c = ((float)freq) / size;

		for (k = 0; k < size; ++k) {
			for (j = 0; j < size; ++j) {
				for (i = 0; i < size; ++i) {
					base[(i + j*size + k*size*size)] = cnoise(i*c + 0.33f, j*c + 0.33f, k*c + 0.33f, freq);
				}
			}
		}

		float max = 0;
		for (k = 0; k < size; ++k) {
			for (j = 0; j < size; ++j) {
				for (i = 0; i < size; ++i) {
					int ip = i;
					int jp = j;
					int kp = k;
					float amp = 1;
					float f = 0;
					for (l = 0; l < octaves; ++l) {
						f += base[ip + jp * size + kp * size * size] * amp;
						ip = (ip * lacunarity) % size;
						jp = (jp * lacunarity) % size;
						kp = (kp * lacunarity) % size;
						amp *= gain;
					}
					data[i + j * size + k * size * size] = f;
					max = std::max<float>(fabs(f), max);
				}
			}
		}

		for (k = 0; k < size; ++k) {
			for (j = 0; j < size; ++j) {
				for (i = 0; i < size; ++i) {
					data[i + j*size + k*size*size] = 0.5f + data[i + j*size + k*size*size] / max * 0.5f;
				}
			}
		}

		delete[] base;
		return data;
	}

}