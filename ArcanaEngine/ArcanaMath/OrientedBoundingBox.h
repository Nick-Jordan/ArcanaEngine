#ifndef ORIENTED_BOUNDING_BOX_H_
#define ORIENTED_BOUNDING_BOX_H_

#include "ArcanaMath.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Matrix4.h"

namespace Arcana
{
	/** \brief A bounding box aligned that is optionally oriented
	 *
	 *  Defined as a minimum point and a maximum point and a transformation.
	 *  Includes simple contains and intersection methods.
	 */
	template<typename T>
	class OrientedBoundingBox
	{
	public:

		/** \brief OrientedBoundingBox default constructor.
		 *
		 *  Creates an empty bounding box.
		 */
		OrientedBoundingBox();

		/** \brief OrientedBoundingBox constructor with vector arguments.
		 */
		OrientedBoundingBox(const Vector3<T>& min, const Vector3<T>& max, const Quaternion<T>& rotation = Quaternion<T>::IDENTITY);

		/** \brief OrientedBoundingBox constructor with scalar arguments.
		 */
		OrientedBoundingBox(T minX, T minY, T minZ, T maxX, T maxY, T maxZ, const Quaternion<T>& rotation = Quaternion<T>::IDENTITY);

		/** \brief OrientedBoundingBox destructor.
		 */
		~OrientedBoundingBox();

		/** \brief Sets the minimum and maximum bounding box points with vectors.
		 */
		void set(const Vector3<T>& min, const Vector3<T>& max, const Quaternion<T>& rotation = Quaternion<T>::IDENTITY);

		/** \brief Sets the minimum and maximum bounding box points with scalars.
		 */
		void set(T minX, T minY, T minZ, T maxX, T maxY, T maxZ, const Quaternion<T>& rotation = Quaternion<T>::IDENTITY);

		/** \brief Returns the minimum point.
		 */
		const Vector3<T> getMin() const;

		/** \brief Returns the maximum point.
		 */
		const Vector3<T> getMax() const;

		/** \brief Returns the rotation.
		 */
		const Quaternion<T>& getRotation() const;

		/** \brief Sets the minimum point.
		 */
		void setMin(const Vector3<T>& min);

		/** \brief Sets the maximum point.
		 */
		void setMax(const Vector3<T>& max);

		/** \brief Sets the rotation.
		 */
		void setRotation(const Quaternion<T>& rotation);

		/** \brief Returns the size of the bounding box.
		 *
		 *  Subtracts the minimum point from the maximum point.
		 */
		Vector3<T> getSize() const;

		/** \brief Returns the bounding box width.
		 */
		T getWidth() const;

		/** \brief Returns the bounding box height.
		 */
		T getHeight() const;

		/** \brief Returns the bounding box depth.
		 */
		T getDepth() const;

		/** \brief Returns true if the bounding box contains the point.
		 */
		bool contains(Vector3<T> point) const;

		/** \brief Returns true if the bounding box contains the point.
		 */
		bool contains(T x, T y, T z) const;

		/** \brief Returns true if this bounding box contains the argument bounding box.
		 *
		 *  Checks if the minimum 'boundingBox' point is greater than this box's minimum.
		 *  Checks if the maximum 'boundingBox' point is less than this box's maximum.
		 */
		bool contains(OrientedBoundingBox<T> boundingBox) const;

		/** \brief Returns true if this bounding box intersects the argument bounding box.
		 */
		bool intersects(OrientedBoundingBox<T> boundingBox) const;

		/** \brief Returns true if the minimum point equals the maximum point.
		 */
		bool isEmpty() const;

	private:

		Vector3<T> _min;  ///< The minimum point.
		Vector3<T> _max;  ///< The maximum point.

		Quaternion<T> _rotation;
	};

	typedef OrientedBoundingBox<float> OrientedBoundingBoxf;
	typedef OrientedBoundingBox<double> OrientedBoundingBoxd;
	typedef OrientedBoundingBox<int> OrientedBoundingBoxi;

	template<typename T>
	using OBB = OrientedBoundingBox<T>;
}

#include "OrientedBoundingBox.inl"

#endif // !ORIENTED_BOUNDING_BOX_H_

