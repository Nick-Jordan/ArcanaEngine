
namespace Arcana
{
	template<typename T>
	AxisAlignedBoundingBox<T>::AxisAlignedBoundingBox() : _min(Vector3<T>::zero()), _max(Vector3<T>::zero())
	{

	}

	template<typename T>
	AxisAlignedBoundingBox<T>::AxisAlignedBoundingBox(const Vector3<T>& min, const Vector3<T>& max)
		: _min(min), _max(max)
	{

	}

	template<typename T>
	AxisAlignedBoundingBox<T>::AxisAlignedBoundingBox(T minX, T minY, T minZ, T maxX, T maxY, T maxZ)
		: _min(minX, minY, minZ), _max(maxX, maxY, maxZ)
	{

	}

	template<typename T>
	AxisAlignedBoundingBox<T>::~AxisAlignedBoundingBox()
	{

	}


	template<typename T>
	void AxisAlignedBoundingBox<T>::set(const Vector3<T>& min, const Vector3<T>& max)
	{
		_min = min;
		_max = max;
	}

	template<typename T>
	void AxisAlignedBoundingBox<T>::set(T minX, T minY, T minZ, T maxX, T maxY, T maxZ)
	{
		_min = Vector3<T>(minX, minY, minZ);
		_max = Vector3<T>(maxX, maxY, maxZ);
	}

	template<typename T>
	const Vector3<T>& AxisAlignedBoundingBox<T>::getMin() const
	{
		return _min;
	}

	template<typename T>
	const Vector3<T>& AxisAlignedBoundingBox<T>::getMax() const
	{
		return _max;
	}

	template<typename T>
	void AxisAlignedBoundingBox<T>::setMin(const Vector3<T>& min)
	{
		_min = min;
	}

	template<typename T>
	void AxisAlignedBoundingBox<T>::setMax(const Vector3<T>& max)
	{
		_max = max;
	}

	template<typename T>
	Vector3<T> AxisAlignedBoundingBox<T>::getSize() const
	{
		return _max - _min;
	}

	template<typename T>
	T AxisAlignedBoundingBox<T>::getWidth() const
	{
		return _max.x - _min.x;
	}

	template<typename T>
	T AxisAlignedBoundingBox<T>::getHeight() const
	{
		return _max.y - _min.y;
	}

	template<typename T>
	T AxisAlignedBoundingBox<T>::getDepth() const
	{
		return _max.z - _min.z;
	}

	template<typename T>
	bool AxisAlignedBoundingBox<T>::contains(const Vector3<T>& point) const
	{
		return contains(point.x, point.y, point.z);
	}

	template<typename T>
	bool AxisAlignedBoundingBox<T>::contains(T x, T y, T z) const
	{
		return x >= _min.x && x <= _max.x && y >= _min.y && y <= _max.y && z >= _min.z && z <= _max.z;
	}

	template<typename T>
	bool AxisAlignedBoundingBox<T>::contains(const AxisAlignedBoundingBox<T>& boundingBox) const
	{
		return boundingBox._min.x >= _min.x
			&& boundingBox._min.y >= _min.y
			&& boundingBox._min.z >= _min.z
			&& boundingBox._max.x <= _max.x
			&& boundingBox._max.y <= _max.y
			&& boundingBox._max.z <= _max.z;
	}

	template<typename T>
	bool AxisAlignedBoundingBox<T>::intersects(const AxisAlignedBoundingBox<T>& boundingBox) const
	{
		return (_min.x < boundingBox._max.x) && (_max.x > boundingBox._min.x) &&
			(_min.y < boundingBox._max.y) && (_max.y > boundingBox._min.y) &&
			(_min.z < boundingBox._max.z) && (_max.z > boundingBox._min.z);
	}

	template<typename T>
	bool AxisAlignedBoundingBox<T>::isEmpty() const
	{
		return _min == _max;
	}
}