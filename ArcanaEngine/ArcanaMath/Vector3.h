#ifndef VECTOR3_H_
#define VECTOR3_H_

#include "ArcanaMath.h"

namespace Arcana
{
	/** \brief A 3-component vector class that represents a row vector.
	 */

	template<typename T>
	class Vector3
	{
		template<typename T>
		friend Vector3<T> operator*(T lhs, const Vector3<T> &rhs);
		template<typename T>
		friend Vector3<T> operator*(const Vector3<T> &lhs, const Vector3<T> &rhs);
		template<typename T>
		friend Vector3<T> operator-(const Vector3<T> &v);
	public:
		T x, y, z;

		/** \brief Calculates the cross product of two 3-component vectors.
		 */
		static Vector3<T> cross(const Vector3<T> &p, const Vector3<T> &q);
		/** \brief Calculates the distance between two vectors.
		 */
		static T distance(const Vector3<T> &pt1, const Vector3<T> &pt2);
		/** \brief Calculates the squared distance between two vectors.
		 */
		static T distanceSq(const Vector3<T> &pt1, const Vector3<T> &pt2);
		/** \brief Calculates the dot product of two 3-component vectors.
		 */
		static T dot(const Vector3<T> &p, const Vector3<T> &q);
		/** \brief Performs a linear interpolation between two 3d points.
		 */
		static Vector3<T> lerp(const Vector3<T> &p, const Vector3<T> &q, T t);
		/** \brief Turns two vectors into orthonormal basis vectors.
		 *  Performs Gram-Schmidt Orthogonalization on the 2 basis vectors to turn them into orthonormal basis vectors.
		 *  
		 */
		static void orthogonalize(Vector3<T> &v1, Vector3<T> &v2);
		/** \brief Turns three vectors into orthonormal basis vectors.
		 *  Performs Gram-Schmidt Orthogonalization on the 3 basis vectors to turn them into orthonormal basis vectors.
		 *
		 */
		static void orthogonalize(Vector3<T> &v1, Vector3<T> &v2, Vector3<T> &v3);
		/** \brief Calculates the projection of vector 'p' onto vector 'q'
		 *  v = dot(p, q) / |q|^2 * q
		 */
		static Vector3<T> proj(const Vector3<T> &p, const Vector3<T> &q);
		/** \brief Calculates the component of vector 'p' perpendicular to vector 'q'.
		 *  v = p - dot(p, q) / |q|^2 * q
		 */
		static Vector3<T> perp(const Vector3<T> &p, const Vector3<T> &q);
		/** \brief Calculates reflection vector from entering ray direction 'i' and surface normal 'n'.
		 * 
		 */
		static Vector3<T> reflect(const Vector3<T> &i, const Vector3<T> &n);
		/** \brief Normalizes a vector.
		 *  The vector is multiplied by the multiplicative inverse of its magnitude.
		 */
		static Vector3<T> normalize(const Vector3<T> &i);

		/** \brief Returns the zero vector.
		 */
		static Vector3<T> zero();
		/** \brief Returns the one vector.
		 */
		static Vector3<T> one();

		/** \brief Returns the unit-x vector.
		 *  vector = i = <1, 0, 0>
		 */
		static const Vector3<T>& unitX();
		/** \brief Returns the unit-y vector.
		 *  vector = j = <0, 1, 0>
		 */
		static const Vector3<T>& unitY();
		/** \brief Returns the unit-z vector.
		 *  vector = k = <0, 0, 1>
		 */
		static const Vector3<T>& unitZ();

		/** \brief Default vector constructor.
		 *  Initializes the vector to the zero vector.
		 */
		Vector3();
		/** \brief Basic vector constructor.
		 *  Initializes all three components of the vector.
		 */
		Vector3(T x_, T y_, T z_);
		/** \brief Single value vector constructor.
		 *  Initializes all three components of the vector to a single value.
		 */
		Vector3(T v);
		/** \brief Direction constructor.
		 *  Initializes the vector to the direction from 'p1' to 'p2'.
		 */
		Vector3(const Vector3<T>& p1, const Vector3<T>& p2);
		/** \brief Default destructor.
		 */
		~Vector3();

		/** \brief Checks if the vector is equal to the zero vector.
		 */
		bool isZero() const;
		/** \brief Checks if the vector is equal to the one vector.
		 */
		bool isOne() const;

		/** \brief Add description of smooth
		 */
		void smooth(const Vector3<T>& target, T elapsedTime, T responseTime);
		/** \brief Returns the magnitude of the vector.
		 *  |v| = sqrt(x^2 + y^2 + z^2)
		 */
		T magnitude() const;
		/** \brief Returns the magnitude of the vector squared.
		 *  m = x^2 + y^2 + z^2
		 */
		T magnitudeSq() const;
		/** \brief Returns the inverse of the vector.
		 *  v = <-x, -y, -z>
		 */
		Vector3<T> inverse() const;
		/** \brief Normalizes the vector.
		 *  v = v / |v|
		 */
		void normalize();
		/** \brief Sets the vector's components to new values.
		 */
		void set(T x_, T y_, T z_);
		/** \brief Sets the vector's components to the components of another vector.
		 */
		void set(const Vector3<T>& v);
		/** \brief Scales the vector components by a scalar value.
		 */
		void scale(T s);

		/** \brief Casts the vector to another type.
		 */
		template<typename N>
		Vector3<N> cast() const
		{
			return Vector3<N>((N)x, (N)y, (N)z);
		}

		/** \brief Determines if the vector equals another vector.
		 *  Uses Math::closeEnough to determine equality of vector components.
		 */
		bool operator==(const Vector3<T> &rhs) const;
		/** \brief Determines if the vector does not equal another vector.
		 *  Returns the inverse of the '==' operator;
		 */
		bool operator!=(const Vector3<T> &rhs) const;

		/** \brief Adds a vector.
		 */
		Vector3<T> &operator+=(const Vector3<T> &rhs);
		/** \brief Subtracts a vector.
		 */
		Vector3<T> &operator-=(const Vector3<T> &rhs);
		/** \brief Scales the vector's components by a scalar value.
		 */
		Vector3<T> &operator*=(T scalar);
		/** \brief Scales the vector's components by the inverse of a scalar value.
		 */
		Vector3<T> &operator/=(T scalar);

		/** \brief Adds a vector.
		*/
		Vector3<T> operator+(const Vector3<T> &rhs) const;
		/** \brief Subtracts a vector.
		*/
		Vector3<T> operator-(const Vector3<T> &rhs) const;
		/** \brief Scales the vector's components by a scalar value.
		*/
		Vector3<T> operator*(T scalar) const;
		/** \brief Scales the vector's components by the inverse of a scalar value.
		*/
		Vector3<T> operator/(T scalar) const;

		/** \brief Evaluates if the left-hand side vector is less than the right-hand side vector.
		 */

		friend bool operator < (Vector3<T> const& lhs, Vector3<T> const& rhs)
		{
			return lhs.x < rhs.x && lhs.y < rhs.y && lhs.z < rhs.z;
		}
	};

	typedef Vector3<float> Vector3f;
	typedef Vector3<double> Vector3d;
	typedef Vector3<int> Vector3i;
}

#include "Vector3.inl"

#undef ARCANA_MATH_EXPORTS

#endif // !VECTOR3_H_