
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

	}

	template<typename T>
	Quaternion<T>::Quaternion(const Vector3<T> &axis, T degrees)
	{

	}

	template<typename T>
	Quaternion<T>::Quaternion(const Matrix3<T> &m)
	{

	}

	template<typename T>
	Quaternion<T>::Quaternion(const Matrix4<T> &m)
	{

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


	//---------------------------------------------------Operators---------------------------------------------------//

	/*bool operator==(const Quaternion<T> &rhs) const;

	bool operator!=(const Quaternion<T> &rhs) const;

	Quaternion<T> &operator+=(const Quaternion<T> &rhs);

	Quaternion<T> &operator-=(const Quaternion<T> &rhs);

	Quaternion<T> &operator*=(const Quaternion<T> &rhs);

	Quaternion<T> &operator*=(T scalar);

	Quaternion<T> &operator/=(T scalar);

	Quaternion<T> operator+(const Quaternion<T> &rhs) const;

	Quaternion<T> operator-(const Quaternion<T> &rhs) const;

	Quaternion<T> operator*(const Quaternion<T> &rhs) const;

	Quaternion<T> operator*(T scalar) const;

	Quaternion<T> operator/(T scalar) const;*/

}