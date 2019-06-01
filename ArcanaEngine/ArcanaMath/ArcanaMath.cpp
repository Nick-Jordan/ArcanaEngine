// ArcanaMath.cpp : Defines the exported functions for the DLL application.
//

#include "ArcanaMath.h"

#include <cmath>

namespace Arcana
{
	
	const double Math::PI = 3.14159265358979323846;
	const double Math::HALF_PI = PI / 2.0;
	const double Math::QUARTER_PI = PI / 4.0;
	const double Math::TWO_PI = 2.0 * PI;
	const double Math::EPSILON = 1e-6;
	//const double Math::TOLERANCE = 2e-37;

	const double Math::DEG_TO_RAD = 0.017453292519943295769236907684886;
	const double Math::RAD_TO_DEG = 57.295779513082320876798154814105;

	double Math::fract(double num, double* intpart)
	{
		double n = modf(num, intpart);
		return n;
	}
	
	void Math::cartesianToSpherical(double x, double y, double z,
		double &rho, double &phi, double &theta)
	{
		// All angles are in radians.
		// rho = distance from origin O to point P (i.e., length of OP)
		// phi = angle between OP and the XZ plane
		// theta = angle between X-axis and OP projected onto XZ plane

		rho = sqrt(square(x) + square(y) + square(z));
		phi = acos(z / rho);
		theta = atan(y / x); 
	}

	void Math::sphericalToCartesian(double rho, double phi, double theta,
		double &x, double &y, double &z)
	{
		x = rho * cos(phi) * cos(theta);
		y = rho * sin(phi);
		z = rho * cos(phi) * sin(theta);
	}

	bool Math::closeEnough(double f1, double f2)
	{
		return fabs((f1 - f2) / ((f2 == 0.0f) ? 1.0f : f2)) < EPSILON;
	}

	double Math::degreesToRadians(double degrees)
	{
		return (degrees * PI) / 180.0;
	}

	double Math::radiansToDegrees(double radians)
	{
		return (radians * 180.0f) / PI;
	}

	double Math::smoothstep(double a, double b, double x)
	{
		// Returns a gradual transition of 'x' from 0 to 1 beginning at
		// threshold 'a' and ending at threshold 'b'.
		//
		// References:
		//  [1] http://www.rendermanacademy.com/docs/smoothstep.htm
		//  [2] http://www.brlcad.org/doxygen/d8/d33/noise_8c-source.html
		//  [3] Natalya Tatarchuk, "Efficient High-Level Shader Development",
		//      Game Developers Conference Europe, August 2003.

		if (x < a)
		{
			return 0.0;
		}
		else if (x >= b)
		{
			return 1.0;
		}
		else
		{
			x = (x - a) / (b - a);
			return x * x * (3.0 - 2.0 * x);
		}
	}

	void Math::smooth(double* x, double target, double elapsedTime, double responseTime)
	{

		if (elapsedTime > 0)
		{
			*x += (target - *x) * elapsedTime / (elapsedTime + responseTime);
		}
	}

	void Math::smooth(double* x, double target, double elapsedTime, double riseTime, double fallTime)
	{

		if (elapsedTime > 0)
		{
			float delta = target - *x;
			*x += delta * elapsedTime / (elapsedTime + (delta > 0 ? riseTime : fallTime));
		}
	}
}