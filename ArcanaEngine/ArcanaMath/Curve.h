#ifndef CURVE_H_
#define CURVE_H_

#include "ArcanaMath.h"
#include "Vector4.h"

#include <vector>

//vld
#include <vld.h>

namespace Arcana
{
	/** \brief A class used for interpolating points along a curve
	 *
	 *  The user can specify the interpolation method, control points, and the number of point components.
	 */
	class ARCANA_MATH_API Curve
	{
	public:
		
		/// Interpolation method enum
		enum InterpolationMethod
		{
			Linear,	  ///< Linear interpolation.  WORKS
			BSpline,   ///< B-Spline curve.      WORKS
			Bezier,	  ///< Bezier curve.  DOESN'T WORK
			Hermite,  ///< Hermite interpolation. DOESN'T WORK
			Flat,	  ///< Flat hermite interpolation. DOESN'T WORK
			Step	  ///< Step interpolation. WORKS
		};

		/** \brief A control point on the curve.
		 *
		 *  Has a user defined size and parametric position on the curve.
		 */
		class ARCANA_MATH_API Point
		{
			friend class Curve;

		public:

			/** \brief Point default constructor.
			 */
			Point();

			/** \brief Sets the point size, value, and time.
			 */
			Point(double time, const Vector4d& value);

			/** \brief Point copy constructor.
			 */
			Point(const Point& other);

			/** \brief Point destructor.
			 */
			~Point();

			/** \brief Accessor for the point time on the curve.
			 */
			double getTime() const;

			/** \brief Accessor for the value pointer.
			 */
			const Vector4d& getValue() const;

			/** \brief Mutator for the value pointer.
			 */
			void setValue(const Vector4d& value);


			/** \brief Point assignment operator.
			 */
			Point& operator=(const Point& other);

		private:

			double _time;  ///< Parametric time variable.
			Vector4d _value; ///< Point value
		};

		struct Value
		{
			Vector4d Position;
			Vector4d Tangent;
		};

		/** \brief Curve default constructor
		 */
		Curve();

		/** \brief Constructs a curve with a specified interpolation method.
		 */
		Curve(InterpolationMethod method);

		/** \brief Curve copy constructor.
		 */
		Curve(const Curve& other);

		/** \brief Curve destructor.
		 */
		~Curve();

		/** \brief Returns the number of control points.
		 */
		unsigned int getNumPoints() const;

		/** \brief Accessor for the interpolation method.
		 */
		InterpolationMethod getInterpolationMethod() const;

		/** \brief Mutator for the interpolation method.
		 */
		void setInterpolationMethod(InterpolationMethod method);

		/** \brief Returns the time for the control point at the specified index. 
		 */
		double getTime(unsigned int i) const;

		/** \brief Sets the value and time for a control point at the specified index.
		 */
		void setPoint(unsigned int i, double time, const Vector4d& value);

		/** \brief Adds a control point.
		 */
		void addPoint(double time, const Vector4d& value);

		void addPoint(const Vector4d& value);

		/** \brief Returns a const reference to the control point at the specified index.
		 */
		const Point& getPoint(unsigned int i) const;

		/** \brief Returns the interpolated value at the specified time.
		 */
		Value getValue(double time);

		Vector4d getValuePosition(double time);

		Vector4d getValueTangent(double time);

		/** \brief Curve assignment operator.
		 */
		Curve& operator=(const Curve& other);

		/** \brief Performs a linear interpolation.
		 *
		 *  Defined as v[i] = start[i] + (end[i] - start[i]) * time;
		 */
		void linearInterpolation(double time, Value& value);

		/** \brief Performs a bspline interpolation.
		 */
		void bsplineInterpolation(double time, Value& value);

		/** \brief Performs a bezier interpolation.
		 */
		void bezierInterpolation(double time, Value& value);

		/** \brief Performs a hermite interpolation.
		 */
		void hermiteInterpolation(double time, Value& value);

		/** \brief Performs a flat hermite interpolation.
		 */
		void flatHermiteInterpolation(double time, Value& value);

		/** \brief Performs a step interpolation.
		 */
		void stepInterpolation(double time, Value& value);

	private:

		std::vector<Point> _points;	  ///< The vector of control points.
		InterpolationMethod _method;  ///< The interpolation method
	};

}

#endif // !CURVE_H_

