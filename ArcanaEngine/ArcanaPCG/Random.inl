#include <random>

namespace Arcana
{
	template<typename T>
	T Random<T>::random(T min, T max)
	{
		std::random_device rd;
		std::mt19937 eng(rd());
		std::uniform_real_distribution<T> distr(min, max);

		return distr(eng);
	}

	template<typename T>
	Vector3<T> Random<T>::random(const Vector3<T>& min, const Vector3<T>& max)
	{
		return Vector3<T>(
			random(min.x, max.x), 
			random(min.y, max.y), 
			random(min.z, max.z)
			);
	}

	template<typename T>
	T Random<T>::linearRandom(T min, T max)
	{
		return (T)0;
	}

	template<typename T>
	Vector3<T> Random<T>::linearRandom(const Vector3<T>& min, const Vector3<T>& max)
	{
		return Vector3<T>::zero();	
	}

	template<typename T>
	T Random<T>::gaussianRandom(T mean, T deviation)
	{
		return (T)0;
	}

	template<typename T>
	Vector2<T> Random<T>::circularRandom(T radius)
	{
		return Vector2<T>::zero();
	}

	template<typename T>
	Vector3<T> Random<T>::sphericalRandom(T radius)
	{
		return Vector3<T>::zero();
	}

	template<typename T>
	Vector2<T> Random<T>::diskRandom(T radius)
	{
		return Vector2<T>::zero();
	}

	template<typename T>
	Vector3<T> Random<T>::sphericalVolumeRandom(T radius)
	{
		return Vector3<T>::zero();
	}
}