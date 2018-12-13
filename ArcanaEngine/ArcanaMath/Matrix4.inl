
namespace Arcana
{
	//---------------------------------------------------Static Methods---------------------------------------------------//

	template<typename T>
	const Matrix4<T> Matrix4<T>::IDENTITY((T)1, (T)0, (T)0, (T)0, (T)0, (T)1, (T)0, (T)0, (T)0, (T)0, (T)1, (T)0, (T)0, (T)0, (T)0, (T)1);

	template<typename T>
	Matrix4<T> Matrix4<T>::createScale(const Vector3<T>& scale)
	{
		return createScale(scale.x, scale.y, scale.z);
	}

	template<typename T>
	Matrix4<T> Matrix4<T>::createScale(T xScale, T yScale, T zScale)
	{
		Matrix4<T> scale;

		scale.set(0, 0, xScale);
		scale.set(1, 1, yScale);
		scale.set(2, 2, zScale);

		return scale;
	}

	template<typename T>
	Matrix4<T> Matrix4<T>::createRotation(const Vector3<T>& axis, T angle)
	{
		Matrix3<T> rotation3x3 = Matrix3<T>::createRotation(axis, angle);

		return Matrix4(rotation3x3);
	}

	template<typename T>
	Matrix4<T> Matrix4<T>::createRotation(T pitch, T yaw, T roll)
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

		Matrix4<T> rotation;
		rotation.set(0, ca * cb);
		rotation.set(1, sa * cb);
		rotation.set(2, -sb);
		rotation.set(3, (T)0);

		rotation.set(4, ca * sb * sy - sa * cy);
		rotation.set(5, sa * sb * sy + ca * cy);
		rotation.set(6, cb * sy);
		rotation.set(7, (T)0);

		rotation.set(8, ca * sb * cy + sa * sy);
		rotation.set(9, sa * sb * cy - ca * sy);
		rotation.set(10, cb * cy);
		rotation.set(11, (T)0);

		rotation.set(12, (T)0);
		rotation.set(13, (T)0);
		rotation.set(14, (T)0);
		rotation.set(15, (T)1);

		return rotation;
	}

	template<typename T>
	Matrix4<T> Matrix4<T>::createTranslation(const Vector3<T>& translation)
	{
		return createTranslation(translation.x, translation.y, translation.z);
	}

	template<typename T>
	Matrix4<T> Matrix4<T>::createTranslation(T xTranslation, T yTranslation, T zTranslation)
	{
		Matrix4<T> translation;

		translation.set(3, 0, xTranslation);
		translation.set(3, 1, yTranslation);
		translation.set(3, 2, zTranslation);

		return translation;
	}

	template<typename T>
	Matrix4<T> Matrix4<T>::createPerspective(T fieldOfView, T aspectRatio, T zNearPlane, T zFarPlane)
	{
		Matrix4<T> perspective;

		if (zFarPlane != zNearPlane && aspectRatio != (T)0)
		{
			T fn = (T)1 / (zFarPlane - zNearPlane);
			T theta = (T)Math::degreesToRadians(fieldOfView) * (T)0.5;
			if (abs(fmod(theta, Math::HALF_PI)) < Math::EPSILON)
			{
				//printf("Invalid field of view value (%d) causes attempted calculation tan(%d), which is undefined.", fieldOfView, theta);
				return perspective;
			}

			T divisor = tan(theta);
			
			if (divisor == (T)0) return perspective;

			T factor = (T)1 / divisor;

			perspective.set(0, 0, ((T)1 / aspectRatio) * factor);
			perspective.set(1, 1, factor);
			perspective.set(2, 2, (-(zFarPlane + zNearPlane)) * fn);
			perspective.set(2, 3, (T)-1);
			perspective.set(3, 2, ((T)-2) * zFarPlane * zNearPlane * fn);
		}

		return perspective;
	}

	template<typename T>
	Matrix4<T> Matrix4<T>::createOrthographic(T width, float height, T zNearPlane, T zFarPlane)
	{
		T halfWidth = width / (T)2;
		T halfHeight = height / (T)2;
		return createOrthographicOffCenter(-halfWidth, halfWidth, -halfHeight, halfHeight, zNearPlane, zFarPlane);
	}

	template<typename T>
	Matrix4<T> Matrix4<T>::createOrthographicOffCenter(T left, T right, T bottom, T top, T zNearPlane, T zFarPlane)
	{
		Matrix4<T> orthographic;

		if (right != left && top != bottom && zFarPlane != zNearPlane)
		{
			orthographic.set(0, 0, (T)2 / (right - left));
			orthographic.set(1, 0, (T)2 / (top - bottom));
			orthographic.set(3, 0, (left + right) / (left - right));
			orthographic.set(2, 2, (T)1 / (zNearPlane - zFarPlane));
			orthographic.set(3, 1, (top + bottom) / (bottom - top));
			orthographic.set(3, 2, (T)zNearPlane / (zNearPlane - zFarPlane));
			orthographic.set(3, 3, (T)1);
		}

		return orthographic;
	}

	//---------------------------------------------------Class Methods---------------------------------------------------//

	template<typename T>
	Matrix4<T>::Matrix4()
	{
		_values[0] = (T)1;
		_values[1] = (T)0;
		_values[2] = (T)0;
		_values[3] = (T)0;
		_values[4] = (T)0;
		_values[5] = (T)1;
		_values[6] = (T)0;
		_values[7] = (T)0;
		_values[8] = (T)0;
		_values[9] = (T)0;
		_values[10] = (T)1;
		_values[11] = (T)0;
		_values[12] = (T)0;
		_values[13] = (T)0;
		_values[14] = (T)0;
		_values[15] = (T)1;
	}

	template<typename T>
	Matrix4<T>::Matrix4(T m00, T m01, T m02, T m03,
		T m10, T m11, T m12, T m13,
		T m20, T m21, T m22, T m23,
		T m30, T m31, T m32, T m33)
	{
		_values[0] = m00;
		_values[1] = m01;
		_values[2] = m02;
		_values[3] = m03;
		_values[4] = m10;
		_values[5] = m11;
		_values[6] = m12;
		_values[7] = m13;
		_values[8] = m20;
		_values[9] = m21;
		_values[10] = m22;
		_values[11] = m23;
		_values[12] = m30;
		_values[13] = m31;
		_values[14] = m32;
		_values[15] = m33;
	}

	template<typename T>
	Matrix4<T>::Matrix4(const Matrix3<T>& matrix)
	{
		_values[0] = matrix.at(0);
		_values[1] = matrix.at(1);
		_values[2] = matrix.at(2);
		_values[3] = (T)0;
		_values[4] = matrix.at(3);
		_values[5] = matrix.at(4);
		_values[6] = matrix.at(5);
		_values[7] = (T)0;
		_values[8] = matrix.at(6);
		_values[9] = matrix.at(7);
		_values[10] = matrix.at(8);
		_values[11] = (T)0;
		_values[12] = (T)0;
		_values[13] = (T)0;
		_values[14] = (T)0;
		_values[15] = (T)1;
	}

	template<typename T>
	Matrix4<T>::~Matrix4()
	{

	}

	template<typename T>
	T Matrix4<T>::determinant() const
	{
		return (_values[0] * _values[5] - _values[4] * _values[1])
			* (_values[10] * _values[15] - _values[14] * _values[11])
			- (_values[0] * _values[9] - _values[8] * _values[1])
			* (_values[6] * _values[15] - _values[14] * _values[7])
			+ (_values[0] * _values[13] - _values[12] * _values[1])
			* (_values[6] * _values[11] - _values[10] * _values[7])
			+ (_values[4] * _values[9] - _values[8] * _values[5])
			* (_values[2] * _values[15] - _values[14] * _values[3])
			- (_values[4] * _values[13] - _values[12] * _values[5])
			* (_values[2] * _values[11] - _values[10] * _values[3])
			+ (_values[8] * _values[13] - _values[12] * _values[9])
			* (_values[2] * _values[7] - _values[6] * _values[3]);
	}

	template<typename T>
	void Matrix4<T>::fromAxes(const Vector3<T> &x, const Vector3<T> &y, const Vector3<T> &z)
	{
		_values[0] = x.x;
		_values[1] = x.y;
		_values[2] = x.z;
		_values[3] = (T)0;
		_values[4] = y.x;
		_values[5] = y.y;
		_values[6] = y.z;
		_values[7] = (T)0;
		_values[8] = z.x;
		_values[9] = z.y;
		_values[10] = z.z;
		_values[11] = (T)0;
		_values[12] = (T)0;
		_values[13] = (T)0;
		_values[14] = (T)0;
		_values[15] = (T)1;
	}

	template<typename T>
	void Matrix4<T>::fromAxesTransposed(const Vector3<T> &x, const Vector3<T> &y, const Vector3<T> &z)
	{
		_values[0] = x.x;
		_values[1] = y.x;
		_values[2] = z.x;
		_values[3] = (T)0;
		_values[4] = x.y;
		_values[5] = y.y;
		_values[6] = z.y;
		_values[7] = (T)0;
		_values[8] = x.z;
		_values[9] = y.z;
		_values[10] = z.z;
		_values[11] = (T)0;
		_values[12] = (T)0;
		_values[13] = (T)0;
		_values[14] = (T)0;
		_values[15] = (T)1;
	}

	template<typename T>
	void Matrix4<T>::toAxes(Vector3<T> &x, Vector3<T> &y, Vector3<T> &z) const
	{
		x.set(_values[0], _values[1], _values[2]);
		y.set(_values[4], _values[5], _values[6]);
		z.set(_values[8], _values[9], _values[10]);
	}
	template<typename T>
	void Matrix4<T>::toAxesTransposed(Vector3<T> &x, Vector3<T> &y, Vector3<T> &z) const
	{
		x.set(_values[0], _values[4], _values[8]);
		y.set(_values[1], _values[5], _values[9]);
		z.set(_values[2], _values[6], _values[10]);
	}

	template<typename T>
	void Matrix4<T>::identity()
	{
		_values[0] = (T)1;
		_values[1] = (T)0;
		_values[2] = (T)0;
		_values[3] = (T)0;
		_values[4] = (T)0;
		_values[5] = (T)1;
		_values[6] = (T)0;
		_values[7] = (T)0;
		_values[8] = (T)0;
		_values[9] = (T)0;
		_values[10] = (T)1;
		_values[11] = (T)0;
		_values[12] = (T)0;
		_values[13] = (T)0;
		_values[14] = (T)0;
		_values[15] = (T)1;
	}

	template<typename T>
	Matrix4<T> Matrix4<T>::inverse() const
	{
		Matrix4<T> temp;
		T d = determinant();

		if (d == (T)0)
		{
			temp.identity();
		}
		else
		{
			d = (T)1 / d;

			temp._values[0] = d * (_values[5] * (_values[10] * _values[15] - _values[14] * _values[11]) + _values[9] * (_values[14] * _values[7] - _values[6] * _values[15]) + _values[13] * (_values[6] * _values[11] - _values[10] * _values[7]));
			temp._values[4] = d * (_values[6] * (_values[8] * _values[15] - _values[12] * _values[11]) + _values[10] * (_values[12] * _values[7] - _values[4] * _values[15]) + _values[14] * (_values[4] * _values[11] - _values[8] * _values[7]));
			temp._values[8] = d * (_values[7] * (_values[8] * _values[13] - _values[12] * _values[9]) + _values[11] * (_values[12] * _values[5] - _values[4] * _values[13]) + _values[15] * (_values[4] * _values[9] - _values[8] * _values[5]));
			temp._values[12] = d * (_values[4] * (_values[13] * _values[10] - _values[9] * _values[14]) + _values[8] * (_values[5] * _values[14] - _values[13] * _values[6]) + _values[12] * (_values[9] * _values[6] - _values[5] * _values[10]));

			temp._values[1] = d * (_values[9] * (_values[2] * _values[15] - _values[14] * _values[3]) + _values[13] * (_values[10] * _values[3] - _values[2] * _values[11]) + _values[1] * (_values[14] * _values[11] - _values[10] * _values[15]));
			temp._values[5] = d * (_values[10] * (_values[0] * _values[15] - _values[12] * _values[3]) + _values[14] * (_values[8] * _values[3] - _values[0] * _values[11]) + _values[2] * (_values[12] * _values[11] - _values[8] * _values[15]));
			temp._values[9] = d * (_values[11] * (_values[0] * _values[13] - _values[12] * _values[1]) + _values[15] * (_values[8] * _values[1] - _values[0] * _values[9]) + _values[3] * (_values[12] * _values[9] - _values[8] * _values[13]));
			temp._values[13] = d * (_values[8] * (_values[13] * _values[2] - _values[1] * _values[14]) + _values[12] * (_values[1] * _values[10] - _values[9] * _values[2]) + _values[0] * (_values[9] * _values[14] - _values[13] * _values[10]));

			temp._values[2] = d * (_values[13] * (_values[2] * _values[7] - _values[6] * _values[3]) + _values[1] * (_values[6] * _values[15] - _values[14] * _values[7]) + _values[5] * (_values[14] * _values[3] - _values[2] * _values[15]));
			temp._values[6] = d * (_values[14] * (_values[0] * _values[7] - _values[4] * _values[3]) + _values[2] * (_values[4] * _values[15] - _values[12] * _values[7]) + _values[6] * (_values[12] * _values[3] - _values[0] * _values[15]));
			temp._values[10] = d * (_values[15] * (_values[0] * _values[5] - _values[4] * _values[1]) + _values[3] * (_values[4] * _values[13] - _values[12] * _values[5]) + _values[7] * (_values[12] * _values[1] - _values[0] * _values[13]));
			temp._values[14] = d * (_values[12] * (_values[5] * _values[2] - _values[1] * _values[6]) + _values[0] * (_values[13] * _values[6] - _values[5] * _values[14]) + _values[4] * (_values[1] * _values[14] - _values[13] * _values[2]));

			temp._values[3] = d * (_values[1] * (_values[10] * _values[7] - _values[6] * _values[11]) + _values[5] * (_values[2] * _values[11] - _values[10] * _values[3]) + _values[9] * (_values[6] * _values[3] - _values[2] * _values[7]));
			temp._values[7] = d * (_values[2] * (_values[8] * _values[7] - _values[4] * _values[11]) + _values[6] * (_values[0] * _values[11] - _values[8] * _values[3]) + _values[10] * (_values[4] * _values[3] - _values[0] * _values[7]));
			temp._values[11] = d * (_values[3] * (_values[8] * _values[5] - _values[4] * _values[9]) + _values[7] * (_values[0] * _values[9] - _values[8] * _values[1]) + _values[11] * (_values[4] * _values[1] - _values[0] * _values[5]));
			temp._values[15] = d * (_values[0] * (_values[5] * _values[10] - _values[9] * _values[6]) + _values[4] * (_values[9] * _values[2] - _values[1] * _values[10]) + _values[8] * (_values[1] * _values[6] - _values[5] * _values[2]));
		}

		return temp;
	}

	template<typename T>
	Matrix4<T> Matrix4<T>::transpose() const
	{
		Matrix4<T> temp;

		temp._values[0] = _values[0];
		temp._values[1] = _values[4];
		temp._values[2] = _values[8];
		temp._values[3] = _values[12];

		temp._values[4] = _values[1];
		temp._values[5] = _values[5];
		temp._values[6] = _values[9];
		temp._values[7] = _values[13];

		temp._values[8] = _values[2];
		temp._values[9] = _values[6];
		temp._values[10] = _values[10];
		temp._values[11] = _values[14];

		temp._values[12] = _values[3];
		temp._values[13] = _values[7];
		temp._values[14] = _values[11];
		temp._values[15] = _values[15];

		return temp;
	}

	template<typename T>
	T Matrix4<T>::at(unsigned int i, unsigned int j) const
	{
		if (i < 4 && j < 4)
		{
			return _values[j + 4 * i];
		}
		return (T)0;
	}

	template<typename T>
	T Matrix4<T>::at(unsigned int i) const
	{
		if (i < 16)
		{
			return _values[i];
		}

		return (T)0;
	}

	template<typename T>
	void Matrix4<T>::set(unsigned int i, unsigned int j, T value)
	{
		if (i < 4 && j < 4)
		{
			_values[j + 4 * i] = value;
		}
	}

	template<typename T>
	void Matrix4<T>::set(unsigned int i, T value)
	{
		if (i < 16)
		{
			_values[i] = value;
		}
	}

	template<typename T>
	const T* Matrix4<T>::getValuePointer() const
	{
		return _values;
	}


	template<typename T>
	Vector3<T> Matrix4<T>::getUpVector() const
	{
		return Vector3<T>(_values[4], _values[5], _values[6]);
	}

	template<typename T>
	Vector3<T> Matrix4<T>::getDownVector() const
	{
		return Vector3<T>(-_values[4], -_values[5], -_values[6]);
	}

	template<typename T>
	Vector3<T> Matrix4<T>::getLeftVector() const
	{
		return Vector3<T>(-_values[0], -_values[1], -_values[2]);
	}

	template<typename T>
	Vector3<T> Matrix4<T>::getRightVector() const
	{
		return Vector3<T>(_values[0], _values[1], _values[2]);
	}

	template<typename T>
	Vector3<T> Matrix4<T>::getForwardVector() const
	{
		return Vector3<T>(_values[8], _values[9], _values[10]);
	}

	template<typename T>
	Vector3<T> Matrix4<T>::getBackVector() const
	{
		return Vector3<T>(-_values[8], -_values[9], -_values[10]);
	}

	//---------------------------------------------------Operators---------------------------------------------------//

	template<typename T>
	Vector3<T> operator*(const Matrix4<T> &lhs, const Vector3<T> &rhs)
	{
		return Vector3<T>((rhs.x * lhs._values[0]) + (rhs.y * lhs._values[3]) + (rhs.z * lhs._values[6]),
			(rhs.x * lhs._values[1]) + (rhs.y * lhs._values[4]) + (rhs.z * lhs._values[7]),
			(rhs.x * lhs._values[2]) + (rhs.y * lhs._values[5]) + (rhs.z * lhs._values[8]));
	}
	template<typename T>
	Matrix4<T> operator*(T scalar, const Matrix4<T> &rhs)
	{
		return rhs * scalar;
	}

	template<typename T>
	bool Matrix4<T>::operator==(const Matrix4<T> &rhs) const
	{
		return _values[0] == rhs._values[0]
			&& _values[1] == rhs._values[1]
			&& _values[2] == rhs._values[2]
			&& _values[3] == rhs._values[3]
			&& _values[4] == rhs._values[4]
			&& _values[5] == rhs._values[5]
			&& _values[6] == rhs._values[6]
			&& _values[7] == rhs._values[7]
			&& _values[8] == rhs._values[8]
			&& _values[9] == rhs._values[9]
			&& _values[10] == rhs._values[10]
			&& _values[11] == rhs._values[11]
			&& _values[12] == rhs._values[12]
			&& _values[13] == rhs._values[13]
			&& _values[14] == rhs._values[14]
			&& _values[15] == rhs._values[15];
	}
	template<typename T>
	bool Matrix4<T>::operator!=(const Matrix4<T> &rhs) const
	{
		return !(*this == rhs);
	}
	template<typename T>
	Matrix4<T> &Matrix4<T>::operator+=(const Matrix4<T> &rhs)
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
		_values[9] += rhs._values[9];
		_values[10] += rhs._values[10];
		_values[11] += rhs._values[11];
		_values[12] += rhs._values[12];
		_values[13] += rhs._values[13];
		_values[14] += rhs._values[14];
		_values[15] += rhs._values[15];
		return *this;
	}
	template<typename T>
	Matrix4<T> &Matrix4<T>::operator-=(const Matrix4<T> &rhs)
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
		_values[9] -= rhs._values[9];
		_values[10] -= rhs._values[10];
		_values[11] -= rhs._values[11];
		_values[12] -= rhs._values[12];
		_values[13] -= rhs._values[13];
		_values[14] -= rhs._values[14];
		_values[15] -= rhs._values[15];
		return *this;
	}
	template<typename T>
	Matrix4<T> &Matrix4<T>::operator*=(const Matrix4<T> &rhs)
	{
		Matrix4<T> temp;

		// Row 1.
		temp._values[0] = (_values[0] * rhs._values[0]) + (_values[1] * rhs._values[4]) + (_values[2] * rhs._values[8]) + (_values[3] * rhs._values[12]);
		temp._values[1] = (_values[0] * rhs._values[1]) + (_values[1] * rhs._values[5]) + (_values[2] * rhs._values[9]) + (_values[3] * rhs._values[13]);
		temp._values[2] = (_values[0] * rhs._values[2]) + (_values[1] * rhs._values[6]) + (_values[2] * rhs._values[10]) + (_values[3] * rhs._values[14]);
		temp._values[3] = (_values[0] * rhs._values[3]) + (_values[1] * rhs._values[7]) + (_values[2] * rhs._values[11]) + (_values[3] * rhs._values[15]);

		// Row 2.
		temp._values[4] = (_values[4] * rhs._values[0]) + (_values[5] * rhs._values[4]) + (_values[6] * rhs._values[8]) + (_values[7] * rhs._values[12]);
		temp._values[5] = (_values[4] * rhs._values[1]) + (_values[5] * rhs._values[5]) + (_values[6] * rhs._values[9]) + (_values[7] * rhs._values[13]);
		temp._values[6] = (_values[4] * rhs._values[2]) + (_values[5] * rhs._values[6]) + (_values[6] * rhs._values[10]) + (_values[7] * rhs._values[14]);
		temp._values[7] = (_values[4] * rhs._values[3]) + (_values[5] * rhs._values[7]) + (_values[6] * rhs._values[11]) + (_values[7] * rhs._values[15]);

		// Row 3.
		temp._values[8] = (_values[8] * rhs._values[0]) + (_values[9] * rhs._values[4]) + (_values[10] * rhs._values[8]) + (_values[11] * rhs._values[12]);
		temp._values[9] = (_values[8] * rhs._values[1]) + (_values[9] * rhs._values[5]) + (_values[10] * rhs._values[9]) + (_values[11] * rhs._values[13]);
		temp._values[10] = (_values[8] * rhs._values[2]) + (_values[9] * rhs._values[6]) + (_values[10] * rhs._values[10]) + (_values[11] * rhs._values[14]);
		temp._values[11] = (_values[8] * rhs._values[3]) + (_values[9] * rhs._values[7]) + (_values[10] * rhs._values[11]) + (_values[11] * rhs._values[15]);

		// Row 4.
		temp._values[12] = (_values[12] * rhs._values[0]) + (_values[13] * rhs._values[4]) + (_values[14] * rhs._values[8]) + (_values[15] * rhs._values[12]);
		temp._values[13] = (_values[12] * rhs._values[1]) + (_values[13] * rhs._values[5]) + (_values[14] * rhs._values[9]) + (_values[15] * rhs._values[13]);
		temp._values[14] = (_values[12] * rhs._values[2]) + (_values[13] * rhs._values[6]) + (_values[14] * rhs._values[10]) + (_values[15] * rhs._values[14]);
		temp._values[15] = (_values[12] * rhs._values[3]) + (_values[13] * rhs._values[7]) + (_values[14] * rhs._values[11]) + (_values[15] * rhs._values[15]);

		*this = temp;
		return *this;
	}
	template<typename T>
	Matrix4<T> &Matrix4<T>::operator*=(T scalar)
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
		_values[9] *= scalar;
		_values[10] *= scalar;
		_values[11] *= scalar;
		_values[12] *= scalar;
		_values[13] *= scalar;
		_values[14] *= scalar;
		_values[15] *= scalar;
		return *this;
	}
	template<typename T>
	Matrix4<T> &Matrix4<T>::operator/=(T scalar)
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
		_values[9] /= scalar;
		_values[10] /= scalar;
		_values[11] /= scalar;
		_values[12] /= scalar;
		_values[13] /= scalar;
		_values[14] /= scalar;
		_values[15] /= scalar;
		return *this;
	}
	template<typename T>
	Matrix4<T> Matrix4<T>::operator+(const Matrix4<T> &rhs) const
	{
		Matrix4<T> temp(*this);
		temp += rhs;
		return temp;
	}
	template<typename T>
	Matrix4<T> Matrix4<T>::operator-(const Matrix4<T> &rhs) const
	{
		Matrix4<T> temp(*this);
		temp -= rhs;
		return temp;
	}
	template<typename T>
	Matrix4<T> Matrix4<T>::operator*(const Matrix4<T> &rhs) const
	{
		Matrix4<T> temp(*this);
		temp *= rhs;
		return temp;
	}
	template<typename T>
	Matrix4<T> Matrix4<T>::operator*(T scalar) const
	{
		Matrix4<T> temp(*this);
		temp *= scalar;
		return temp;
	}
	template<typename T>
	Matrix4<T> Matrix4<T>::operator/(T scalar) const
	{
		Matrix4<T> temp(*this);
		temp /= scalar;
		return temp;
	}
	template<typename T>
	Vector3<T> &Matrix4<T>::operator*(Vector3<T> &rhs)
	{
		Vector4<T> v = Vector4<T>(
			(rhs.x * _values[0]) + (rhs.y * _values[4]) + (rhs.z * _values[8]) + (_values[12]),
			(rhs.x * _values[1]) + (rhs.y * _values[5]) + (rhs.z * _values[9]) + (_values[13]),
			(rhs.x * _values[2]) + (rhs.y * _values[6]) + (rhs.z * _values[10]) + (_values[14]),
			(rhs.x * _values[3]) + (rhs.y * _values[7]) + (rhs.z * _values[11]) + (_values[15]));
		rhs.x = v.x; rhs.y = v.y; rhs.z = v.z;
		return rhs;
	}
	template<typename T>
	Vector4<T> &Matrix4<T>::operator*(Vector4<T> &rhs)
	{
		Vector4<T> v = Vector4<T>(
			(rhs.x * _values[0]) + (rhs.y * _values[4]) + (rhs.z * _values[8]) + (rhs.w * _values[12]),
			(rhs.x * _values[1]) + (rhs.y * _values[5]) + (rhs.z * _values[9]) + (rhs.w * _values[13]),
			(rhs.x * _values[2]) + (rhs.y * _values[6]) + (rhs.z * _values[10]) + (rhs.w * _values[14]),
			(rhs.x * _values[3]) + (rhs.y * _values[7]) + (rhs.z * _values[11]) + (rhs.w * _values[15]));
		rhs.x = v.x; rhs.y = v.y; rhs.z = v.z; rhs.w = v.w;
		return rhs;
	}
}