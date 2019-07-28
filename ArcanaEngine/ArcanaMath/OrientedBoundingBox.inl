
namespace Arcana
{
	template<typename T>
	OrientedBoundingBox<T>::OrientedBoundingBox() 
		: _center(Vector3<T>::zero()), _extents(Vector3<T>::zero()), _transformation(Matrix4<T>::IDENTITY)
	{

	}

	template<typename T>
	OrientedBoundingBox<T>::OrientedBoundingBox(const Vector3<T>& center, const Vector3<T>& extents, const Quaternion<T>& rotation)
		: _center(center), _extents(extents), _rotation(rotation)
	{

	}

	template<typename T>
	OrientedBoundingBox<T>::OrientedBoundingBox(T centerX, T centerY, T centerZ, T extentsX, T extentsY, T extentsZ, const Quaternion<T>& rotation)
		: _center(centerX, centerY, centerZ), _extents(extentsX, extentsY, extentsZ), _rotation(rotation)
	{

	}

	template<typename T>
	OrientedBoundingBox<T>::~OrientedBoundingBox()
	{

	}


	template<typename T>
	void OrientedBoundingBox<T>::set(const Vector3<T>& center, const Vector3<T>& extents, const Quaternion<T>& rotation)
	{
		_center = center;
		_extents = extents;
		_rotation = rotation;
	}

	template<typename T>
	void OrientedBoundingBox<T>::set(T centerX, T centerY, T centerZ, T extentsX, T extentsY, T extentsZ, const Quaternion<T>& rotation)
	{
		_center = Vector3<T>(centerX, centerY, centerZ);
		_extents = Vector3<T>(extentsX, extentsY, extentsZ);
		_rotation = rotation;
	}

	template<typename T>
	const Vector3<T>& OrientedBoundingBox<T>::getCenter() const
	{
		return _center;
	}

	template<typename T>
	const Quaternion<T>& OrientedBoundingBox<T>::getRotation() const
	{
		return _rotation;
	}

	template<typename T>
	void OrientedBoundingBox<T>::setCenter(const Vector3<T>& center)
	{
		_center = center;
	}

	template<typename T>
	void OrientedBoundingBox<T>::setExtents(const Vector3<T>& extents)
	{
		_extents = extents;
	}

	template<typename T>
	void OrientedBoundingBox<T>::setRotation(const Quaternion<T>& rotation)
	{
		_rotation = rotation;
	}

	template<typename T>
	Vector3<T> OrientedBoundingBox<T>::getSize() const
	{
		return _extents * (T) 2;
	}

	template<typename T>
	const Vector3<T>& OrientedBoundingBox<T>::getExtents() const
	{
		return _extents;
	}

	template<typename T>
	T OrientedBoundingBox<T>::getWidth() const
	{
		return _extents.x * (T) 2;
	}

	template<typename T>
	T OrientedBoundingBox<T>::getHeight() const
	{
		return _extents.y * (T)2;
	}

	template<typename T>
	T OrientedBoundingBox<T>::getDepth() const
	{
		return _extents.z * (T)2;
	}

	template<typename T>
	bool OrientedBoundingBox<T>::contains(const Vector3<T>& point) const
	{
		Vector3<T> p = _rotation.inverse().rotate(point);

		T x = p.x;
		T y = p.y;
		T z = p.z;

		Vector3<T> min = _center - _extents;
		Vector3<T> max = _center + _extents;

		return x >= min.x && x <= max.x && y >= min.y && y <= max.y && z >= min.z && z <= max.z;
	}

	template<typename T>
	bool OrientedBoundingBox<T>::contains(T x, T y, T z) const
	{
		return contains(Vector3<T>(x, y, z));
	}

	template<typename T>
	bool OrientedBoundingBox<T>::contains(const OrientedBoundingBox<T>& boundingBox) const
	{
		Vector3<T> v0 = boundingBox._rotation.rotate(boundingBox._center - boundingBox._extents);
		Vector3<T> v1 = boundingBox._rotation.rotate(Vector3<T>(boundingBox._center.x - boundingBox._extents.x, boundingBox._center.y - boundingBox._extents.y, boundingBox._center.z + boundingBox._extents.z));
		Vector3<T> v2 = boundingBox._rotation.rotate(Vector3<T>(boundingBox._center.x + boundingBox._extents.x, boundingBox._center.y - boundingBox._extents.y, boundingBox._center.z + boundingBox._extents.z));
		Vector3<T> v3 = boundingBox._rotation.rotate(Vector3<T>(boundingBox._center.x + boundingBox._extents.x, boundingBox._center.y - boundingBox._extents.y, boundingBox._center.z - boundingBox._extents.z));
		Vector3<T> v4 = boundingBox._rotation.rotate(Vector3<T>(boundingBox._center.x - boundingBox._extents.x, boundingBox._center.y + boundingBox._extents.y, boundingBox._center.z + boundingBox._extents.z));
		Vector3<T> v5 = boundingBox._rotation.rotate(Vector3<T>(boundingBox._center.x + boundingBox._extents.x, boundingBox._center.y + boundingBox._extents.y, boundingBox._center.z + boundingBox._extents.z));
		Vector3<T> v6 = boundingBox._rotation.rotate(Vector3<T>(boundingBox._center.x + boundingBox._extents.x, boundingBox._center.y + boundingBox._extents.y, boundingBox._center.z - boundingBox._extents.z));
		Vector3<T> v7 = boundingBox._rotation.rotate(boundingBox._center + boundingBox._extents);

		return contains(v0) && contains(v1) && contains(v2) && contains(v3)
			&& contains(v4) && contains(v5) && contains(v6) && contains(v7);
	}

	template<typename T>
	bool OrientedBoundingBox<T>::intersects(const OrientedBoundingBox<T>& boundingBox) const
	{
		/*return (_min.x < boundingBox._max.x) && (_max.x > boundingBox._min.x) &&
			(_min.y < boundingBox._max.y) && (_max.y > boundingBox._min.y) &&
			(_min.z < boundingBox._max.z) && (_max.z > boundingBox._min.z);*/
		return false;
	}

	template<typename T>
	bool OrientedBoundingBox<T>::isEmpty() const
	{
		return _extents.isZero();
	}

	template<typename T>
	void OrientedBoundingBox<T>::resize(T dx, T dy, T dz)
	{
		_extents.x += dx;
		_extents.y += dy;
		_extents.z += dz;
	}

	template<typename T>
	void OrientedBoundingBox<T>::resize(const Vector3<T>& ds)
	{
		resize(ds.x, ds.y, ds.z);
	}

	template<typename T>
	void OrientedBoundingBox<T>::offset(T dx, T dy, T dz)
	{
		_center.x += dx;
		_center.y += dy;
		_center.z += dz;
	}

	template<typename T>
	void OrientedBoundingBox<T>::offset(const Vector3<T>& ds)
	{
		offset(ds.x, ds.y, ds.z);
	}
}