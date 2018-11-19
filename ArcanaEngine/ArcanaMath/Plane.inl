namespace Arcana
{
	template<typename T>
	Plane<T>::Plane() : _normal(Vector3<T>::zero()), _distance((T)0)
	{

	}

	template<typename T>
	Plane<T>::Plane(const Vector3<T>& point, const Vector3<T>& normal)
	{
		set(point, normal);
	}

	template<typename T>
	Plane<T>::Plane(const Vector3<T>& point, T distance)
	{
		set(point, distance);
	}

	template<typename T>
	Plane<T>::Plane(T distance, const Vector3<T>& normal)
	{
		set(distance, normal);
	}

	template<typename T>
	Plane<T>::~Plane()
	{

	}

	template<typename T>
	const Vector3<T>& Plane<T>::getNormal() const
	{
		return _normal;
	}

	template<typename T>
	void Plane<T>::setNormal(const Vector3<T>& normal)
	{
		_normal = normal;
		normalize();
	}

	template<typename T>
	T Plane<T>::getDistance() const
	{
		return _distance;
	}

	template<typename T>
	void Plane<T>::setDistance(T distance)
	{
		_distance = distance;
	}

	template<typename T>
	bool Plane<T>::isFacing(const Vector3<T>& direction) const
	{
		return Vector3<T>::dot(_normal, direction) <= (T)0;
	}

	template<typename T>
	T Plane<T>::distance(const Vector3<T>& point) const
	{
		return (T)abs(_normal.x * point.x + _normal.y * point.y + _normal.z * point.z + _distance);
	}

	template<typename T>
	T Plane<T>::signedDistance(const Vector3<T>& point) const
	{
		return Vector3<T>::dot(point, _normal) + _distance;
	}

	template<typename T>
	bool Plane<T>::isParallel(const Plane<T>& plane) const
	{
		return (_normal.y * plane._normal.z) - (_normal.z * plane._normal.y) == (T)0 
			&& (_normal.z * plane._normal.x) - (_normal.x * plane._normal.z) == (T)0 
			&& (_normal.x * plane._normal.y) - (_normal.y * plane._normal.x) == (T)0;
	}

	template<typename T>
	void Plane<T>::set(const Vector3<T>& point, T distance)
	{
		_distance = distance;
		//normal?
	}

	template<typename T>
	void Plane<T>::set(T distance, const Vector3<T>& normal)
	{
		_normal = normal;
		_distance = distance;
		normalize();
	}

	template<typename T>
	void Plane<T>::set(const Vector3<T>& point, const Vector3<T>& normal)
	{
		_normal = normal;
		_distance = -Vector3<T>::dot(point, _normal);
		normalize();
	}

	template<typename T>
	void Plane<T>::set(const Plane<T>& plane)
	{
		_normal = plane._normal;
		_distance = plane._distance;
		normalize();
	}

	template<typename T>
	void Plane<T>::normalize()
	{
		if (_normal.isZero())
		{
			return;
		}

		T normalizeFactor = (T)1 / _normal.magnitude();

		if (normalizeFactor != (T)1)
		{
			_normal *= normalizeFactor;
			_distance *= normalizeFactor;
		}
	}

	template<typename T>
	void Plane<T>::transform(const Matrix4<T>& matrix)
	{
		Matrix4<T> inverted = matrix.inverse();
		Vector4<T> v = Vector4<T>(_normal, _distance);
		v = v * inverted;

		T divisor = (T)sqrt(v.x * v.x + v.y * v.y + v.z * v.z);

		if(divisor != (T)0)
		{
			T factor = (T)1 / divisor;

			_normal.x = v.x * factor;
			_normal.y = v.y * factor;
			_normal.z = v.z * factor;
			_distance = v.w * factor;
		}
	}
}