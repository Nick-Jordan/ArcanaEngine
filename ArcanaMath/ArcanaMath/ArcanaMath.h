#ifndef ARCANA_MATH_H_
#define ARCANA_MATH_H_

#ifdef ARCANA_MATH_EXPORTS  
#define ARCANA_MATH_API __declspec(dllexport)   
#else  
#define ARCANA_MATH_API __declspec(dllimport)   
#endif  

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

		static double clamp(double x, double min, double max);

		/** \brief Transposes number from one range to another.
		 *
		 *  This method calculates the scale factor as the difference in the second range over the difference in this first.
		 *  The scale is then multiplied by the difference between the number and the low end of the first range.
		 *  Finally, the low end of the second range is added to the result.
		 */

		static double range(double x, double lo1, double hi1, double lo2, double hi2);

		/** \brief Performs a linear interpolation.
		 *
		 *  This method interpolates between two numbers.  Given two endpoints, a linear interpolation calculates this point t away from a.
		 *  A linear interpolation creates continuous data from discrete points.
		 */

		static double lerp(const double &a, const double &b, double t);

		/** \brief Performs a bi-linear interpolation.
		 *
		 *  A bi-linear interpolation is an extension of the linear interpolation. It interpolates between four numbers on a rectangular grid, rather than two number in a line.
		 *  The basic idea is to perform a linear interpolation in one direction (on the x-axis), then again in the other direction (on the y-axis).
		 */

		static double bilerp(const double &a, const double &b, const double &c, const double &d, double u, double v);

		/** \brief Gives the sign of a number.
		 *
		 *  This method returns -1 or 1 depending on the sign of the number, returning 0 if x equals 0.0.
		 */

		static int sign(double x);

		/** \brief Squares a number.
		 *
		 *  This method is a simple utility for squaring a number.  It returns x * x.
		 */

		static double square(double x);

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
}

#endif // !ARCANA_MATH_H_
