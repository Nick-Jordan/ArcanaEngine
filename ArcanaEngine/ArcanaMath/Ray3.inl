
namespace Arcana
{
	template<typename T>
	Ray3<T>::Ray3() : _origin(0, 0, 0), _direction(0, 0, 0)
	{

	}

	template<typename T>
	Ray3<T>::Ray3(const Vector3<T>& origin, const Vector3<T>& direction)
	{
		set(origin, direction);
	}

	template<typename T>
	Ray3<T>::Ray3(T originX, T originY, T originZ, T directionX, T directionY, T directionZ)
	{
		set(originX, originY, originZ, directionX, directionY, directionZ);
	}

	template<typename T>
	Ray3<T>::Ray3(const Ray3<T>& copy)
	{
		set(copy);
	}

	template<typename T>
	Ray3<T>::~Ray3()
	{

	}

	template<typename T>
	const Vector3<T>& Ray3<T>::getOrigin() const
	{
		return _origin;
	}

	template<typename T>
	void Ray3<T>::setOrigin(const Vector3<T>& origin)
	{
		_origin = origin;
	}

	template<typename T>
	void Ray3<T>::setOrigin(T originX, T originY, T originZ)
	{
		_origin.x = originX;
		_origin.y = originY;
		_origin.z = originZ;
	}

	template<typename T>
	const Vector3<T>& Ray3<T>::getDirection() const
	{
		return _direction;
	}

	template<typename T>
	void Ray3<T>::setDirection(const Vector3<T>& direction)
	{
		_direction = direction;
	}

	template<typename T>
	void Ray3<T>::setDirection(T directionX, T directionY, T directionZ)
	{
		_direction.x = directionX;
		_direction.y = directionY;
		_direction.z = directionZ;
	}

	template<typename T>
	void Ray3<T>::set(const Vector3<T>& origin, const Vector3<T>& direction)
	{
		_origin = origin;
		_direction = direction;
	}

	template<typename T>
	void Ray3<T>::set(T originX, T originY, T originZ, T directionX, T directionY, T directionZ)
	{
		_origin.x = originX;
		_origin.y = originY;
		_origin.z = originZ;
		_direction.x = directionX;
		_direction.y = directionY;
		_direction.z = directionZ;
	}

	template<typename T>
	void Ray3<T>::normalize()
	{
		_direction.normalize();
	}

	template<typename T>
	void Ray3<T>::transform(const Matrix4<T>& matrix)
	{
		_origin = matrix * _origin;

		Vector4<T> dir = Vector4<T>(_direction.x, _direction.y, _direction.z, (T)0);

		dir = matrix * dir;

		_direction.x = dir.x;
		_direction.y = dir.y;
		_direction.z = dir.z;
	}

	template<typename T>
	void Ray3<T>::transform(const Matrix3<T>& matrix)
	{
		_origin = matrix * _origin;
		_direction = matrix * _direction;
	}

	template<typename T>
	Ray3<T>& Ray3<T>::operator*=(const Matrix4<T>& matrix)
	{
		transform(matrix);
		return *this;
	}

	template<typename T>
	Ray3<T>& Ray3<T>::operator*=(const Matrix3<T>& matrix)
	{
		transform(matrix);
		return *this;
	}

	template<typename T>
	Ray3<T>& Ray3<T>::operator*=(T scale)
	{
		_direction.scale(scale);
		return *this;
	}

	template<typename T>
	Ray3<T>& Ray3<T>::operator*=(const Vector3<T>& scale)
	{
		_direction *= scale;
		return *this;
	}

	template<typename T>
	Ray3<T>& Ray3<T>::operator+=(const Vector3<T>& offset)
	{
		_origin += offset;
		return *this;
	}

	template<typename T>
	Ray3<T>& Ray3<T>::operator=(const Ray3<T>& ray)
	{
		_origin = ray._origin;
		_direction = ray._direction;

		return *this;
	}
}