#ifndef NOISE_H_
#define NOISE_H_

#include "PCGDefines.h"

#include "Types.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Array.h"

namespace Arcana
{
	class ARCANA_PCG_API Noise
	{
	public:

		enum Type
		{
			BaseNoise,
			Squared,
			Cubed,
			RidgedNoise,
			AbsNoise,
			SquaredNoise,
			CubedNoise,
			CellularNoise,
			CellularSquaredNoise,
			CellularCubedNoise,
			Constant,
			PassThrough
		};

		enum Operation
		{
			Add,
			Subtract,
			Multiply,
			Divide
		};

		struct FunctionProperties
		{
			Type function = BaseNoise;
			Operation operation = Add;
			int32 octaves = 1;
			double persistence = 1.0;
			double frequency = 1.0;
			double low = -1.0;
			double high = 1.0;
			Vector2d clamp = Vector2d(0.0);
			Array<FunctionProperties> children;
		};

		struct Base
		{
			double base = 0.0;
			Array<FunctionProperties> functions;
		};

		static Vector2d cellular(const Vector3d& point);

		static double noise(const int32 octaves,
			const double persistence,
			const double frequency,
			const double x,
			const double y);

		static double noise(const int32 octaves,
			const double persistence,
			const double frequency,
			const double x,
			const double y,
			const double z);

		static double noise(const int32 octaves,
			const double persistence,
			const double frequency,
			const double x,
			const double y,
			const double z,
			const double w);

		static double simplexNoise(const double x, const double y);
		static double simplexNoise(const double x, const double y, const double z);
		static double simplexNoise(const double x, const double y, const double z, const double w);

		static double noiseScaled(const int32 octaves, const double persistence, const double frequency, const double x, const double y, const double low, const double high);
		static double noiseScaled(const int32 octaves, const double persistence, const double frequency, const double x, const double y, const double z, const double low, const double high);
		static double noiseScaled(const int32 octaves, const double persistence, const double frequency, const double x, const double y, const double z, const double w, const double low, const double high);

		static double simplexScaled(const double x, const double y, const double low, const double high);
		static double simplexScaled(const double x, const double y, const double z, const double low, const double high);
		static double simplexScaled(const double x, const double y, const double z, const double w, const double low, const double high);


		static Vector3d permute(const Vector3d& x);

		static double performOperation(const Operation& operation, double a, double b);

		static void evaluateNoise(const Vector3d& position,
			const Array<FunctionProperties>& functions,
			double* modifier,
			const Operation& operation,
			double& height);

		static void evaluateNoise(const Vector3d& position,
			const Base& base,
			double& height);

		static const double grad3[12][3];
		static const double grad4[32][4];
		static const int32 perm[512];
		static const int32 simplex[64][4];
	};
}

#endif // !NOISE_H_