
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
	const T& AxisAlignedBoundingBox<T>::getMinX() const
	{
		return _min.x;
	}

	template<typename T>
	const T& AxisAlignedBoundingBox<T>::getMaxX() const
	{
		return _max.x;
	}

	template<typename T>
	void AxisAlignedBoundingBox<T>::setMinX(const T& min)
	{
		_min.x = min;
	}

	template<typename T>
	void AxisAlignedBoundingBox<T>::setMaxX(const T& max)
	{
		_max.x = max;
	}

	template<typename T>
	const T& AxisAlignedBoundingBox<T>::getMinY() const
	{
		return _min.y;
	}

	template<typename T>
	const T& AxisAlignedBoundingBox<T>::getMaxY() const
	{
		return _max.y;
	}

	template<typename T>
	void AxisAlignedBoundingBox<T>::setMinY(const T& min)
	{
		_min.y = min;
	}

	template<typename T>
	void AxisAlignedBoundingBox<T>::setMaxY(const T& max)
	{
		_max.y = max;
	}

	template<typename T>
	const T& AxisAlignedBoundingBox<T>::getMinZ() const
	{
		return _min.z;
	}

	template<typename T>
	const T& AxisAlignedBoundingBox<T>::getMaxZ() const
	{
		return _max.z;
	}

	template<typename T>
	void AxisAlignedBoundingBox<T>::setMinZ(const T& min)
	{
		_min.z = min;
	}

	template<typename T>
	void AxisAlignedBoundingBox<T>::setMaxZ(const T& max)
	{
		_max.z = max;
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

	template<typename T>
	void AxisAlignedBoundingBox<T>::resize(T dx, T dy, T dz)
	{
		_min.x -= dx;
		_min.y -= dy;
		_min.z -= dz;
		_max.x += dx;
		_max.y += dy;
		_max.z += dz;
	}

	template<typename T>
	void AxisAlignedBoundingBox<T>::resize(const Vector3<T>& ds)
	{
		resize(ds.x, ds.y, ds.z);
	}

	template<typename T>
	void AxisAlignedBoundingBox<T>::offset(T dx, T dy, T dz)
	{
		_min.x += dx;
		_min.y += dy;
		_min.z += dz;
		_max.x += dx;
		_max.y += dy;
		_max.z += dz;
	}

	template<typename T>
	void AxisAlignedBoundingBox<T>::offset(const Vector3<T>& ds)
	{
		offset(ds.x, ds.y, ds.z);
	}

	template<typename T>
	void AxisAlignedBoundingBox<T>::merge(const AxisAlignedBoundingBox<T>& box)
	{
		_min.x = Math::min(_min.x, box._min.x);
		_min.y = Math::min(_min.y, box._min.y);
		_min.z = Math::min(_min.z, box._min.z);
		_max.x = Math::max(_max.x, box._max.x);
		_max.y = Math::max(_max.y, box._max.y);
		_max.z = Math::max(_max.z, box._max.z);
	}

	template<typename T>
	bool AxisAlignedBoundingBox<T>::operator==(const AxisAlignedBoundingBox<T>& rhs)
	{
		return _min == rhs._min && _max == rhs._max;
	}

	template<typename T>
	bool AxisAlignedBoundingBox<T>::operator!=(const AxisAlignedBoundingBox<T>& rhs)
	{
		return !((*this) == rhs);
	}
}