#ifndef MATRIX4_H_
#define MATRIX4_H_

#include "ArcanaMath.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix3.h"

namespace Arcana
{
	/** \brief A row-major 4x4 matrix class.
	*  Matrices are concatenated in a left to right order. Multiplies vectors to the left of the matrix.
	*/

	template<typename T>
	class Matrix4
	{
		template<typename T>
		friend Vector3<T> operator*(const Matrix4<T> &lhs, const Vector3<T> &rhs);
		template<typename T>
		friend Matrix4<T> operator*(T scalar, const Matrix4<T> &rhs);

	public:

		/** \brief The identity matrix.
		 */
		static const Matrix4<T> IDENTITY;

		/** \brief Creates a scale matrix using vector parameter.
		 *  The vector becomes the scale matrix diagonal.
		 */

		static Matrix4<T> createScale(const Vector3<T>& scale);
		/** \brief Creates a scale matrix using scalar parameters.
		 *  The vector becomes the scale matrix diagonal.
		 */

		static Matrix4<T> createScale(T xScale, T yScale, T zScale);
		/** \brief Creates a rotation matrix using axis/angle parameters.
		 *
		 */

		static Matrix4<T> createRotation(const Vector3<T>& axis, T angle);
		/** \brief Creates a rotation matrix using pitch/yaw/roll parameters.
		 *
		 */
		static Matrix4<T> createRotation(T pitch, T yaw, T roll);
		/** \brief Creates a translation matrix using vector parameter.
		 *
		 */
		static Matrix4<T> createTranslation(const Vector3<T>& translation);
		/** \brief Creates a translation matrix using scalar parameters.
		 *
		 */
		static Matrix4<T> createTranslation(T xTranslation, T yTranslation, T zTranslation);

		/** \brief Creates a translation matrix using scalar parameters.
		*
		*/
		static Matrix4<T> createPerspective(T fieldOfView, T aspectRatio, T zNearPlane, T zFarPlane);

		/** \brief Creates a translation matrix using scalar parameters.
		*
		*/
		static Matrix4<T> createOrthographic(T width, T height, T zNearPlane, T zFarPlane);

		/** \brief Creates a translation matrix using scalar parameters.
		*
		*/
		static Matrix4<T> createOrthographicOffCenter(T left, T right, T bottom, T top, T zNearPlane, T zFarPlane);

		static Matrix4<T> createLookAt(Vector3<T> eye, Vector3<T> center, Vector3<T> up);

		/** \brief Default matrix constructor.
		 *  Initializes the matrix to the identity matrix.
		 */
		Matrix4();

		/** \brief Basic matrix constructor.
		*  Initializes all elements of the matrix.
		*/
		Matrix4(T m00, T m01, T m02, T m03,
			T m10, T m11, T m12, T m13,
			T m20, T m21, T m22, T m23,
			T m30, T m31, T m32, T m33);

		/** \brief Constructor that takes a 3x3 row-major matrix as an argument.
		 *  Initializes inner 3x3 matrix to the elements of the argument.
		 */
		Matrix4(const Matrix3<T>& matrix);

		/** \brief Default destructor.
		 */
		~Matrix4();

		/** \brief Calculates the matrix determinant.
		 *  Matrix determinant calculated by expansion by minors.
		 */
		T determinant() const;

		/** \brief fromAxes
		 *
		 */
		void fromAxes(const Vector3<T> &x, const Vector3<T> &y, const Vector3<T> &z);

		/** \brief fromAxesTransposed
		 *
		 */
		void fromAxesTransposed(const Vector3<T> &x, const Vector3<T> &y, const Vector3<T> &z);

		/** \brief Sets the matrix to the identity matrix.
		 *
		 */
		void identity();

		/** \brief toAxesTransposed
		 * 
		 */
		void toAxes(Vector3<T> &x, Vector3<T> &y, Vector3<T> &z) const;

		/** \brief toAxesTransposed
		 *
		 */
		void toAxesTransposed(Vector3<T> &x, Vector3<T> &y, Vector3<T> &z) const;

		/** \brief Calculates the inverse of the matrix.
		 *
		 */
		Matrix4<T> inverse() const;

		/** \brief Calculates the transpose of the matrix.
		 *  Elements are flipped along the matrix diagonal.  Element i,j becomes element j,i.
		 */
		Matrix4<T> transpose() const;

		/** \brief Returns the element at position (i, j).
		 *
		 */

		T at(unsigned int i, unsigned int j) const;

		/** \brief Returns the element at position (i / 3, i % 3).
		 *
		 */
		T at(unsigned int i) const;

		/** \brief Sets the element at position (i, j).
		 *
		 */
		void set(unsigned int i, unsigned int j, T value);

		/** \brief Sets the element at position (i / 3, i % 3).
		  *
		*/
		void set(unsigned int i, T value);

		/** \brief Returns a pointer to the value array.
		 *
		 */
		const T* getValuePointer() const;

		/** \brief Returns the matrix up vector.
		 *
		 */
		Vector3<T> getUpVector() const;

		/** \brief Returns the matrix down vector.
		 *
		 */
		Vector3<T> getDownVector() const;

		/** \brief Returns the matrix left vector.
		 *
		 */
		Vector3<T> getLeftVector() const;

		/** \brief Returns the matrix right vector.
		 *
		 */
		Vector3<T> getRightVector() const;

		/** \brief Returns the matrix forward vector.
		 *
		 */
		Vector3<T> getForwardVector() const;

		/** \brief Returns the matrix back vector.
		 *
		 */
		Vector3<T> getBackVector() const;

		Matrix3<T> toMatrix3() const;

		/** \brief Casts the matrix to another type.
		 */
		template<typename N>
		Matrix4<N> cast() const
		{
			return Matrix4<N>((N)_values[0], (N)_values[1], (N)_values[2], (N)_values[3],
				(N)_values[4], (N)_values[5], (N)_values[6], (N)_values[7],
				(N)_values[8], (N)_values[9], (N)_values[10], (N)_values[11],
				(N)_values[12], (N)_values[13], (N)_values[14], (N)_values[15]);
		};


		bool operator==(const Matrix4<T> &rhs) const;
		bool operator!=(const Matrix4<T> &rhs) const;

		Matrix4<T>& operator+=(const Matrix4<T> &rhs);
		Matrix4<T>& operator-=(const Matrix4<T> &rhs);
		Matrix4<T>& operator*=(const Matrix4<T> &rhs);
		Matrix4<T>& operator*=(T scalar);
		Matrix4<T>& operator/=(T scalar);

		Matrix4<T> operator+(const Matrix4<T> &rhs) const;
		Matrix4<T> operator-(const Matrix4<T> &rhs) const;
		Matrix4<T> operator*(const Matrix4<T> &rhs) const;
		Matrix4<T> operator*(T scalar) const;
		Matrix4<T> operator/(T scalar) const;

		Vector3<T>& operator*(Vector3<T> &rhs);
		Vector4<T>& operator*(Vector4<T> &rhs);

		Vector3<T>& operator*(const Vector3<T> &rhs) const;
		Vector4<T>& operator*(const Vector4<T> &rhs) const;

	private:

		T _values[16];
	};

	typedef Matrix4<float> Matrix4f;
	typedef Matrix4<double> Matrix4d;
	typedef Matrix4<int> Matrix4i;
}

#include "Matrix4.inl"

#endif // !MATRIX4_H_
