#ifndef ELLIPSOID_H_
#define ELLIPSOID_H_

#include "Vector3.h"

namespace Arcana
{
	/** \brief An ellipsoid created from a center position and axes.
	 */
	template<typename T>
	class Ellipsoid
	{
	public:

		/** \brief Ellipsoid default constructor.
		 *
		 *  Creates an empty ellipsoid at the origin.
		 */
		Ellipsoid();

		/** \brief Ellipsoid center and axes constructor.
		 */
		Ellipsoid(const Vector3<T>& center, const Vector3<T>& axes);

		/** \brief Ellipsoid destructor
		 */
		~Ellipsoid();

		/** \brief Accessor for the ellipsoid center
		 */
		const Vector3<T>& getCenter() const;

		/** \brief Accessor for the ellipsoid axes
		 */
		const Vector3<T>& getAxes() const;

		/** \brief Returns true if the ellipsoid contains the point.
		 */
		bool contains(const Vector3<T>& point);

		/** \brief Returns true if the ellipsoid contains the point.
		 */
-		bool contains(T x, T y, T z);

	private:

		Vector3<T> _center;  ///< The ellipsoid center point.
		Vector3<T> _axes;    ///< The ellipsoid axes.
	};

	typedef Ellipsoid<float> Ellipsoidf;
	typedef Ellipsoid<double> Ellipsoidd;
	typedef Ellipsoid<int> Ellipsoidi;
}

#include "Ellipsoid.inl"

#endif // !ELLIPSOID_H_