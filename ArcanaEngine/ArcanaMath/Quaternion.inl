
namespace Arcana
{
	//---------------------------------------------------Static Methods---------------------------------------------------//

	template<typename T>
	const Quaternion<T> Quaternion<T>::IDENTITY = Quaternion<T>((T)1, (T)0, (T)0, (T)0);

	template<typename T>
	Quaternion<T> Quaternion<T>::slerp(const Quaternion<T> &a, const Quaternion<T> &b, T t)
	{

	}


	//---------------------------------------------------Class Methods---------------------------------------------------//

	template<typename T>
	Quaternion<T>::Quaternion() : w((T)1), x((T)0), y((T)0), z((T)0)
	{

	}

	template<typename T>
	Quaternion<T>::Quaternion(T w_, T x_, T y_, T z_) : w(w_), x(x_), y(y_), z(z_)
	{

	}

	template<typename T>
	Quaternion<T>::Quaternion(T pitch, T yaw, T roll)
	{
		fromPitchYawRoll(pitch, yaw, roll);
	}

	template<typename T>
	Quaternion<T>::Quaternion(const Vector3<T> &axis, T angle)
	{
		fromAxisAngle(axis, angle);
	}

	template<typename T>
	Quaternion<T>::Quaternion(const Quaternion<T>& quaternion) : w(quaternion.w), x(quaternion.x), y(quaternion.y), z(quaternion.z)
	{

	}

	template<typename T>
	Quaternion<T>::Quaternion(const Matrix3<T> &m)
	{
		fromMatrix3(m);
	}

	template<typename T>
	Quaternion<T>::Quaternion(const Matrix4<T> &m)
	{
		fromMatrix4(m);
	}

	template<typename T>
	Quaternion<T>::~Quaternion()
	{

	}

	template<typename T>
	Quaternion<T> Quaternion<T>::conjugate() const
	{
		return Quaternion<T>(w, -x, -y, -z);
	}

	template<typename T>
	void Quaternion<T>::identity()
	{
		w = (T)1;
		x = (T)0;
		y = (T)0;
		z = (T)0;
	}

	template<typename T>
	bool Quaternion<T>::isIdentity() const
	{
		return w == (T)1 && x == (T)0 && y == (T)0 && z == (T)0;
	}

	template<typename T>
	bool Quaternion<T>::isZero() const
	{
		return w == (T)0 && x == (T)0 && y == (T)0 && z == (T)0;
	}

	template<typename T>
	Quaternion<T> Quaternion<T>::inverse() const
	{
		T invMag = (T)1 / magnitude();
		return conjugate() * invMag;
	}

	template<typename T>
	T Quaternion<T>::magnitude() const
	{
		return (T)sqrt(w * w + x * x + y * y + z * z);
	}

	template<typename T>
	void Quaternion<T>::normalize()
	{
		T invMag = (T)1 / magnitude();
		w *= invMag;
		x *= invMag;
		y *= invMag;
		z *= invMag;
	}

	template<typename T>
	void Quaternion<T>::set(T w_, T x_, T y_, T z_)
	{
		w = w_;
		x = x_;
		y = y_;
		z = z_;
	}

	template<typename T>
	void Quaternion<T>::set(Quaternion<T> q)
	{
		w = q.w;
		x = q.x;
		y = q.y;
		z = q.z;
	}


	template<typename T>
	void Quaternion<T>::fromAxisAngle(const Vector3<T> &axis, T degrees)
	{
		T halfTheta = (T)Math::degreesToRadians(degrees) * (T)0.5;
		T s = sin(halfTheta);

		w = cos(halfTheta);
		x = axis.x * s;
		y = axis.y * s;
		z = axis.z * s;
	}

	template<typename T>
	void Quaternion<T>::fromPitchYawRoll(T pitch, T yaw, T roll)
	{
		Matrix3<T> m = Matrix3<T>::createRotation(pitch, yaw, roll);
		fromMatrix3(m);
	}

	template<typename T>
	void Quaternion<T>::fromMatrix3(const Matrix3<T> &m)
	{
		T s = (T)0;
		T q[4] = { (T)0 };
		T trace = m.at(0, 0) + m.at(1, 1) + m.at(2, 2);

		if (trace > (T)0)
		{
			s = sqrtf(trace + (T)1);
			q[3] = s * (T)0.5;
			s = (T)0.5 / s;
			q[0] = (m.at(1, 2) - m.at(2, 1)) * s;
			q[1] = (m.at(2, 0) - m.at(0, 2)) * s;
			q[2] = (m.at(0, 1) - m.at(1, 0)) * s;
		}
		else
		{
			int nxt[3] = { 1, 2, 0 };
			int i = 0, j = 0, k = 0;

			if (m.at(1, 1) > m.at(0, 0))
				i = 1;

			if (m.at(2, 2) > m.at(i, i))
				i = 2;

			j = nxt[i];
			k = nxt[j];
			s = sqrtf((m.at(i, i) - (m.at(j, j) + m.at(k, k))) + (T)1);

			q[i] = s * (T)0.5;
			s = (T)0.5 / s;
			q[3] = (m.at(j, k) - m.at(k, j)) * s;
			q[j] = (m.at(i, j) + m.at(j, i)) * s;
			q[k] = (m.at(i, k) + m.at(k, i)) * s;
		}

		x = q[0];
		y = q[1];
		z = q[2];
		w = q[3];
	}

	template<typename T>
	void Quaternion<T>::fromMatrix4(const Matrix4<T> &m)
	{
		T s = (T)0;
		T q[4] = { (T)0 };
		T trace = m.at(0, 0) + m.at(1, 1) + m.at(2, 2);

		if (trace > (T)0)
		{
			s = sqrtf(trace + (T)1);
			q[3] = s * (T)0.5;
			s = (T)0.5 / s;
			q[0] = (m.at(1, 2) - m.at(2, 1)) * s;
			q[1] = (m.at(2, 0) - m.at(0, 2)) * s;
			q[2] = (m.at(0, 1) - m.at(1, 0)) * s;
		}
		else
		{
			int nxt[3] = { 1, 2, 0 };
			int i = 0, j = 0, k = 0;

			if (m.at(1, 1) > m.at(0, 0))
				i = 1;

			if (m.at(2, 2) > m.at(i, i))
				i = 2;

			j = nxt[i];
			k = nxt[j];
			s = sqrtf((m.at(i, i) - (m.at(j, j) + m.at(k, k))) + (T)1);

			q[i] = s * (T)0.5;
			s = (T)0.5 / s;
			q[3] = (m.at(j, k) - m.at(k, j)) * s;
			q[j] = (m.at(i, j) + m.at(j, i)) * s;
			q[k] = (m.at(i, k) + m.at(k, i)) * s;
		}

		x = q[0];
		y = q[1];
		z = q[2];
		w = q[3];
	}

	template<typename T>
	Matrix3<T> Quaternion<T>::toMatrix3() const
	{
		T x2 = x + x;
		T y2 = y + y;
		T z2 = z + z;
		T xx = x * x2;
		T xy = x * y2;
		T xz = x * z2;
		T yy = y * y2;
		T yz = y * z2;
		T zz = z * z2;
		T wx = w * x2;
		T wy = w * y2;
		T wz = w * z2;

		Matrix3<T> m;

		m.set(0, 0, (T)1 - (yy + zz));
		m.set(0, 1, xy + wz);
		m.set(0, 2, xz - wy);

		m.set(1, 0, xy - wz);
		m.set(1, 1, (T)1 - (xx + zz));
		m.set(1, 2, yz + wx);

		m.set(2, 0, xz + wy);
		m.set(2, 1, yz - wx);
		m.set(2, 2, (T)1 - (xx + yy));

		return m;
	}

	template<typename T>
	Matrix4<T> Quaternion<T>::toMatrix4() const
	{
		T x2 = x + x;
		T y2 = y + y;
		T z2 = z + z;
		T xx = x * x2;
		T xy = x * y2;
		T xz = x * z2;
		T yy = y * y2;
		T yz = y * z2;
		T zz = z * z2;
		T wx = w * x2;
		T wy = w * y2;
		T wz = w * z2;

		Matrix4<T> m;

		m.set(0, 0, (T)1 - (yy + zz));
		m.set(0, 1, xy + wz);
		m.set(0, 2, xz - wy);
		m.set(0, 3, (T)0);

		m.set(1, 0, xy - wz);
		m.set(1, 1, (T)1 - (xx + zz));
		m.set(1, 2, yz + wx);
		m.set(1, 3, (T)0);

		m.set(2, 0, xz + wy);
		m.set(2, 1, yz - wx);
		m.set(2, 2, (T)1 - (xx + yy));
		m.set(2, 3, (T)0);

		m.set(0, 3, (T)0);
		m.set(1, 3, (T)0);
		m.set(2, 3, (T)0);
		m.set(3, 3, (T)1);

		return m;
	}

	//---------------------------------------------------Operators---------------------------------------------------//

	template<typename T>
	Quaternion<T> operator*(T lhs, const Quaternion<T> &rhs)
	{
		return rhs * lhs;
	}

	template<typename T>
	bool Quaternion<T>::operator==(const Quaternion<T> &rhs) const
	{
		return w == rhs.w && x == rhs.x && y == rhs.y && z == rhs.z;
	}
	template<typename T>
	bool Quaternion<T>::operator!=(const Quaternion<T> &rhs) const
	{
		return !(*this == rhs);
	}
	template<typename T>
	Quaternion<T> &Quaternion<T>::operator+=(const Quaternion<T> &rhs)
	{
		w += rhs.w, x += rhs.x, y += rhs.y, z += rhs.z;
		return *this;
	}
	template<typename T>
	Quaternion<T> &Quaternion<T>::operator-=(const Quaternion<T> &rhs)
	{
		w -= rhs.w, x -= rhs.x, y -= rhs.y, z -= rhs.z;
		return *this;
	}
	template<typename T>
	Quaternion<T> &Quaternion<T>::operator*=(const Quaternion<T> &rhs)
	{
		Quaternion<T> temp(
			(w * rhs.w) - (x * rhs.x) - (y * rhs.y) - (z * rhs.z),
			(w * rhs.x) + (x * rhs.w) - (y * rhs.z) + (z * rhs.y),
			(w * rhs.y) + (x * rhs.z) + (y * rhs.w) - (z * rhs.x),
			(w * rhs.z) - (x * rhs.y) + (y * rhs.x) + (z * rhs.w));

		*this = temp;

		return *this;
	}
	template<typename T>
	Quaternion<T> &Quaternion<T>::operator*=(T scalar)
	{
		w *= scalar, x *= scalar, y *= scalar, z *= scalar;
		return *this;
	}
	template<typename T>
	Quaternion<T> &Quaternion<T>::operator/=(T scalar)
	{
		w /= scalar, x /= scalar, y /= scalar, z /= scalar;
		return *this;
	}
	template<typename T>
	Quaternion<T> Quaternion<T>::operator+(const Quaternion<T> &rhs) const
	{
		Quaternion<T> temp(*this);
		temp += rhs;
		return temp;
	}
	template<typename T>
	Quaternion<T> Quaternion<T>::operator-(const Quaternion<T> &rhs) const
	{
		Quaternion<T> temp(*this);
		temp -= rhs;
		return temp;
	}
	template<typename T>
	Quaternion<T> Quaternion<T>::operator*(const Quaternion<T> &rhs) const
	{
		Quaternion<T> temp(*this);
		temp *= rhs;
		return temp;
	}
	template<typename T>
	Quaternion<T> Quaternion<T>::operator*(T scalar) const
	{
		Quaternion<T> temp(*this);
		temp *= scalar;
		return temp;
	}
	template<typename T>
	Quaternion<T> Quaternion<T>::operator/(T scalar) const
	{
		Quaternion<T> temp(*this);
		temp /= scalar;
		return temp;
	}


}