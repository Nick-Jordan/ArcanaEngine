#ifndef RAY3_H_
#define RAY3_H_

#include "Vector3.h"
#include "Matrix3.h"
#include "Matrix4.h"

namespace Arcana
{
	template<typename T>
	class Ray3
	{
	public:

		Ray3();

		Ray3(const Vector3<T>& origin, const Vector3<T>& direction);

		Ray3(T originX, T originY, T originZ, T directionX, T directionY, T directionZ);

		Ray3(const Ray3<T>& copy);

		~Ray3();

		const Vector3<T>& getOrigin() const;

		void setOrigin(const Vector3<T>& origin);

		void setOrigin(T originX, T originY, T originZ);

		const Vector3<T>& getDirection() const;

		void setDirection(const Vector3<T>& direction);

		void setDirection(T directionX, T directionY, T directionZ);

		void set(const Vector3<T>& origin, const Vector3<T>& direction);

		void set(T originX, T originY, T originZ, T directionX, T directionY, T directionZ);

		void normalize();

		void transform(const Matrix4<T>& matrix);

		void transform(const Matrix3<T>& matrix);

		Ray3<T>& operator*=(const Matrix4<T>& matrix);

		Ray3<T>& operator*=(const Matrix3<T>& matrix);

		Ray3<T>& operator*=(T scale);

		Ray3<T>& operator*=(const Vector3<T>& scale);

		Ray3<T>& operator+=(const Vector3<T>& offset);

		Ray3<T>& operator=(const Ray3<T>& ray);

	private:

		Vector3<T> _origin;
		Vector3<T> _direction;
	};

	typedef Ray3<float> Ray3f;
	typedef Ray3<double> Ray3d;
	typedef Ray3<int> Ray3i;
}

#include "Ray3.inl"

#endif // !RAY3_H_