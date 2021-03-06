#ifndef ARCANA_MATH_H_
#define ARCANA_MATH_H_

#ifdef ARCANA_MATH_EXPORTS  
#define ARCANA_MATH_API __declspec(dllexport)   
#else  
#define ARCANA_MATH_API __declspec(dllimport)   
#endif  

//vld
#include <vld.h>

#include <cmath>

namespace Arcana
{
	/** \brief Arcana math utilities.
	 *
	 *   
	 */

	class ARCANA_MATH_API Math
	{
	public:

		/** \brief Mathematical constant pi.
		 */
		static const double PI;
		/** \brief Mathematical constant pi / 2.0.
		 */
		static const double HALF_PI;
		/** \brief Mathematical constant pi / 4.0.
		 */
		static const double QUARTER_PI;
		/** \brief Mathematical constant pi * 2.0.
		 */
		static const double TWO_PI;
		/** \brief Constant representing small change.
		 */
		static const double EPSILON;
		//static const double TOLERANCE;

		/** \brief Constant for transposing from degrees to radians.
		 */
		static const double DEG_TO_RAD;
		/** \brief Constant for transposing from radians to degrees.
		 */
		static const double RAD_TO_DEG;


		/** \brief Clamps number between min and max.
		 *
		 *  This method returns min if the number is less than min, max if the number is greater than max, or x if the number is in the range.
		 */

		template<typename T>
		static T clamp(T x, T min, T max);

		/** \brief Transposes number from one range to another.
		 *
		 *  This method calculates the scale factor as the difference in the second range over the difference in this first.
		 *  The scale is then multiplied by the difference between the number and the low end of the first range.
		 *  Finally, the low end of the second range is added to the result.
		 */

		template<typename T>
		static T range(T x, T lo1, T hi1, T lo2, T hi2);

		/** \brief Performs a linear interpolation.
		 *
		 *  This method interpolates between two numbers.  Given two endpoints, a linear interpolation calculates this point t away from a.
		 *  A linear interpolation creates continuous data from discrete points.
		 */

		template<typename T>
		static T lerp(const T &a, const T &b, double t);
		

		/** \brief Performs a bi-linear interpolation.
		 *
		 *  A bi-linear interpolation is an extension of the linear interpolation. It interpolates between four numbers on a rectangular grid, rather than two number in a line.
		 *  The basic idea is to perform a linear interpolation in one direction (on the x-axis), then again in the other direction (on the y-axis).
		 */

		template<typename T>
		static T bilerp(const T &a, const T &b, const T &c, const T &d, double u, double v);

		/** \brief Gives the sign of a number.
		 *
		 *  This method returns -1 or 1 depending on the sign of the number, returning 0 if x equals 0.0.
		 */

		template<typename T>
		static int sign(T x);

		/** \brief Squares a number.
		 *
		 *  This method is a simple utility for squaring a number.  It returns x * x.
		 */

		template<typename T>
		static T square(T x);

		template<typename T>
		static T min(T a, T b);

		template<typename T>
		static T max(T a, T b);

		template<typename T>
		static T abs(T a);

		/** \brief Gives the fractional part of a decimal number.
		 *
		 *  This method uses modf to return the fractional part of the number.  It also sets the intpart pointer to the integer part of the number.
		 */

		static double fract(double num, double* intpart);

		/** \brief Transposes a 3D point from the Cartesian plane to spherical coordinates.
		 *
		 *  rho = |<x, y, z|
		 *  phi = acos(z / rho)
		 *  theta = atan(y / x)
		 */

		static void cartesianToSpherical(double x, double y, double z,
			double &rho, double &phi, double &theta);

		/** \brief Transposes a 3D point from spherical coordinates plane to the Cartesian plane.
		 *
		 *  x = rho * cos(phi) * cos(theta)
		 *  y = rho * sin(phi)
		 *  z = rho * cos(phi) * sin(theta)
		 */

		static void sphericalToCartesian(double rho, double phi, double theta,
			double &x, double &y, double &z);

		/** \brief Determines if the difference between two numbers is less than EPSILON.
		 *
		 *  Calculates difference between f1 and f2.  Returns true if the absolute value of the difference is less than EPSILON.
		 */

		static bool closeEnough(double f1, double f2);

		/** \brief Converts a number in degrees to radians.
		 *
		 *  radians = degrees * PI / 180.0
		 */

		static double degreesToRadians(double degrees);

		/** \brief Converts a number in radians to degrees.
		 *
		 *  degrees = radians * 180.0 / PI
		 */

		static double radiansToDegrees(double radians);

		static double smoothstep(double a, double b, double x);

		static void smooth(double* x, double target, double elapsedTime, double responseTime);

		static void smooth(double* x, double target, double elapsedTime, double riseTime, double fallTime);
	};

	template<typename T>
	inline T Math::clamp(T x, T min, T max)
	{
		return Math::min(Math::max(x, min), max);
	}

	template<typename T>
	inline T Math::range(T x, T lo1, T hi1, T lo2, T hi2)
	{
		if (lo1 == hi1)
			return clamp(x, lo2, hi2); //deal with this case

		T scale = (hi2 - lo2) / (hi1 - lo1);
		return (lo2 + ((x - lo1) * scale));
	}

	template<typename T>
	inline T Math::lerp(const T &a, const T &b, double t)
	{
		return a + (b - a) * t;
	}

	template<typename T>
	inline T Math::bilerp(const T &a, const T &b, const T &c, const T &d, double u, double v)
	{
		return a * ((1.0 - u) * (1.0 - v))
			+ b * (u * (1.0 - v))
			+ c * (v * (1.0 - u))
			+ d * (u * v);
	}

	template<typename T>
	inline int Math::sign(T x)
	{
		return (T(0) < x) - (x < T(0));
	}

	template<typename T>
	inline T Math::square(T x)
	{
		return x * x;
	}

	template<typename T>
	inline T Math::min(T a, T b)
	{
		return a < b ? a : b;
	}

	template<typename T>
	inline T Math::max(T a, T b)
	{
		return a > b ? a : b;
	}

	template<typename T>
	inline T Math::abs(T a)
	{
		return a >= (T)0 ? a : -a;
	}
}

#endif // !ARCANA_MATH_H_
