#ifndef PLANE_H_
#define PLANE_H_

#include "Matrix4.h"
#include "Vector3.h"
#include "Vector4.h"

namespace Arcana
{
	/** \brief A plane defined with a point and normal.
	 *
	 *  The plane can be defined with a point/normal, point/distance, or distance/normal.
	 */
	template<typename T>
	class Plane
	{
	public:

		/** \brief Plane default constructor.
		 */
		Plane();

		/** \brief Plane point/normal constructor.
		 */
		Plane(const Vector3<T>& point, const Vector3<T>& normal);

		/** \brief Plane point/distance constructor.
		 */
		Plane(const Vector3<T>& point, T distance);

		/** \brief Plane distance/normal constructor.
		 */
		Plane(T distance, const Vector3<T>& normal);

		/** \brief Plane destructor.
		 */
		~Plane();

		/** \brief Accessor for the plane normal.
		 */
		const Vector3<T>& getNormal() const;

		/** \brief Mutator for the plane normal.
		 */
		void setNormal(const Vector3<T>& normal);

		/** \brief Accessor for the plane distance.
		 */
		T getDistance() const;

		/** \brief Mutator for the plane distance.
		 */
		void setDistance(T distance);

		/** \brief Returns true if the plane is facing the direction.
		 *
		 *  Returns true if normal * direction >= 0;
		 */
		bool isFacing(const Vector3<T>& direction) const;

		/** \brief Returns the closest distance to a point.
		 *
		 *  Returns |normal * point + distance|;
		 */
		T distance(const Vector3<T>& point) const;

		/** \brief Returns the closest signed distance to a point.
		 *
		 *  Returns normal * point + distance;
		 */
		T signedDistance(const Vector3<T>& point) const;

		/** \brief Returns true if the two planes are parallel.
		 */
		bool isParallel(const Plane<T>& plane) const;

		/** \brief Sets the plane by point/distance.
		 */
		void set(const Vector3<T>& point, T distance);

		/** \brief Sets the plane by distance/normal.
		 */
		void set(T distance, const Vector3<T>& normal);

		/** \brief Sets the plane by point/normal.
		 */
		void set(const Vector3<T>& point, const Vector3<T>& normal);

		/** \brief Copies another plane's values.
		 */
		void set(const Plane<T>& plane);

		/** \brief Normalizes the plane's normal.
		 */
		void normalize();

		/** \brief Transforms the plane by a matrix.
		 */
		void transform(const Matrix4<T>& matrix);

	private:

		Vector3<T> _normal;  ///< The plane normal.
		T _distance;		 ///< The distance from the origin.
	};

	typedef Plane<float> Planef;
	typedef Plane<double> Planed;
	typedef Plane<int> Planei;
}

#include "Plane.inl"

#endif // !PLANE_H_