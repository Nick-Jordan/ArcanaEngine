
namespace Arcana
{
	//---------------------------------------------------Static Methods---------------------------------------------------//

	template<typename T>
	const Matrix3<T> Matrix3<T>::IDENTITY((T)1, (T)0, (T)0, (T)0, (T)1, (T)0, (T)0, (T)0, (T)1);

	template<typename T>
	Matrix3<T> Matrix3<T>::createScale(const Vector3<T>& scale)
	{
		return createScale(scale.x, scale.y, scale.z);
	}

	template<typename T>
	Matrix3<T> Matrix3<T>::createScale(T xScale, T yScale, T zScale)
	{
		Matrix3<T> scale;

		scale.set(0, 0, xScale);
		scale.set(1, 1, yScale);
		scale.set(2, 2, zScale);

		return scale;
	}

	template<typename T>
	Matrix3<T> Matrix3<T>::createRotation(const Vector3<T>& axis, T angle)
	{
		T x = axis.x;
		T y = axis.y;
		T z = axis.z;

		T n = x * x + y * y + z * z;
		if (n != (T)1)
		{
			n = sqrt(n);
			if (n > (T)Math::EPSILON)
			{
				n = (T)1 / n;
				x *= n;
				y *= n;
				z *= n;
			}
		}

		T c = cos(angle);
		T s = sin(angle);

		T t = (T)1 - c;
		T tx = t * x;
		T ty = t * y;
		T tz = t * z;
		T txy = tx * y;
		T txz = tx * z;
		T tyz = ty * z;
		T sx = s * x;
		T sy = s * y;
		T sz = s * z;

		Matrix3<T> rotation;

		rotation.set(0, c + tx*x);
		rotation.set(1, txy + sz);
		rotation.set(2, txz - sy);

		rotation.set(3, txy - sz);
		rotation.set(4, c + ty*y);
		rotation.set(5, tyz + sx);

		rotation.set(6, txz + sy);
		rotation.set(7, tyz - sx);
		rotation.set(8, c + tz*z);

		return rotation;
	}

	template<typename T>
	Matrix3<T> Matrix3<T>::createRotation(T pitch, T yaw, T roll)
	{
		T p = (T)Math::degreesToRadians(pitch);
		T y = (T)Math::degreesToRadians(yaw);
		T r = (T)Math::degreesToRadians(roll);

		T ca = cos(p);
		T sa = sin(p);
		T cb = cos(y);
		T sb = sin(y);
		T cy = cos(r);
		T sy = sin(r);

		Matrix3<T> rotation;
		rotation.set(0, ca * cb);
		rotation.set(1, sa * cb);
		rotation.set(2, -sb);

		rotation.set(3, ca * sb * sy - sa * cy);
		rotation.set(4, sa * sb * sy + ca * cy);
		rotation.set(5, cb * sy);

		rotation.set(6, ca * sb * cy + sa * sy);
		rotation.set(7, sa * sb * cy - ca * sy);
		rotation.set(8, cb * cy);

		return rotation;
	}

	//---------------------------------------------------Class Methods---------------------------------------------------//

	template<typename T>
	Matrix3<T>::Matrix3()
	{
		_values[0] = (T)1;
		_values[1] = (T)0;
		_values[2] = (T)0;
		_values[3] = (T)0;
		_values[4] = (T)1;
		_values[5] = (T)0;
		_values[6] = (T)0;
		_values[7] = (T)0;
		_values[8] = (T)1;
	}

	template<typename T>
	Matrix3<T>::Matrix3(T m00, T m01, T m02,
		T m10, T m11, T m12,
		T m20, T m21, T m22)
	{
		_values[0] = m00;
		_values[1] = m01;
		_values[2] = m02;
		_values[3] = m10;
		_values[4] = m11;
		_values[5] = m12;
		_values[6] = m20;
		_values[7] = m21;
		_values[8] = m22;
	}

	template<typename T>
	Matrix3<T>::~Matrix3()
	{

	}

	template<typename T>
	T Matrix3<T>::determinant() const
	{
		return (_values[0] * (_values[4] * _values[8] - _values[5] * _values[7]))
			- (_values[1] * (_values[3] * _values[8] - _values[5] * _values[6]))
			+ (_values[2] * (_values[3] * _values[7] - _values[4] * _values[6]));
	}

	template<typename T>
	void Matrix3<T>::fromAxes(const Vector3<T> &x, const Vector3<T> &y, const Vector3<T> &z)
	{
		_values[0] = x.x;
		_values[1] = x.y;
		_values[2] = x.z;
		_values[3] = y.x;
		_values[4] = y.y;
		_values[5] = y.z;
		_values[6] = z.x;
		_values[7] = z.y;
		_values[8] = z.z;
	}

	template<typename T>
	void Matrix3<T>::fromAxesTransposed(const Vector3<T> &x, const Vector3<T> &y, const Vector3<T> &z)
	{
		_values[0] = x.x;
		_values[1] = y.x;
		_values[2] = z.x;
		_values[3] = x.y;
		_values[4] = y.y;
		_values[5] = z.y;
		_values[6] = x.z;
		_values[7] = y.z;
		_values[8] = z.z;
	}

	template<typename T>
	void Matrix3<T>::toAxes(Vector3<T> &x, Vector3<T> &y, Vector3<T> &z) const
	{
		x.set(_values[0], _values[1], _values[2]);
		y.set(_values[3], _values[4], _values[5]);
		z.set(_values[6], _values[7], _values[8]);
	}
	template<typename T>
	void Matrix3<T>::toAxesTransposed(Vector3<T> &x, Vector3<T> &y, Vector3<T> &z) const
	{
		x.set(_values[0], _values[3], _values[6]);
		y.set(_values[1], _values[4], _values[7]);
		z.set(_values[2], _values[5], _values[8]);
	}

	template<typename T>
	void Matrix3<T>::identity()
	{
		_values[0] = (T)1;
		_values[1] = (T)0;
		_values[2] = (T)0;
		_values[3] = (T)0;
		_values[4] = (T)1;
		_values[5] = (T)0;
		_values[6] = (T)0;
		_values[7] = (T)0;
		_values[8] = (T)1;
	}

	template<typename T>
	Matrix3<T> Matrix3<T>::inverse() const
	{
		Matrix3<T> temp;
		T d = determinant();

		if (d == (T)0)
		{
			temp.identity();
		}
		else
		{
			d = (T)1 / d;

			temp._values[0] = d * (_values[4] * _values[8] - _values[5] * _values[7]);
			temp._values[1] = d * (_values[2] * _values[7] - _values[1] * _values[8]);
			temp._values[2] = d * (_values[1] * _values[5] - _values[2] * _values[4]);

			temp._values[3] = d * (_values[5] * _values[6] - _values[3] * _values[8]);
			temp._values[4] = d * (_values[0] * _values[8] - _values[2] * _values[6]);
			temp._values[5] = d * (_values[2] * _values[3] - _values[0] * _values[5]);

			temp._values[6] = d * (_values[3] * _values[7] - _values[4] * _values[6]);
			temp._values[7] = d * (_values[1] * _values[6] - _values[0] * _values[7]);
			temp._values[8] = d * (_values[0] * _values[4] - _values[1] * _values[3]);
		}

		return temp;
	}

	template<typename T>
	Matrix3<T> Matrix3<T>::transpose() const
	{
		Matrix3<T> temp;

		temp._values[0] = _values[0];
		temp._values[1] = _values[3];
		temp._values[2] = _values[6];

		temp._values[3] = _values[1];
		temp._values[4] = _values[4];
		temp._values[5] = _values[7];

		temp._values[6] = _values[2];
		temp._values[7] = _values[5];
		temp._values[8] = _values[8];

		return temp;
	}

	template<typename T>
	T Matrix3<T>::at(unsigned int i, unsigned int j) const
	{
		if (i < 3 && j < 3)
		{
			return _values[j + 3 * i];
		}
		return (T)0;
	}

	template<typename T>
	T Matrix3<T>::at(unsigned int i) const
	{
		if (i < 9)
		{
			return _values[i];
		}

		return (T)0;
	}

	template<typename T>
	void Matrix3<T>::set(unsigned int i, unsigned int j, T value)
	{
		if (i < 3 && j < 3)
		{
			_values[j + 3 * i] = value;
		}
	}

	template<typename T>
	void Matrix3<T>::set(unsigned int i, T value)
	{
		if (i < 9)
		{
			_values[i] = value;
		}
	}

	template<typename T>
	const T* Matrix3<T>::getValuePointer() const
	{
		return _values;
	}


	template<typename T>
	Vector3<T> Matrix3<T>::getUpVector() const
	{
		return Vector3<T>(_values[3], _values[4], _values[5]);
	}

	template<typename T>
	Vector3<T> Matrix3<T>::getDownVector() const
	{
		return Vector3<T>(-_values[3], -_values[4], -_values[5]);
	}

	template<typename T>
	Vector3<T> Matrix3<T>::getLeftVector() const
	{
		return Vector3<T>(-_values[0], -_values[1], -_values[2]);
	}

	template<typename T>
	Vector3<T> Matrix3<T>::getRightVector() const
	{
		return Vector3<T>(_values[0], _values[1], _values[2]);
	}

	template<typename T>
	Vector3<T> Matrix3<T>::getForwardVector() const
	{
		return Vector3<T>(_values[6], _values[7], _values[8]);
	}

	template<typename T>
	Vector3<T> Matrix3<T>::getBackVector() const
	{
		return Vector3<T>(-_values[6], -_values[7], -_values[8]);
	}

	//---------------------------------------------------Operators---------------------------------------------------//

	template<typename T>
	Vector3<T> operator*(const Matrix3<T> &lhs, const Vector3<T> &rhs)
	{
		return Vector3<T>((rhs.x * lhs._values[0]) + (rhs.y * lhs._values[3]) + (rhs.z * lhs._values[6]),
			(rhs.x * lhs._values[1]) + (rhs.y * lhs._values[4]) + (rhs.z * lhs._values[7]),
			(rhs.x * lhs._values[2]) + (rhs.y * lhs._values[5]) + (rhs.z * lhs._values[8]));
	}
	template<typename T>
	Matrix3<T> operator*(T scalar, const Matrix3<T> &rhs)
	{
		return rhs * scalar;
	}

	template<typename T>
	bool Matrix3<T>::operator==(const Matrix3<T> &rhs) const
	{
		return _values[0] == rhs._values[0]
			&& _values[1] == rhs._values[1]
			&& _values[2] == rhs._values[2]
			&& _values[3] == rhs._values[3]
			&& _values[4] == rhs._values[4]
			&& _values[5] == rhs._values[5]
			&& _values[6] == rhs._values[6]
			&& _values[7] == rhs._values[7]
			&& _values[8] == rhs._values[8];
	}
	template<typename T>
	bool Matrix3<T>::operator!=(const Matrix3<T> &rhs) const
	{
		return !(*this == rhs);
	}
	template<typename T>
	Matrix3<T> &Matrix3<T>::operator+=(const Matrix3<T> &rhs)
	{
		_values[0] += rhs._values[0];
		_values[1] += rhs._values[1];
		_values[2] += rhs._values[2];
		_values[3] += rhs._values[3];
		_values[4] += rhs._values[4];
		_values[5] += rhs._values[5];
		_values[6] += rhs._values[6];
		_values[7] += rhs._values[7];
		_values[8] += rhs._values[8];

		return *this;
	}
	template<typename T>
	Matrix3<T> &Matrix3<T>::operator-=(const Matrix3<T> &rhs)
	{
		_values[0] -= rhs._values[0];
		_values[1] -= rhs._values[1];
		_values[2] -= rhs._values[2];
		_values[3] -= rhs._values[3];
		_values[4] -= rhs._values[4];
		_values[5] -= rhs._values[5];
		_values[6] -= rhs._values[6];
		_values[7] -= rhs._values[7];
		_values[8] -= rhs._values[8];

		return *this;
	}
	template<typename T>
	Matrix3<T> &Matrix3<T>::operator*=(const Matrix3<T> &rhs)
	{
		Matrix3<T> temp;

		// Row 0.
		temp._values[0] = (_values[0] * rhs._values[0]) + (_values[1] * rhs._values[3]) + (_values[2] * rhs._values[6]);
		temp._values[1] = (_values[0] * rhs._values[1]) + (_values[1] * rhs._values[4]) + (_values[2] * rhs._values[7]);
		temp._values[2] = (_values[0] * rhs._values[2]) + (_values[1] * rhs._values[5]) + (_values[2] * rhs._values[8]);

		// Row 1.
		temp._values[3] = (_values[3] * rhs._values[0]) + (_values[4] * rhs._values[3]) + (_values[5] * rhs._values[6]);
		temp._values[4] = (_values[3] * rhs._values[1]) + (_values[4] * rhs._values[4]) + (_values[5] * rhs._values[7]);
		temp._values[5] = (_values[3] * rhs._values[2]) + (_values[4] * rhs._values[5]) + (_values[5] * rhs._values[8]);

		// Row 2.
		temp._values[6] = (_values[6] * rhs._values[0]) + (_values[7] * rhs._values[3]) + (_values[8] * rhs._values[6]);
		temp._values[7] = (_values[6] * rhs._values[1]) + (_values[7] * rhs._values[4]) + (_values[8] * rhs._values[7]);
		temp._values[8] = (_values[6] * rhs._values[2]) + (_values[7] * rhs._values[5]) + (_values[8] * rhs._values[8]);

		*this = temp;

		return *this;
	}
	template<typename T>
	Matrix3<T> &Matrix3<T>::operator*=(T scalar)
	{
		_values[0] *= scalar;
		_values[1] *= scalar;
		_values[2] *= scalar;
		_values[3] *= scalar;
		_values[4] *= scalar;
		_values[5] *= scalar;
		_values[6] *= scalar;
		_values[7] *= scalar;
		_values[8] *= scalar;
		return *this;
	}
	template<typename T>
	Matrix3<T> &Matrix3<T>::operator/=(T scalar)
	{
		_values[0] /= scalar;
		_values[1] /= scalar;
		_values[2] /= scalar;
		_values[3] /= scalar;
		_values[4] /= scalar;
		_values[5] /= scalar;
		_values[6] /= scalar;
		_values[7] /= scalar;
		_values[8] /= scalar;
		return *this;
	}
	template<typename T>
	Matrix3<T> Matrix3<T>::operator+(const Matrix3<T> &rhs) const
	{
		Matrix3<T> temp(*this);
		temp += rhs;
		return temp;
	}
	template<typename T>
	Matrix3<T> Matrix3<T>::operator-(const Matrix3<T> &rhs) const
	{
		Matrix3<T> temp(*this);
		temp -= rhs;
		return temp;
	}
	template<typename T>
	Matrix3<T> Matrix3<T>::operator*(const Matrix3<T> &rhs) const
	{
		Matrix3<T> temp(*this);
		temp *= rhs;
		return temp;
	}
	template<typename T>
	Matrix3<T> Matrix3<T>::operator*(T scalar) const
	{
		Matrix3<T> temp(*this);
		temp *= scalar;
		return temp;
	}
	template<typename T>
	Matrix3<T> Matrix3<T>::operator/(T scalar) const
	{
		Matrix3<T> temp(*this);
		temp /= scalar;
		return temp;
	}
}