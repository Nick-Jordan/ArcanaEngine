#ifndef QUATERNION_H_
#define QUATERNION_H_

#include "Vector3.h"
#include "Matrix4.h"
#include "Matrix3.h"

namespace Arcana
{

	template<typename T>
	class Quaternion
	{
		template<typename T>
		friend Quaternion<T> operator*(const Quaternion<T> &lhs, T rhs);

	public:

		T w, x, y, z;

		static const Quaternion<T> IDENTITY;

		static Quaternion<T> slerp(const Quaternion<T> &a, const Quaternion<T> &b, T t);


		Quaternion();

		Quaternion(T w_, T x_, T y_, T z_);

		Quaternion(T pitch, T yaw, T roll);

		Quaternion(const Vector3<T> &axis, T angle);

		Quaternion(const Quaternion<T>& quaternion);

		explicit Quaternion(const Matrix3<T> &m);

		explicit Quaternion(const Matrix4<T> &m);

		~Quaternion();


		Quaternion<T> conjugate() const;

		void identity();

		bool isIdentity() const;

		bool isZero() const;

		Quaternion<T> inverse() const;

		T magnitude() const;

		void normalize();

		void set(T w_, T x_, T y_, T z_);

		void set(Quaternion<T> q);


		void fromAxisAngle(const Vector3<T> &axis, T degrees);

		void fromPitchYawRoll(T pitch, T yaw, T roll);

		void fromMatrix3(const Matrix3<T> &m);

		void fromMatrix4(const Matrix4<T> &m);

		Matrix3<T> toMatrix3();

		Matrix4<T> toMatrix4();

		template<typename N>
		Quaternion<N> cast()
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

