#ifndef RANDOM_H_
#define RANDOM_H_

#include "Vector2.h"
#include "Vector3.h"

namespace Arcana
{
	template<typename T>
	class Random
	{
	public:

		static T random(T min = (T)0, T max = (T)1);

		static Vector3<T> random(const Vector3<T>& min, const Vector3<T>& max);

		static T linearRandom(T min, T max);

		static Vector3<T> linearRandom(const Vector3<T>& min, const Vector3<T>& max);

		static T gaussianRandom(T mean, T deviation);

		static Vector2<T> circularRandom(T radius);

		static Vector3<T> sphericalRandom(T radius);

		static Vector2<T> diskRandom(T radius);

		static Vector3<T> sphericalVolumeRandom(T radius);
	};
}

#include "Random.inl"

#endif // !RANDOM_H_