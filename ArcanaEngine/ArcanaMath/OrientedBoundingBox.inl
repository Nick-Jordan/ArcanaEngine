
namespace Arcana
{
	template<typename T>
	OrientedBoundingBox<T>::OrientedBoundingBox() 
		: _min(Vector3<T>::zero()), _max(Vector3<T>::zero()), _transformation(Matrix4<T>::IDENTITY)
	{

	}

	template<typename T>
	OrientedBoundingBox<T>::OrientedBoundingBox(const Vector3<T>& min, const Vector3<T>& max, const Quaternion<T>& rotation)
		: _min(min), _max(max), _rotation(rotation)
	{

	}

	template<typename T>
	OrientedBoundingBox<T>::OrientedBoundingBox(T minX, T minY, T minZ, T maxX, T maxY, T maxZ, const Quaternion<T>& rotation)
		: _min(minX, minY, minZ), _max(maxX, maxY, maxZ), _rotation(rotation)
	{

	}

	template<typename T>
	OrientedBoundingBox<T>::~OrientedBoundingBox()
	{

	}


	template<typename T>
	void OrientedBoundingBox<T>::set(const Vector3<T>& min, const Vector3<T>& max, const Quaternion<T>& rotation)
	{
		_min = min;
		_max = max;
		_rotation = rotation;
	}

	template<typename T>
	void OrientedBoundingBox<T>::set(T minX, T minY, T minZ, T maxX, T maxY, T maxZ, const Quaternion<T>& rotation)
	{
		_min = Vector3<T>(minX, minY, minZ);
		_max = Vector3<T>(maxX, maxY, maxZ);
		_rotation = rotation;
	}

	template<typename T>
	const Vector3<T> OrientedBoundingBox<T>::getMin() const
	{
		return _min;
	}

	template<typename T>
	const Vector3<T> OrientedBoundingBox<T>::getMax() const
	{
		return _max;
	}

	template<typename T>
	const Quaternion<T>& OrientedBoundingBox<T>::getRotation() const
	{
		return _rotation;
	}

	template<typename T>
	void OrientedBoundingBox<T>::setMin(const Vector3<T>& min)
	{
		_min = min;
	}

	template<typename T>
	void OrientedBoundingBox<T>::setMax(const Vector3<T>& max)
	{
		_max = max;
	}

	template<typename T>
	void OrientedBoundingBox<T>::setRotation(const Quaternion<T>& rotation)
	{
		_rotation = rotation;
	}

	template<typename T>
	Vector3<T> OrientedBoundingBox<T>::getSize() const
	{
		return _max - _min;
	}

	template<typename T>
	T OrientedBoundingBox<T>::getWidth() const
	{
		return _max.x - _min.x;
	}

	template<typename T>
	T OrientedBoundingBox<T>::getHeight() const
	{
		return _max.y - _min.y;
	}

	template<typename T>
	T OrientedBoundingBox<T>::getDepth() const
	{
		return _max.z - _min.z;
	}

	template<typename T>
	bool OrientedBoundingBox<T>::contains(Vector3<T> point) const
	{
		point = point * _rotation.inverse();

		T x = point.x;
		T y = point.y;
		T z = point.z;

		return x >= _min.x && x <= _max.x && y >= _min.y && y <= _max.y && z >= _min.z && z <= _max.z;
	}

	template<typename T>
	bool OrientedBoundingBox<T>::contains(T x, T y, T z) const
	{
		return contains(Vector3<T>(x, y, z));
	}

	template<typename T>
	bool OrientedBoundingBox<T>::contains(OrientedBoundingBox<T> boundingBox) const
	{
		/*return boundingBox._min.x >= _min.x
			&& boundingBox._min.y >= _min.y
			&& boundingBox._min.z >= _min.z
			&& boundingBox._max.x <= _max.x
			&& boundingBox._max.y <= _max.y
			&& boundingBox._max.z <= _max.z;*/
		return false;
	}

	template<typename T>
	bool OrientedBoundingBox<T>::intersects(OrientedBoundingBox<T> boundingBox) const
	{
		/*return (_min.x < boundingBox._max.x) && (_max.x > boundingBox._min.x) &&
			(_min.y < boundingBox._max.y) && (_max.y > boundingBox._min.y) &&
			(_min.z < boundingBox._max.z) && (_max.z > boundingBox._min.z);*/
		return false;
	}

	template<typename T>
	bool OrientedBoundingBox<T>::isEmpty() const
	{
		return _min == _max;
	}
}