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
		return (T)Math::abs(Vector3<T>::dot(point, _normal) + _distance);
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
		//normalize();
	}

	template<typename T>
	void Plane<T>::set(const Vector3<T>& point, const Vector3<T>& normal)
	{
		_normal = normal;
		_distance = -Vector3<T>::dot(point, _normal);
		//normalize();
	}

	template<typename T>
	void Plane<T>::set(const Plane<T>& plane)
	{
		_normal = plane._normal;
		_distance = plane._distance;
		//normalize();
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

	template<typename T>
	void Plane<T>::intersection(const Plane<T>& p1, const Plane<T>& p2, const Plane<T>& p3, Vector3<T>& point)
	{
		Vector3<T> p1norm = Vector3<T>::normalize(p1._normal);
		Vector3<T> p2norm = Vector3<T>::normalize(p2._normal);
		Vector3<T> p3norm = Vector3<T>::normalize(p3._normal);

		T det = p1norm.x * (p2norm.y * p3norm.z -
			p2norm.z * p3norm.y) - p2norm.x * (p1norm.y * p3norm.z -
				p1norm.z * p3norm.y) + p3norm.x * (p1norm.y * p2norm.z - p1norm.z * p2norm.y);

		if (Math::abs(det) <= Math::EPSILON)
			return;

		T p1x = -p1norm.x * p1._distance;
		T p1y = -p1norm.y * p1._distance;
		T p1z = -p1norm.z * p1._distance;
		T p2x = -p2norm.x * p2._distance;
		T p2y = -p2norm.y * p2._distance;
		T p2z = -p2norm.z * p2._distance;
		T p3x = -p3norm.x * p3._distance;
		T p3y = -p3norm.y * p3._distance;
		T p3z = -p3norm.z * p3._distance;

		T c1x = (p2norm.y * p3norm.z) - (p2norm.z * p3norm.y);
		T c1y = (p2norm.z * p3norm.x) - (p2norm.x * p3norm.z);
		T c1z = (p2norm.x * p3norm.y) - (p2norm.y * p3norm.x);
		T c2x = (p3norm.y * p1norm.z) - (p3norm.z * p1norm.y);
		T c2y = (p3norm.z * p1norm.x) - (p3norm.x * p1norm.z);
		T c2z = (p3norm.x * p1norm.y) - (p3norm.y * p1norm.x);
		T c3x = (p1norm.y * p2norm.z) - (p1norm.z * p2norm.y);
		T c3y = (p1norm.z * p2norm.x) - (p1norm.x * p2norm.z);
		T c3z = (p1norm.x * p2norm.y) - (p1norm.y * p2norm.x);

		T s1 = p1x * p1norm.x + p1y * p1norm.y + p1z * p1norm.z;
		T s2 = p2x * p2norm.x + p2y * p2norm.y + p2z * p2norm.z;
		T s3 = p3x * p3norm.x + p3y * p3norm.y + p3z * p3norm.z;
		T detI = (T)1 / det;
		point.x = (s1 * c1x + s2 * c2x + s3 * c3x) * detI;
		point.y = (s1 * c1y + s2 * c2y + s3 * c3y) * detI;
		point.z = (s1 * c1z + s2 * c2z + s3 * c3z) * detI;
	}
}