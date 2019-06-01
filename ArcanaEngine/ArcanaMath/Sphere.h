#ifndef SPHERE_H_
#define SPHERE_H_

#include "Ellipsoid.h"

namespace Arcana
{
	/** \brief A special case of an Ellipsoid in which all axes are equal.
	 */
	template<typename T>
	class Sphere
	{
	public:

		/** \brief Sphere default constructor.
		 *
		 *  Creates an empty sphere.
		 */
		Sphere() : _center(Vector3<T>::zero()), _radius((T)0) {};

		/** \brief Sphere center/radius constructor.
		 */
		Sphere(const Vector3<T>& center, T radius) : _center(center), _radius(radius) {};

		/** \brief Sphere destructor.
		 */
		~Sphere(){};

		/** \brief Accessor for the sphere radius.
		 */
		T getRadius() const { return _radius; };

		/** \brief Accessor for the sphere center.
		 */

		const Vector3<T>& getCenter() const { return _center; };

	private:

		Vector3<T> _center;
		T _radius;
	};

	typedef Sphere<float> Spheref;
	typedef Sphere<double> Sphered;
	typedef Sphere<int> Spherei;
}

#endif // !SPHERE_H_