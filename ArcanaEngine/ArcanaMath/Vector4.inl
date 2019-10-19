
namespace Arcana
{
	//---------------------------------------------------Static Methods---------------------------------------------------//

	template<typename T>
	T Vector4<T>::distance(const Vector4<T> &pt1, const Vector4<T> &pt2)
	{
		return (T)sqrt(distanceSq(pt1, pt2));
	}

	template<typename T>
	T Vector4<T>::distanceSq(const Vector4<T> &pt1, const Vector4<T> &pt2)
	{
		return ((pt1.x - pt2.x) * (pt1.x - pt2.x))
			+ ((pt1.y - pt2.y) * (pt1.y - pt2.y))
			+ ((pt1.z - pt2.z) * (pt1.z - pt2.z))
			+ ((pt1.w - pt2.w) * (pt1.w - pt2.w));
	}

	template<typename T>
	T Vector4<T>::dot(const Vector4<T> &p, const Vector4<T> &q)
	{
		return (p.x * q.x) + (p.y * q.y) + (p.z * q.z) + (p.w * q.w);
	}

	template<typename T>
	Vector4<T> Vector4<T>::lerp(const Vector4<T> &p, const Vector4<T> &q, T t)
	{
		return p + t * (q - p);
	}

	template<typename T>
	void Vector4<T>::orthogonalize(Vector4<T> &v1, Vector4<T> &v2)
	{
		v1.normalize();

		v2 = v2 - proj(v2, v1);
		v2.normalize();
	}

	template<typename T>
	void Vector4<T>::orthogonalize(Vector4<T> &v1, Vector4<T> &v2, Vector4<T> &v3)
	{
		v1.normalize();

		v2 = v2 - proj(v2, v1);
		v2.normalize();

		v3 = v3 - proj(v3, v1) - proj(v3, v2);
		v3.normalize();
	}

	template<typename T>
	Vector4<T> Vector4<T>::proj(const Vector4<T> &p, const Vector4<T> &q)
	{
		double length = q.magnitude();
		return q * (Vector4<T>::dot(p, q) / (length * length));
	}

	template<typename T>
	Vector4<T> Vector4<T>::perp(const Vector4<T> &p, const Vector4<T> &q)
	{
		double length = q.magnitude();
		return p - (q * (Vector4<T>::dot(p, q) / (length * length)));
	}

	template<typename T>
	Vector4<T> Vector4<T>::reflect(const Vector4<T> &i, const Vector4<T> &n)
	{
		return i - (T)2 * Vector4<T>::proj(i, n);
	}

	template<typename T>
	Vector4<T> Vector4<T>::normalize(const Vector4<T> &i)
	{
		double invMag = (T)1 / i.magnitude();
		return Vector4<T>(i.x * invMag, i.y * invMag, i.z * invMag, i.w * invMag);
	}

	template<typename T>
	Vector4<T> Vector4<T>::zero()
	{
		return Vector4<T>((T)0, (T)0, (T)0, (T)0);
	}

	template<typename T>
	Vector4<T> Vector4<T>::one()
	{
		return Vector4<T>((T)1, (T)1, (T)1, (T)1);
	}

	template<typename T>
	const Vector4<T>& Vector4<T>::unitX()
	{
		static Vector4<T> i((T)1, (T)0, (T)0, (T)0);
		return i;
	}

	template<typename T>
	const Vector4<T>& Vector4<T>::unitY()
	{
		static Vector4<T> j((T)0, (T)1, (T)0, (T)0);
		return j;
	}

	template<typename T>
	const Vector4<T>& Vector4<T>::unitZ()
	{
		static Vector4<T> k((T)0, (T)0, (T)1, (T)0);
		return k;
	}

	template<typename T>
	const Vector4<T>& Vector4<T>::unitW()
	{
		static Vector4<T> l((T)0, (T)0, (T)0, (T)0, (T)1);
		return l;
	}

	//---------------------------------------------------Class Methods---------------------------------------------------//

	template<typename T>
	Vector4<T>::Vector4()
		: x((T)0), y((T)0), z((T)0), w((T)0)
	{
	}

	template<typename T>
	Vector4<T>::Vector4(T x_, T y_, T z_, T w_)
		: x(x_), y(y_), z(z_), w(w_)
	{
	}

	template<typename T>
	Vector4<T>::Vector4(T v)
		: x(v), y(v), z(v), w(v)
	{
	}

	template<typename T>
	Vector4<T>::Vector4(const Vector4<T>& p1, const Vector4<T>& p2)
	{
		x = p2.x - p1.x;
		y = p2.y - p1.y;
		z = p2.z - p1.z;
		w = p2.w - p1.w;
	}

	template<typename T>
	Vector4<T>::~Vector4()
	{
	}

	template<typename T>
	bool Vector4<T>::isZero() const
	{
		return x == (T)0 && y == (T)0 && z == (T)0 && w == (T)0;
	}

	template<typename T>
	bool Vector4<T>::isOne() const
	{
		return x == (T)1 && y == (T)1 && z == (T)1 && w == (T)1;
	}

	template<typename T>
	void Vector4<T>::smooth(const Vector4<T>& target, T elapsedTime, T responseTime)
	{
		if (elapsedTime > 0)
		{
			*this += (target - *this) * (elapsedTime / (elapsedTime + responseTime));
		}
	}

	template<typename T>
	T Vector4<T>::magnitude() const
	{
		return sqrt((x * x) + (y * y) + (z * z) + (w * w));
	}

	template<typename T>
	T Vector4<T>::magnitudeSq() const
	{
		return (x * x) + (y * y) + (z * z) + (w * w);
	}

	template<typename T>
	Vector4<T> Vector4<T>::inverse() const
	{
		return Vector4<T>(-x, -y, -z, -w);
	}

	template<typename T>
	void Vector4<T>::normalize()
	{
		T invMag = 1 / magnitude();
		x *= invMag, y *= invMag, z *= invMag, w *= invMag;
	}

	template<typename T>
	void Vector4<T>::set(T x_, T y_, T z_, T w_)
	{
		x = x_, y = y_, z = z_, w = w_;
	}

	template<typename T>
	void Vector4<T>::set(const Vector4<T>& v)
	{
		x = v.x, y = v.y, z = v.z, w = v.w;
	}

	template<typename T>
	void Vector4<T>::scale(T s)
	{
		x *= s;
		y *= s;
		z *= s;
		w *= s;
	}

	template<typename T>
	Vector3<T> Vector4<T>::xyz() const
	{
		return Vector3<T>(x, y, z);
	}

	//---------------------------------------------------Operators---------------------------------------------------//

	template<typename T>
	Vector4<T> operator*(T lhs, const Vector4<T> &rhs)
	{
		return Vector4<T>(lhs * rhs.x, lhs * rhs.y, lhs * rhs.z, lhs * rhs.w);
	};

	template<typename T>
	Vector4<T> operator*(const Vector4<T> &lhs, const Vector4<T> &rhs)
	{
		return Vector4<T>(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z, lhs.w * rhs.w);
	};

	template<typename T>
	Vector4<T> operator-(const Vector4<T> &v)
	{
		return Vector4<T>(-v.x, -v.y, -v.z, -v.w);
	};

	template<typename T>
	bool Vector4<T>::operator==(const Vector4<T> &rhs) const
	{
		return Math::closeEnough(x, rhs.x) && Math::closeEnough(y, rhs.y) 
			&& Math::closeEnough(z, rhs.z) && Math::closeEnough(w, rhs.w);
	}

	template<typename T>
	bool Vector4<T>::operator!=(const Vector4<T> &rhs) const
	{
		return !(*this == rhs);
	}

	template<typename T>
	Vector4<T> &Vector4<T>::operator+=(const Vector4<T> &rhs)
	{
		x += rhs.x, y += rhs.y, z += rhs.z, w += rhs.w;
		return *this;
	}

	template<typename T>
	Vector4<T> &Vector4<T>::operator-=(const Vector4<T> &rhs)
	{
		x -= rhs.x, y -= rhs.y, z -= rhs.z, w -= rhs.w;
		return *this;
	}

	template<typename T>
	Vector4<T> &Vector4<T>::operator*=(T scalar)
	{
		x *= scalar, y *= scalar, z *= scalar, w *= scalar;
		return *this;
	}

	template<typename T>
	Vector4<T> &Vector4<T>::operator/=(T scalar)
	{
		x /= scalar, y /= scalar, z /= scalar, w /= scalar;
		return *this;
	}

	template<typename T>
	Vector4<T> Vector4<T>::operator+(const Vector4<T> &rhs) const
	{
		Vector4<T> tmp(*this);
		tmp += rhs;
		return tmp;
	}

	template<typename T>
	Vector4<T> Vector4<T>::operator-(const Vector4<T> &rhs) const
	{
		Vector4<T> tmp(*this);
		tmp -= rhs;
		return tmp;
	}

	template<typename T>
	Vector4<T> Vector4<T>::operator*(T scalar) const
	{
		return Vector4<T>(x * scalar, y * scalar, z * scalar, w * scalar);
	}

	template<typename T>
	Vector4<T> Vector4<T>::operator/(T scalar) const
	{
		return Vector4<T>(x / scalar, y / scalar, z / scalar, w / scalar);
	}
}