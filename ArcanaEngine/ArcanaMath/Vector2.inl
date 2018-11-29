
namespace Arcana
{
	//---------------------------------------------------Static Methods---------------------------------------------------//
	template<typename T>
	T Vector2<T>::distance(const Vector2<T> &pt1, const Vector2<T> &pt2)
	{
		return (T)sqrt(distanceSq(pt1, pt2));
	}

	template<typename T>
	T Vector2<T>::distanceSq(const Vector2<T> &pt1, const Vector2<T> &pt2)
	{
		return ((pt1.x - pt2.x) * (pt1.x - pt2.x))
			+ ((pt1.y - pt2.y) * (pt1.y - pt2.y));
	}

	template<typename T>
	T Vector2<T>::dot(const Vector2<T> &p, const Vector2<T> &q)
	{
		return (p.x * q.x) + (p.y * q.y);
	}

	template<typename T>
	Vector2<T> Vector2<T>::lerp(const Vector2<T> &p, const Vector2<T> &q, T t)
	{
		return p + t * (q - p);
	}

	template<typename T>
	void Vector2<T>::orthogonalize(Vector2<T> &v1, Vector2<T> &v2)
	{
		v2 = v2 - proj(v2, v1);
		v2.normalize();
	}

	template<typename T>
	Vector2<T> Vector2<T>::proj(const Vector2<T> &p, const Vector2<T> &q)
	{
		double length = q.magnitude();
		return (Vector2<T>::dot(p, q) / (length * length)) * q;
	}

	template<typename T>
	Vector2<T> Vector2<T>::perp(const Vector2<T> &p, const Vector2<T> &q)
	{
		double length = q.magnitude();
		return p - ((Vector2<T>::dot(p, q) / (length * length)) * q);
	}

	template<typename T>
	Vector2<T> Vector2<T>::reflect(const Vector2<T> &i, const Vector2<T> &n)
	{
		return i - (T)2 * Vector2<T>::proj(i, n);
	}

	template<typename T>
	Vector2<T> Vector2<T>::normalize(const Vector2<T> &i)
	{
		double invMag = (T)1 / i.magnitude();
		return Vector2<T>(i.x * invMag, i.y * invMag);
	}

	template<typename T>
	Vector2<T> Vector2<T>::zero()
	{
		return Vector2<T>((T)0, (T)0);
	}

	template<typename T>
	Vector2<T> Vector2<T>::one()
	{
		return Vector2<T>((T)1, (T)1);
	}

	template<typename T>
	const Vector2<T>& Vector2<T>::unitX()
	{
		static Vector2<T> i((T)1, (T)0);
		return i;
	}

	template<typename T>
	const Vector2<T>& Vector2<T>::unitY()
	{
		static Vector2<T> j((T)0, (T)1);
		return j;
	}

	//---------------------------------------------------Class Methods---------------------------------------------------//

	template<typename T>
	Vector2<T>::Vector2()
		: x((T)0), y((T)0)
	{
	}

	template<typename T>
	Vector2<T>::Vector2(T x_, T y_)
		: x(x_), y(y_)
	{
	}

	template<typename T>
	Vector2<T>::Vector2(T v)
		: x(v), y(v)
	{
	}

	template<typename T>
	Vector2<T>::Vector2(const Vector2<T>& p1, const Vector2<T>& p2)
	{
		x = p2.x - p1.x;
		y = p2.y - p1.y;
	}

	/*template<typename T>
	Vector2<T>::Vector2(const Vector2<T>& copy) : x(copy.x), y(copy.y)
	{

	}*/

	template<typename T>
	Vector2<T>::~Vector2()
	{
	}

	template<typename T>
	bool Vector2<T>::isZero() const
	{
		return x == (T)0 && y == (T)0;
	}

	template<typename T>
	bool Vector2<T>::isOne() const
	{
		return x == (T)1 && y == (T)1;
	}

	template<typename T>
	void Vector2<T>::smooth(const Vector2<T>& target, T elapsedTime, T responseTime)
	{
		if (elapsedTime > 0)
		{
			*this += (target - *this) * (elapsedTime / (elapsedTime + responseTime));
		}
	}

	template<typename T>
	T Vector2<T>::magnitude() const
	{
		return sqrt((x * x) + (y * y));
	}

	template<typename T>
	T Vector2<T>::magnitudeSq() const
	{
		return (x * x) + (y * y);
	}

	template<typename T>
	Vector2<T> Vector2<T>::inverse() const
	{
		return Vector2<T>(-x, -y);
	}

	template<typename T>
	void Vector2<T>::normalize()
	{
		T invMag = 1 / magnitude();
		x *= invMag, y *= invMag;
	}

	template<typename T>
	void Vector2<T>::set(T x_, T y_)
	{
		x = x_, y = y_;
	}

	template<typename T>
	void Vector2<T>::set(const Vector2<T>& v)
	{
		x = v.x, y = v.y;
	}

	template<typename T>
	void Vector2<T>::scale(T s)
	{
		x *= s;
		y *= s;
	}

	//---------------------------------------------------Operators---------------------------------------------------//

	template<typename T>
	Vector2<T> operator*(T lhs, const Vector2<T> &rhs)
	{
		return Vector2<T>(lhs * rhs.x, lhs * rhs.y);
	};

	template<typename T>
	Vector2<T> operator*(const Vector2<T> &lhs, const Vector2<T> &rhs)
	{
		return Vector2<T>(lhs.x * rhs.x, lhs.y * rhs.y);
	};

	template<typename T>
	Vector2<T> operator-(const Vector2<T> &v)
	{
		return Vector2<T>(-v.x, -v.y);
	};

	template<typename T>
	bool Vector2<T>::operator==(const Vector2<T> &rhs) const
	{
		return Math::closeEnough(x, rhs.x) && Math::closeEnough(y, rhs.y) && Math::closeEnough(z, rhs.z);
	}

	template<typename T>
	bool Vector2<T>::operator!=(const Vector2<T> &rhs) const
	{
		return !(*this == rhs);
	}

	template<typename T>
	Vector2<T> &Vector2<T>::operator+=(const Vector2<T> &rhs)
	{
		x += rhs.x, y += rhs.y;
		return *this;
	}

	template<typename T>
	Vector2<T> &Vector2<T>::operator-=(const Vector2<T> &rhs)
	{
		x -= rhs.x, y -= rhs.y;
		return *this;
	}

	template<typename T>
	Vector2<T> &Vector2<T>::operator*=(T scalar)
	{
		x *= scalar, y *= scalar;
		return *this;
	}

	template<typename T>
	Vector2<T> &Vector2<T>::operator/=(T scalar)
	{
		x /= scalar, y /= scalar;
		return *this;
	}

	template<typename T>
	Vector2<T> Vector2<T>::operator+(const Vector2<T> &rhs) const
	{
		Vector2<T> tmp(*this);
		tmp += rhs;
		return tmp;
	}

	template<typename T>
	Vector2<T> Vector2<T>::operator-(const Vector2<T> &rhs) const
	{
		Vector2<T> tmp(*this);
		tmp -= rhs;
		return tmp;
	}

	template<typename T>
	Vector2<T> Vector2<T>::operator*(T scalar) const
	{
		return Vector2<T>(x * scalar, y * scalar);
	}

	template<typename T>
	Vector2<T> Vector2<T>::operator/(T scalar) const
	{
		return Vector2<T>(x / scalar, y / scalar);
	}
}