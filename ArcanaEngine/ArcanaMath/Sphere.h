#ifndef SPHERE_H_
#define SPHERE_H_

#include "Ellipsoid.h"

namespace Arcana
{
	template<typename T>
	class Sphere : public Ellipsoid
	{
	public:

		Sphere() : Ellipsoid<T>() {};

		Sphere(const Vector3<T>& center, T radius) : Ellipsoid<T>(center, Vector3<T>(radius, radius, radius)) {};

		~Sphere(){};

		T getRadius() const { return getAxes().x; };
	};

	typedef Sphere<float> Spheref;
	typedef Sphere<double> Sphered;
	typedef Sphere<int> Spherei;
}

#endif // !SPHERE_H_