#ifndef AXIS_ALIGNED_BOUNDING_BOX_H_
#define AXIS_ALIGNED_BOUNDING_BOX_H_

#include "ArcanaMath.h"
#include "Vector2.h"
#include "Vector3.h"

namespace Arcana
{
	template<typename T>
	class AxisAlignedBoundingBox
	{
	public:

		AxisAlignedBoundingBox();

		AxisAlignedBoundingBox(const Vector3<T>& min, const Vector3<T>& max);

		AxisAlignedBoundingBox(T minX, T minY, T minZ, T maxX, T maxY, T maxZ);

		~AxisAlignedBoundingBox();


		void set(const Vector3<T>& min, const Vector3<T>& max);

		void set(T minX, T minY, T minZ, T maxX, T maxY, T maxZ);

		const Vector3<T> getMin() const;

		const Vector3<T> getMax() const;

		void setMin(const Vector3<T>& min);

		void setMax(const Vector3<T>& max);

		Vector2<T> getSize() const;

		T getWidth() const;

		T getHeight() const;

		T getDepth() const;

		bool contains(Vector3<T> point) const;

		bool contains(T x, T y, T z) const;

		bool contains(AxisAlignedBoundingBox<T> boundingBox) const;

		bool intersects(AxisAlignedBoundingBox<T> boundingBox) const;

		bool isEmpty() const;

	private:

		Vector3<T> _min;
		Vector3<T> _max;
	};

	typedef AxisAlignedBoundingBox<float> AxisAlignedBoundingBoxf;
	typedef AxisAlignedBoundingBox<double> AxisAlignedBoundingBoxd;
	typedef AxisAlignedBoundingBox<int> AxisAlignedBoundingBoxi;
}

#include "AxisAlignedBoundingBox.inl"

#endif // !AXIS_ALIGNED_BOUNDING_BOX_H_

