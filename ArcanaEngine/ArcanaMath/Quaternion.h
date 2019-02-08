#ifndef QUATERNION_H_
#define QUATERNION_H_

#include "Vector3.h"
#include "Matrix4.h"
#include "Matrix3.h"

namespace Arcana
{
	/** \brief A quaternion object mainly used for representing rotations.
	 *
	 *  A quaterion is a mathematical object represented by a 4-vector that behaves according to normed division albebra over the real numbers.
	 *  q = [qw, qx, qy, qz]
	 *  Rotation quaternions are defines as follows:
	 *  q = [cos(theta/2), a * sin(theta/2)], where a is the axis of rotation.
	 *  Quaternions can be converted to matrices for use with OpenGL.
	 */
	template<typename T>
	class Quaternion
	{
		template<typename T>
		friend Quaternion<T> operator*(const Quaternion<T> &lhs, T rhs);

		template<typename T>
		friend Vector3<T> operator*(const Quaternion<T> &lhs, const Vector3<T>& rhs);
		template<typename T>
		friend Vector4<T> operator*(const Quaternion<T> &lhs, const Vector4<T>& rhs);
		template<typename T>
		friend Vector3<T> operator*(const Vector3<T>& lhs, const Quaternion<T> &rhs);
		template<typename T>
		friend Vector4<T> operator*(const Vector4<T>& lhs, const Quaternion<T> &rhs);

	public:

		T w, x, y, z;

		/** \brief The identity quaternion.
		 *
		 *  Defined as q = [1, 0, 0, 0]
		 */
		static const Quaternion<T> IDENTITY;

		/** \brief Performs a spherical linear interpolation.
		 *
		 *  NOT YET IMPLEMENTED!
		 */
		static Quaternion<T> slerp(const Quaternion<T> &a, const Quaternion<T> &b, T t);

		/** \brief Quaternion default constructor.
		 * 
		 *  Initializes to the identity.
		 */
		Quaternion();

		/** \brief Basic constructor initializes 4 quaternion elements.
		 */
		Quaternion(T w_, T x_, T y_, T z_);

		/** \brief Constructs a rotation quaternion from euler angles.
		 */
		Quaternion(T pitch, T yaw, T roll);

		/** \brief Constructs a rotation quaternion from axis-angle rotation.
		 */
		Quaternion(const Vector3<T> &axis, T angle);

		/** \brief Quaternion copy constructor.
		 */
		Quaternion(const Quaternion<T>& quaternion);

		/** \brief Constructs a rotation quaternion from a Matrix3.
		 */
		explicit Quaternion(const Matrix3<T> &m);

		/** \brief Constructs a rotation quaternion from a Matrix4.
		 */
		explicit Quaternion(const Matrix4<T> &m);

		/** \brief Quaternion destructor.
		 */
		~Quaternion();

		/** \brief Returns the conjugate quaternion.
		 *
		 *  Defined as q = [qw, -qv]
		 */
		Quaternion<T> conjugate() const;

		/** \brief Resets this quaternion to the identity.
		 */
		void identity();

		/** \brief Returns true if this quaternion is the identity.
		 */
		bool isIdentity() const;

		/** \brief Returns true if all elements are zero.
		 */
		bool isZero() const;

		/** \brief Returns the inverse quaternion.
		 *
		 *  Defined as q = [qw/|q|, -qv/|q|]
		 */
		Quaternion<T> inverse() const;

		/** \brief Returns the quaternion magnitude.
		 *
		 *  Same as 4-vector magnitude.
		 */
		T magnitude() const;

		/** \brief Normalizes the quaternion to unit length.
		 */
		void normalize();

		/** \brief Sets all quaternion elements.
		 */
		void set(T w_, T x_, T y_, T z_);

		/** \brief Copies quaternion elements.
		 */
		void set(Quaternion<T> q);

		/** \brief Creates a rotation quaternion from axis-angle rotation.
		 */
		void fromAxisAngle(const Vector3<T> &axis, T degrees);

		/** \brief Creates a rotation quaternion from euler angles.
		 */
		void fromPitchYawRoll(T pitch, T yaw, T roll);

		/** \brief Creates a rotation quaternion from a Matrix3.
		 */
		void fromMatrix3(const Matrix3<T> &m);

		/** \brief Creates a rotation quaternion from a Matrix4.
		 */
		void fromMatrix4(const Matrix4<T> &m);

		/** \brief Converts this quaternion to a Matrix3.
		 */
		Matrix3<T> toMatrix3() const;

		/** \brief Converts this quaternion to a Matrix4.
		 */
		Matrix4<T> toMatrix4() const;

		/** \brief Rotates a 3-vector by this quaternion.
		 *
		 *  Defined as v' = qvq*;
		 */
		Vector3<T> rotate(const Vector3<T>& rhs) const;

		/** \brief Rotates a 4-vector by this quaternion.
		 *
		 *  Defined as v' = qvq*;
		 */
		Vector4<T> rotate(const Vector4<T>& rhs) const;

		/** \brief Casts the quaternion to another type.
		 */
		template<typename N>
		Quaternion<N> cast() const
		{
			return Quaternion<N>((N)w, (N)x, (N)y, (N)z);
		};


		bool operator==(const Quaternion<T> &rhs) const;

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

		Quaternion<T> operator/(T scalar) const;
	};

	typedef Quaternion<float> Quaternionf;
	typedef Quaternion<double> Quaterniond;
	typedef Quaternion<int> Quaternioni;

}

#include "Quaternion.inl"

#endif // !QUATERNION_H_

