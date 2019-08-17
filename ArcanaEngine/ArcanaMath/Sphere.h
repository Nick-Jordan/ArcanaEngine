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

		/** \brief Mutator for the sphere radius.
		 */
		void setRadius(T radius) { _radius = radius; }

		/** \brief Accessor for the sphere center.
		 */

		void setCenter(const Vector3<T>& center) { _center = center; };

		void set(const Vector3<T>& center, T radius) { _center = center; _radius = radius; }

		bool isEmpty() const { return _radius == (T)0; }

		void merge(const Sphere<T>& sphere)
		{
			if (sphere.isEmpty())
				return;

			T vx = _center.x - sphere._center.x;
			T vy = _center.y - sphere._center.y;
			T vz = _center.z - sphere._center.z;
			T d = sqrt(vx * vx + vy * vy + vz * vz);

			if (d <= (sphere._radius - _radius))
			{
				_center = sphere._center;
				_radius = sphere._radius;
				return;
			}
			else if (d <= (_radius - sphere._radius))
			{
				return;
			}

			if (d != (T)0)
			{
				T dI = (T)1 / d;
				vx *= dI;
				vy *= dI;
				vz *= dI;

				T r = (_radius + sphere._radius + d) / (T)2;

				T scaleFactor = (r - sphere._radius);
				vx = vx * scaleFactor + sphere._center.x;
				vy = vy * scaleFactor + sphere._center.y;
				vz = vz * scaleFactor + sphere._center.z;

				_center.x = vx;
				_center.y = vy;
				_center.z = vz;
				_radius = r;
			}
		}


		bool operator==(const Sphere<T>& rhs)
		{
			return _center == rhs._center && _radius == rhs._radius;
		}

		bool operator!=(const Sphere<T>& rhs)
		{
			return !((*this) == rhs);
		}

	private:

		Vector3<T> _center;
		T _radius;
	};

	typedef Sphere<float> Spheref;
	typedef Sphere<double> Sphered;
	typedef Sphere<int> Spherei;
}

#endif // !SPHERE_H_