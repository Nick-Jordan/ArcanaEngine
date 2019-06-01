
namespace Arcana
{
	//---------------------------------------------------Static Methods---------------------------------------------------//
	template<typename T>
	Vector3<T> Vector3<T>::cross(const Vector3<T> &p, const Vector3<T> &q)
	{
		return Vector3<T>((p.y * q.z) - (p.z * q.y),
			(p.z * q.x) - (p.x * q.z),
			(p.x * q.y) - (p.y * q.x));
	}

	template<typename T>
	T Vector3<T>::distance(const Vector3<T> &pt1, const Vector3<T> &pt2)
	{
		return (T)sqrt(distanceSq(pt1, pt2));
	}

	template<typename T>
	T Vector3<T>::distanceSq(const Vector3<T> &pt1, const Vector3<T> &pt2)
	{
		return ((pt1.x - pt2.x) * (pt1.x - pt2.x))
			+ ((pt1.y - pt2.y) * (pt1.y - pt2.y))
			+ ((pt1.z - pt2.z) * (pt1.z - pt2.z));
	}

	template<typename T>
	T Vector3<T>::dot(const Vector3<T> &p, const Vector3<T> &q)
	{
		return (p.x * q.x) + (p.y * q.y) + (p.z * q.z);
	}

	template<typename T>
	Vector3<T> Vector3<T>::lerp(const Vector3<T> &p, const Vector3<T> &q, T t)
	{
		return p + t * (q - p);
	}

	template<typename T>
	void Vector3<T>::orthogonalize(Vector3<T> &v1, Vector3<T> &v2)
	{
		v1.normalize();

		v2 = v2 - proj(v2, v1);
		v2.normalize();
	}

	template<typename T>
	void Vector3<T>::orthogonalize(Vector3<T> &v1, Vector3<T> &v2, Vector3<T> &v3)
	{
		v1.normalize();

		v2 = v2 - proj(v2, v1);
		v2.normalize();

		v3 = v3 - proj(v3, v1) - proj(v3, v2);
		v3.normalize();
	}

	template<typename T>
	Vector3<T> Vector3<T>::proj(const Vector3<T> &p, const Vector3<T> &q)
	{
		double length = q.magnitude();
		return q * (Vector3<T>::dot(p, q) / (length * length));
	}

	template<typename T>
	Vector3<T> Vector3<T>::perp(const Vector3<T> &p, const Vector3<T> &q)
	{
		double length = q.magnitude();
		return p - (q * (Vector3<T>::dot(p, q) / (length * length)));
	}

	template<typename T>
	Vector3<T> Vector3<T>::reflect(const Vector3<T> &i, const Vector3<T> &n)
	{
		return i - (T)2 * Vector3<T>::proj(i, n);
	}

	template<typename T>
	Vector3<T> Vector3<T>::normalize(const Vector3<T> &i)
	{
		double invMag = (T)1 / i.magnitude();
		return Vector3<T>(i.x * invMag, i.y * invMag, i.z * invMag);
	}

	template<typename T>
	Vector3<T> Vector3<T>::zero()
	{
		return Vector3<T>((T)0, (T)0, (T)0);
	}

	template<typename T>
	Vector3<T> Vector3<T>::one()
	{
		return Vector3<T>((T)1, (T)1, (T)1);
	}

	template<typename T>
	const Vector3<T>& Vector3<T>::unitX()
	{
		static Vector3<T> i((T)1, (T)0, (T)0);
		return i;
	}

	template<typename T>
	const Vector3<T>& Vector3<T>::unitY()
	{
		static Vector3<T> j((T)0, (T)1, (T)0);
		return j;
	}

	template<typename T>
	const Vector3<T>& Vector3<T>::unitZ()
	{
		static Vector3<T> k((T)0, (T)0, (T)1);
		return k;
	}

	//---------------------------------------------------Class Methods---------------------------------------------------//

	template<typename T>
	Vector3<T>::Vector3()
		: x((T)0), y((T)0), z((T)0)
	{
	}

	template<typename T>
	Vector3<T>::Vector3(T x_, T y_, T z_) 
		: x(x_), y(y_), z(z_)  
	{
	}

	template<typename T>
	Vector3<T>::Vector3(T v)
		: x(v), y(v), z(v)
	{
	}

	template<typename T>
	Vector3<T>::Vector3(const Vector3<T>& p1, const Vector3<T>& p2)
	{
		x = p2.x - p1.x;
		y = p2.y - p1.y;
		z = p2.z - p1.z;
	}

	template<typename T>
	Vector3<T>::~Vector3()
	{
	}

	template<typename T>
	bool Vector3<T>::isZero() const
	{
		return x == (T)0 && y == (T)0 && z == (T)0;
	}

	template<typename T>
	bool Vector3<T>::isOne() const
	{
		return x == (T)1 && y == (T)1 && z == (T)1;
	}

	template<typename T>
	void Vector3<T>::smooth(const Vector3<T>& target, T elapsedTime, T responseTime)
	{
		if (elapsedTime > 0)
		{
			*this += (target - *this) * (elapsedTime / (elapsedTime + responseTime));
		}
	}

	template<typename T>
	T Vector3<T>::magnitude() const
	{
		return sqrt((x * x) + (y * y) + (z * z));
	}

	template<typename T>
	T Vector3<T>::magnitudeSq() const
	{
		return (x * x) + (y * y) + (z * z);
	}

	template<typename T>
	Vector3<T> Vector3<T>::inverse() const
	{
		return Vector3<T>(-x, -y, -z);
	}

	template<typename T>
	void Vector3<T>::normalize()
	{
		T invMag = 1 / magnitude();
		x *= invMag, y *= invMag, z *= invMag;
	}

	template<typename T>
	void Vector3<T>::set(T x_, T y_, T z_)
	{
		x = x_, y = y_, z = z_;
	}

	template<typename T>
	void Vector3<T>::set(const Vector3<T>& v)
	{
		x = v.x, y = v.y, z = v.z;
	}

	template<typename T>
	void Vector3<T>::scale(T s)
	{
		x *= s;
		y *= s;
		z *= s;
	}

	//---------------------------------------------------Operators---------------------------------------------------//

	template<typename T>
	Vector3<T> operator*(T lhs, const Vector3<T> &rhs)
	{
		return Vector3<T>(lhs * rhs.x, lhs * rhs.y, lhs * rhs.z);
	};

	template<typename T>
	Vector3<T> operator*(const Vector3<T> &lhs, const Vector3<T> &rhs)
	{
		return Vector3<T>(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z);
	};

	template<typename T>
	Vector3<T> operator-(const Vector3<T> &v)
	{
		return Vector3<T>(-v.x, -v.y, -v.z);
	};

	template<typename T>
	bool Vector3<T>::operator==(const Vector3<T> &rhs) const
	{
		return Math::closeEnough(x, rhs.x) && Math::closeEnough(y, rhs.y) && Math::closeEnough(z, rhs.z);
	}

	template<typename T>
	bool Vector3<T>::operator!=(const Vector3<T> &rhs) const
	{
		return !(*this == rhs);
	}

	template<typename T>
	Vector3<T> &Vector3<T>::operator+=(const Vector3<T> &rhs)
	{
		x += rhs.x, y += rhs.y, z += rhs.z;
		return *this;
	}

	template<typename T>
	Vector3<T> &Vector3<T>::operator-=(const Vector3<T> &rhs)
	{
		x -= rhs.x, y -= rhs.y, z -= rhs.z;
		return *this;
	}

	template<typename T>
	Vector3<T> &Vector3<T>::operator*=(T scalar)
	{
		x *= scalar, y *= scalar, z *= scalar;
		return *this;
	}

	template<typename T>
	Vector3<T> &Vector3<T>::operator/=(T scalar)
	{
		x /= scalar, y /= scalar, z /= scalar;
		return *this;
	}

	template<typename T>
	Vector3<T> Vector3<T>::operator+(const Vector3<T> &rhs) const
	{
		Vector3<T> tmp(*this);
		tmp += rhs;
		return tmp;
	}

	template<typename T>
	Vector3<T> Vector3<T>::operator-(const Vector3<T> &rhs) const
	{
		Vector3<T> tmp(*this);
		tmp -= rhs;
		return tmp;
	}

	template<typename T>
	Vector3<T> Vector3<T>::operator*(T scalar) const
	{
		return Vector3<T>(x * scalar, y * scalar, z * scalar);
	}

	template<typename T>
	Vector3<T> Vector3<T>::operator/(T scalar) const
	{
		return Vector3<T>(x / scalar, y / scalar, z / scalar);
	}
}