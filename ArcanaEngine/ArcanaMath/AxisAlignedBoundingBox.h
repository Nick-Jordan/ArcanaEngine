#ifndef AXIS_ALIGNED_BOUNDING_BOX_H_
#define AXIS_ALIGNED_BOUNDING_BOX_H_

#include "ArcanaMath.h"
#include "Vector2.h"
#include "Vector3.h"

namespace Arcana
{
	/** \brief A bounding box aligned with the axes of the coordinate system.
	 *
	 *  Defined as a minimum point and a maximum point.
	 *  Includes simple contains and intersection methods.
	 */
	template<typename T>
	class AxisAlignedBoundingBox
	{
	public:

		/** \brief AxisAlignedBoundingBox default constructor.
		 *
		 *  Creates an empty bounding box. 
		 */
		AxisAlignedBoundingBox();

		/** \brief AxisAlignedBoundingBox constructor with vector arguments.
		 */
		AxisAlignedBoundingBox(const Vector3<T>& min, const Vector3<T>& max);

		/** \brief AxisAlignedBoundingBox constructor with scalar arguments.
		 */
		AxisAlignedBoundingBox(T minX, T minY, T minZ, T maxX, T maxY, T maxZ);

		/** \brief AxisAlignedBoundingBox destructor.
		 */
		~AxisAlignedBoundingBox();

		/** \brief Sets the minimum and maximum bounding box points with vectors.
		 */
		void set(const Vector3<T>& min, const Vector3<T>& max);

		/** \brief Sets the minimum and maximum bounding box points with scalars.
		 */
		void set(T minX, T minY, T minZ, T maxX, T maxY, T maxZ);

		/** \brief Returns the minimum point.
		 */
		const Vector3<T>& getMin() const;

		/** \brief Returns the maximum point.
		 */
		const Vector3<T>& getMax() const;

		/** \brief Sets the minimum point.
		 */
		void setMin(const Vector3<T>& min);

		/** \brief Sets the maximum point.
		 */
		void setMax(const Vector3<T>& max);

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
		bool contains(const Vector3<T>& point) const;

		/** \brief Returns true if the bounding box contains the point.
		 */
		bool contains(T x, T y, T z) const;

		/** \brief Returns true if this bounding box contains the argument bounding box.
		 * 
		 *  Checks if the minimum 'boundingBox' point is greater than this box's minimum.
		 *  Checks if the maximum 'boundingBox' point is less than this box's maximum.
		 */
		bool contains(const AxisAlignedBoundingBox<T>& boundingBox) const;

		/** \brief Returns true if this bounding box intersects the argument bounding box.
		 */
		bool intersects(const AxisAlignedBoundingBox<T>& boundingBox) const;

		/** \brief Returns true if the minimum point equals the maximum point.
		 */
		bool isEmpty() const;

	private:

		Vector3<T> _min;  ///< The minimum point.
		Vector3<T> _max;  ///< The maximum point.
	};

	typedef AxisAlignedBoundingBox<float> AxisAlignedBoundingBoxf;
	typedef AxisAlignedBoundingBox<double> AxisAlignedBoundingBoxd;
	typedef AxisAlignedBoundingBox<int> AxisAlignedBoundingBoxi;

	template<typename T>
	using AABB = AxisAlignedBoundingBox<T>;
}

#include "AxisAlignedBoundingBox.inl"

#endif // !AXIS_ALIGNED_BOUNDING_BOX_H_