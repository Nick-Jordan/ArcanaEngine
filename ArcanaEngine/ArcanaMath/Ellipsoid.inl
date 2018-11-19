namespace Arcana
{
	template<typename T>
	Ellipsoid<T>::Ellipsoid()
	{

	}

	template<typename T>
	Ellipsoid<T>::Ellipsoid(const Vector3<T>& center, const Vector3<T>& axes) : _center(center), _axes(axes)
	{

	}

	template<typename T>
	Ellipsoid<T>::~Ellipsoid()
	{

	}

	template<typename T>
	const Vector3<T>& Ellipsoid<T>::getCenter() const
	{
		return _center;
	}

	template<typename T>
	const Vector3<T>& Ellipsoid<T>::getAxes() const
	{
		return _axes;
	}

	template<typename T>
	bool Ellipsoid<T>::contains(const Vector3<T>& point)
	{
		Vector3<T> v = point - _center;
		v = Vector3<T>(v.x / _axes.x, v.y / _axes.y, v.z / _axes.z);
		return Vector3<T>::dot(v, v) <= (T)1;
	}

	template<typename T>
	bool Ellipsoid<T>::contains(T x, T y, T z)
	{
		return contains(Vector3<T>(x, y, z));
	}
}