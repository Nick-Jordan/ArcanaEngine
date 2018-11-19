#ifndef PLANE_H_
#define PLANE_H_

#include "Matrix4.h"
#include "Vector3.h"
#include "Vector4.h"

namespace Arcana
{
	template<typename T>
	class Plane
	{
	public:

		Plane();

		Plane(const Vector3<T>& point, const Vector3<T>& normal);

		Plane(const Vector3<T>& point, T distance);

		Plane(T distance, const Vector3<T>& normal);

		~Plane();

		const Vector3<T>& getNormal() const;

		void setNormal(const Vector3<T>& normal);

		T getDistance() const;

		void setDistance(T distance);

		bool isFacing(const Vector3<T>& direction) const;

		T distance(const Vector3<T>& point) const;

		T signedDistance(const Vector3<T>& point) const;

		bool isParallel(const Plane<T>& plane) const;

		void set(const Vector3<T>& point, T distance);

		void set(T distance, const Vector3<T>& normal);

		void set(const Vector3<T>& point, const Vector3<T>& normal);

		void set(const Plane<T>& plane);

		void normalize();

		void transform(const Matrix4<T>& matrix);

	private:

		Vector3<T> _normal;
		T _distance;
	};

	typedef Plane<float> Planef;
	typedef Plane<double> Planed;
	typedef Plane<int> Planei;
}

#include "Plane.inl"

#endif // !PLANE_H_