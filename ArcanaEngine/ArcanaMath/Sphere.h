#ifndef SPHERE_H_
#define SPHERE_H_

#include "Ellipsoid.h"

namespace Arcana
{
	/** \brief A special case of an Ellipsoid in which all axes are equal.
	 */
	template<typename T>
	class Sphere : public Ellipsoid
	{
	public:

		/** \brief Sphere default constructor.
		 *
		 *  Creates an empty sphere.
		 */
		Sphere() : Ellipsoid<T>() {};

		/** \brief Sphere center/radius constructor.
		 */
		Sphere(const Vector3<T>& center, T radius) : Ellipsoid<T>(center, Vector3<T>(radius, radius, radius)) {};

		/** \brief Sphere destructor.
		 */
		~Sphere(){};

		/** \brief Accessor for the sphere radius.
		 */
		T getRadius() const { return getAxes().x; };
	};

	typedef Sphere<float> Spheref;
	typedef Sphere<double> Sphered;
	typedef Sphere<int> Spherei;
}

#endif // !SPHERE_H_