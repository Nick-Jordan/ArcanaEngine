#ifndef VECTOR2_H_
#define VECTOR2_H_

#include "ArcanaMath.h"

namespace Arcana
{
	/** \brief A 2-component vector class that represents a row vector.
	*/

	template<typename T>
	class Vector2
	{
		template<typename T>
		friend Vector2<T> operator*(T lhs, const Vector2<T> &rhs);
		template<typename T>
		friend Vector2<T> operator*(const Vector2<T> &lhs, const Vector2<T> &rhs);
		template<typename T>
		friend Vector2<T> operator-(const Vector2<T> &v);
	public:
		T x, y;

		/** \brief Calculates the distance between two vectors.
		*/
		static T distance(const Vector2<T> &pt1, const Vector2<T> &pt2);
		/** \brief Calculates the squared distance between two vectors.
		*/
		static T distanceSq(const Vector2<T> &pt1, const Vector2<T> &pt2);
		/** \brief Calculates the dot product of two 2-component vectors.
		*/
		static T dot(const Vector2<T> &p, const Vector2<T> &q);
		/** \brief Performs a linear interpolation between two 2d points.
		*/
		static Vector2<T> lerp(const Vector2<T> &p, const Vector2<T> &q, T t);
		/** \brief Turns two vectors into orthonormal basis vectors.
		*  Performs Gram-Schmidt Orthogonalization on the 2 basis vectors to turn them into orthonormal basis vectors.
		*
		*/
		static void orthogonalize(Vector2<T> &v1, Vector2<T> &v2);
		/** \brief Calculates the projection of vector 'p' onto vector 'q'
		*  v = dot(p, q) / |q|^2 * q
		*/
		static Vector2<T> proj(const Vector2<T> &p, const Vector2<T> &q);
		/** \brief Calculates the component of vector 'p' perpendicular to vector 'q'.
		*  v = p - dot(p, q) / |q|^2 * q
		*/
		static Vector2<T> perp(const Vector2<T> &p, const Vector2<T> &q);
		/** \brief Calculates reflection vector from entering ray direction 'i' and surface normal 'n'.
		*
		*/
		static Vector2<T> reflect(const Vector2<T> &i, const Vector2<T> &n);
		/** \brief Normalizes a vector.
		*  The vector is multiplied by the multiplicative inverse of its magnitude.
		*/
		static Vector2<T> normalize(const Vector2<T> &i);

		/** \brief Returns the zero vector.
		*/
		static Vector2<T> zero();
		/** \brief Returns the one vector.
		*/
		static Vector2<T> one();

		/** \brief Returns the unit-x vector.
		*  vector = i = <1, 0>
		*/
		static const Vector2<T>& unitX();
		/** \brief Returns the unit-y vector.
		*  vector = j = <0, 1>
		*/
		static const Vector2<T>& unitY();

		/** \brief Default vector constructor.
		*  Initializes the vector to the zero vector.
		*/
		Vector2();
		/** \brief Basic vector constructor.
		*  Initializes all three components of the vector.
		*/
		Vector2(T x_, T y_);
		/** \brief Single value vector constructor.
		*  Initializes all two components of the vector to a single value.
		*/
		Vector2(T v);
		/** \brief Direction constructor.
		*  Initializes the vector to the direction from 'p1' to 'p2'.
		*/
		Vector2(const Vector2<T>& p1, const Vector2<T>& p2);
		/** \brief Copy constructor.
		 */
		//Vector2(const Vector2<T>& copy);
		/** \brief Default destructor.
		 */
		~Vector2();

		/** \brief Checks if the vector is equal to the zero vector.
		*/
		bool isZero() const;
		/** \brief Checks if the vector is equal to the one vector.
		*/
		bool isOne() const;

		/** \brief Add description of smooth
		*/
		void smooth(const Vector2<T>& target, T elapsedTime, T responseTime);
		/** \brief Returns the magnitude of the vector.
		*  |v| = sqrt(x^2 + y^2)
		*/
		T magnitude() const;
		/** \brief Returns the magnitude of the vector squared.
		*  m = x^2 + y^2
		*/
		T magnitudeSq() const;
		/** \brief Returns the inverse of the vector.
		*  v = <-x, -y>
		*/
		Vector2<T> inverse() const;
		/** \brief Normalizes the vector.
		*  v = v / |v|
		*/
		void normalize();
		/** \brief Sets the vector's components to new values.
		*/
		void set(T x_, T y_);
		/** \brief Sets the vector's components to the components of another vector.
		*/
		void set(const Vector2<T>& v);
		/** \brief Scales the vector components by a scalar value.
		*/
		void scale(T s);

		/** \brief Casts the vector to another type.
		*/
		template<typename N>
		Vector2<N> cast()
		{
			return Vector2<N>((N)x, (N)y);
		}

		/** \brief Determines if the vector equals another vector.
		*  Uses Math::closeEnough to determine equality of vector components.
		*/
		bool operator==(const Vector2<T> &rhs) const;
		/** \brief Determines if the vector does not equal another vector.
		*  Returns the inverse of the '==' operator;
		*/
		bool operator!=(const Vector2<T> &rhs) const;

		/** \brief Adds a vector.
		*/
		Vector2<T> &operator+=(const Vector2<T> &rhs);
		/** \brief Subtracts a vector.
		*/
		Vector2<T> &operator-=(const Vector2<T> &rhs);
		/** \brief Scales the vector's components by a scalar value.
		*/
		Vector2<T> &operator*=(T scalar);
		/** \brief Scales the vector's components by the inverse of a scalar value.
		*/
		Vector2<T> &operator/=(T scalar);

		/** \brief Adds a vector.
		*/
		Vector2<T> operator+(const Vector2<T> &rhs) const;
		/** \brief Subtracts a vector.
		*/
		Vector2<T> operator-(const Vector2<T> &rhs) const;
		/** \brief Scales the vector's components by a scalar value.
		*/
		Vector2<T> operator*(T scalar) const;
		/** \brief Scales the vector's components by the inverse of a scalar value.
		*/
		Vector2<T> operator/(T scalar) const;

		/** \brief Evaluates if the left-hand side vector is less than the right-hand side vector.
		*/

		friend bool operator < (Vector2<T> const& lhs, Vector2<T> const& rhs)
		{
			return lhs.x < rhs.x && lhs.y < rhs.y;
		}
	};

	typedef Vector2<float> Vector2f;
	typedef Vector2<double> Vector2d;
	typedef Vector2<int> Vector2i;
}

#include "Vector2.inl"

#undef ARCANA_MATH_EXPORTS

#endif // !VECTOR2_H_