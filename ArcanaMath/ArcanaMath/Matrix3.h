#ifndef MATRIX3_H_
#define MATRIX3_H_

#include "ArcanaMath.h"
#include "Vector3.h"

#define ARCANA_MATH_EXPORTS

#ifdef ARCANA_MATH_EXPORTS  
#define ARCANA_MATH_API __declspec(dllexport)   
#else  
#define ARCANA_MATH_API __declspec(dllimport)   
#endif  

namespace Arcana
{
	/** \brief A row-major 3x3 matrix class.
	 *  Matrices are concatenated in a left to right order. Multiplies vectors to the left of the matrix.
	 */

	template<typename T>
	class ARCANA_MATH_API Matrix3
	{
		template<typename T>
		friend Vector3<T> operator*(const Matrix3<T> &lhs, const Vector3<T> &rhs);
		template<typename T>
		friend Matrix3<T> operator*(T scalar, const Matrix3<T> &rhs);

	public:

		/** \brief The identity matrix.
		 */
		static const Matrix3<T> IDENTITY;

		/** \brief Creates a scale matrix using vector parameter.
		 *  The vector becomes the scale matrix diagonal.
		 */
		static Matrix3<T> createScale(const Vector3<T>& scale);
		/** \brief Creates a scale matrix using scalar parameters.
		 *  The vector becomes the scale matrix diagonal.
		 */
		static Matrix3<T> createScale(T xScale, T yScale, T zScale);
		/** \brief Creates a rotation matrix using axis/angle paramers.
		 *
		 */
		static Matrix3<T> createRotation(const Vector3<T>& axis, T angle);

		/** \brief Default matrix constructor.
		 *  Initializes the matrix to the identity matrix.
		 */
		Matrix3();

		/** \brief Basic matrix constructor.
		*  Initializes all elements of the matrix.
		*/
		Matrix3(T m00, T m01, T m02,
			T m10, T m11, T m12,
			T m20, T m21, T m22);

		/** \brief Default destructor.
		 */
		~Matrix3();

		/** \brief Calculates the matrix determinant.
		 *  3x3 matrix determinants are calculated with the diagonal method.
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
		Matrix3<T> inverse() const;

		/** \brief Calculates the transpose of the matrix.
		 *  Elements are flipped along the matrix diagonal.  Element i,j becomes element j,i.
		 */
		Matrix3<T> transpose() const;

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
		T* getValuePointer() const;

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

		/** \brief Casts the matrix to another type.
		*/
		template<typename N>
		Matrix3<N> cast()
		{
			return Matrix3<N>((N)_values[0], (N)_values[1], (N)_values[2], 
				(N)_values[3], (N)_values[4], (N)_values[5], 
				(N)_values[6], (N)_values[7], (N)_values[8]);
		};

		bool operator==(const Matrix3<T> &rhs) const;
		bool operator!=(const Matrix3<T> &rhs) const;

		Matrix3<T> &operator+=(const Matrix3<T> &rhs);
		Matrix3<T> &operator-=(const Matrix3<T> &rhs);
		Matrix3<T> &operator*=(const Matrix3<T> &rhs);
		Matrix3<T> &operator*=(T scalar);
		Matrix3<T> &operator/=(T scalar);

		Matrix3<T> operator+(const Matrix3<T> &rhs) const;
		Matrix3<T> operator-(const Matrix3<T> &rhs) const;
		Matrix3<T> operator*(const Matrix3<T> &rhs) const;
		Matrix3<T> operator*(T scalar) const;
		Matrix3<T> operator/(T scalar) const;

	private:

		T _values[9];

	};

	typedef Matrix3<float> Matrix3f;
	typedef Matrix3<double> Matrix3d;
	typedef Matrix3<int> Matrix3i;
}

#include "Matrix3.inl"

#undef ARCANA_MATH_EXPORTS

#endif // !MATRIX3_H_

