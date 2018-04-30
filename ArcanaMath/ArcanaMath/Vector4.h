#ifndef VECTOR4_H_
#define VECTOR4_H_

#include "ArcanaMath.h"

#define ARCANA_MATH_EXPORTS

#ifdef ARCANA_MATH_EXPORTS  
#define ARCANA_MATH_API __declspec(dllexport)   
#else  
#define ARCANA_MATH_API __declspec(dllimport)   
#endif  

namespace Arcana
{
	/** \brief A 4-component vector class that represents a row vector.
	*/

	template<typename T>
	class ARCANA_MATH_API Vector4
	{
		template<typename T>
		friend Vector4<T> operator*(T lhs, const Vector4<T> &rhs);
		template<typename T>
		friend Vector4<T> operator*(const Vector4<T> &lhs, const Vector4<T> &rhs);
		template<typename T>
		friend Vector4<T> operator-(const Vector4<T> &v);
	public:
		T x, y, z, w;

		/** \brief Calculates the distance between two vectors.
		*/
		static T distance(const Vector4<T> &pt1, const Vector4<T> &pt2);
		/** \brief Calculates the squared distance between two vectors.
		*/
		static T distanceSq(const Vector4<T> &pt1, const Vector4<T> &pt2);
		/** \brief Calculates the dot product of two 4-component vectors.
		*/
		static T dot(const Vector4<T> &p, const Vector4<T> &q);
		/** \brief Performs a linear interpolation between two 4d points.
		*/
		static Vector4<T> lerp(const Vector4<T> &p, const Vector4<T> &q, T t);
		/** \brief Turns two vectors into orthonormal basis vectors.
		*  Performs Gram-Schmidt Orthogonalization on the 2 basis vectors to turn them into orthonormal basis vectors.
		*
		*/
		static void orthogonalize(Vector4<T> &v1, Vector4<T> &v2);
		/** \brief Turns three vectors into orthonormal basis vectors.
		*  Performs Gram-Schmidt Orthogonalization on the 3 basis vectors to turn them into orthonormal basis vectors.
		*
		*/
		static void orthogonalize(Vector4<T> &v1, Vector4<T> &v2, Vector4<T> &v3);
		/** \brief Calculates the projection of vector 'p' onto vector 'q'
		*  v = dot(p, q) / |q|^2 * q
		*/
		static Vector4<T> proj(const Vector4<T> &p, const Vector4<T> &q);
		/** \brief Calculates the component of vector 'p' perpendicular to vector 'q'.
		*  v = p - dot(p, q) / |q|^2 * q
		*/
		static Vector4<T> perp(const Vector4<T> &p, const Vector4<T> &q);
		/** \brief Calculates reflection vector from entering ray direction 'i' and surface normal 'n'.
		*
		*/
		static Vector4<T> reflect(const Vector4<T> &i, const Vector4<T> &n);
		/** \brief Normalizes a vector.
		*  The vector is multiplied by the multiplicative inverse of its magnitude.
		*/
		static Vector4<T> normalize(const Vector4<T> &i);

		/** \brief Returns the zero vector.
		*/
		static Vector4<T> zero();
		/** \brief Returns the one vector.
		*/
		static Vector4<T> one();

		/** \brief Returns the unit-x vector.
		*  vector = i = <1, 0, 0, 0>
		*/
		static const Vector4<T>& unitX();
		/** \brief Returns the unit-y vector.
		*  vector = j = <0, 1, 0, 0>
		*/
		static const Vector4<T>& unitY();
		/** \brief Returns the unit-z vector.
		*  vector = k = <0, 0, 1, 0>
		*/
		static const Vector4<T>& unitZ();
		/** \brief Returns the unit-w vector.
		*  vector = l = <0, 0, 0, 1>
		*/
		static const Vector4<T>& unitW();

		/** \brief Default vector constructor.
		*  Initializes the vector to the zero vector.
		*/
		Vector4();
		/** \brief Basic vector constructor.
		*  Initializes all four components of the vector.
		*/
		Vector4(T x_, T y_, T z_, T w_);
		/** \brief Single value vector constructor.
		*  Initializes all four components of the vector to a single value.
		*/
		Vector4(T v);
		/** \brief Direction constructor.
		*  Initializes the vector to the direction from 'p1' to 'p2'.
		*/
		Vector4(const Vector4<T>& p1, const Vector4<T>& p2);
		/** \brief Default destructor.
		*/
		~Vector4();

		/** \brief Checks if the vector is equal to the zero vector.
		*/
		bool isZero() const;
		/** \brief Checks if the vector is equal to the one vector.
		*/
		bool isOne() const;

		/** \brief Add description of smooth
		*/
		void smooth(const Vector4<T>& target, T elapsedTime, T responseTime);
		/** \brief Returns the magnitude of the vector.
		*  |v| = sqrt(x^2 + y^2 + z^2 + w^2)
		*/
		T magnitude() const;
		/** \brief Returns the magnitude of the vector squared.
		*  m = x^2 + y^2 + z^2 + w^2
		*/
		T magnitudeSq() const;
		/** \brief Returns the inverse of the vector.
		*  v = <-x, -y, -z, -w>
		*/
		Vector4<T> inverse() const;
		/** \brief Normalizes the vector.
		*  v = v / |v|
		*/
		void normalize();
		/** \brief Sets the vector's components to new values.
		*/
		void set(T x_, T y_, T z_, T w_);
		/** \brief Sets the vector's components to the components of another vector.
		*/
		void set(const Vector4<T>& v);
		/** \brief Scales the vector components by a scalar value.
		*/
		void scale(T s);

		/** \brief Casts the vector to another type.
		*/
		template<typename N>
		Vector4<N> cast()
		{
			return Vector4<N>((N)x, (N)y, (N)z, (N)w);
		}

		/** \brief Determines if the vector equals another vector.
		*  Uses Math::closeEnough to determine equality of vector components.
		*/
		bool operator==(const Vector4<T> &rhs) const;
		/** \brief Determines if the vector does not equal another vector.
		*  Returns the inverse of the '==' operator;
		*/
		bool operator!=(const Vector4<T> &rhs) const;

		/** \brief Adds a vector.
		*/
		Vector4<T> &operator+=(const Vector4<T> &rhs);
		/** \brief Subtracts a vector.
		*/
		Vector4<T> &operator-=(const Vector4<T> &rhs);
		/** \brief Scales the vector's components by a scalar value.
		*/
		Vector4<T> &operator*=(T scalar);
		/** \brief Scales the vector's components by the inverse of a scalar value.
		*/
		Vector4<T> &operator/=(T scalar);

		/** \brief Adds a vector.
		*/
		Vector4<T> operator+(const Vector4<T> &rhs) const;
		/** \brief Subtracts a vector.
		*/
		Vector4<T> operator-(const Vector4<T> &rhs) const;
		/** \brief Scales the vector's components by a scalar value.
		*/
		Vector4<T> operator*(T scalar) const;
		/** \brief Scales the vector's components by the inverse of a scalar value.
		*/
		Vector4<T> operator/(T scalar) const;

		/** \brief Evaluates if the left-hand side vector is less than the right-hand side vector.
		*/

		friend bool operator < (Vector4<T> const& lhs, Vector4<T> const& rhs)
		{
			return lhs.x < rhs.x && lhs.y < rhs.y && lhs.z < rhs.z && lhs.w < rhs.w;
		}
	};

	template<typename T>
	inline Vector4<T> operator*(T lhs, const Vector4<T> &rhs)
	{
		return Vector4<T>(lhs * rhs.x, lhs * rhs.y, lhs * rhs.z, lhs * rhs.w);
	};

	template<typename T>
	inline Vector4<T> operator*(const Vector4<T> &lhs, const Vector4<T> &rhs)
	{
		return Vector4<T>(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z, lhs.w * rhs.w);
	};

	template<typename T>
	inline Vector4<T> operator-(const Vector4<T> &v)
	{
		return Vector4<T>(-v.x, -v.y, -v.z, -v.w);
	};

	typedef Vector4<float> Vector4f;
	typedef Vector4<double> Vector4d;
	typedef Vector4<int> Vector4i;
}

#include "Vector4.inl"

#undef ARCANA_MATH_EXPORTS

#endif // !VECTOR4_H_