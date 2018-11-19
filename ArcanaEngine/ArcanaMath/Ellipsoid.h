#ifndef ELLIPSOID_H_
#define ELLIPSOID_H_

#include "Vector3.h"

namespace Arcana
{
	template<typename T>
	class Ellipsoid
	{
	public:

		Ellipsoid();

		Ellipsoid(const Vector3<T>& center, const Vector3<T>& axes);

		~Ellipsoid();

		const Vector3<T>& getCenter() const;

		const Vector3<T>& getAxes() const;

		bool contains(const Vector3<T>& point);

-		bool contains(T x, T y, T z);

	private:

		Vector3<T> _center;
		Vector3<T> _axes;
	};

	typedef Ellipsoid<float> Ellipsoidf;
	typedef Ellipsoid<double> Ellipsoidd;
	typedef Ellipsoid<int> Ellipsoidi;
}

#include "Ellipsoid.inl"

#endif // !ELLIPSOID_H_