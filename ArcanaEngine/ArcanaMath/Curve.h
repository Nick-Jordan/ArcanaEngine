#ifndef CURVE_H_
#define CURVE_H_

#include "ArcanaMath.h"

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
			Linear,	  ///< Linear interpolation.
			Spline,   ///< Spline curve.
			Bezier,	  ///< Bezier curve.
			Hermite,  ///< Hermite interpolation.
			Flat,	  ///< Flat hermite interpolation.
			Step	  ///< Step interpolation.
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
			Point(unsigned int size, double time, double* value);

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
			double* getValue() const;

			/** \brief Accessor for the value by component.
			 */
			double getValue(unsigned int component) const;

			/** \brief Mutator for the value pointer.
			 */
			void setValue(double* value);

			/** \brief Accessor for the point size in components.
			 */
			unsigned int getPointSize() const;

			/** \brief Point assignment operator.
			 */
			Point& operator=(const Point& other);

		private:

			double _time;  ///< Parametric time variable.
			double* _value; ///< Point value

			unsigned int _size; ///< The number of point components.
		};

		/** \brief Curve default constructor
		 */
		Curve();

		/** \brief Constructs a curve with a specified interpolation method.
		 */
		Curve(InterpolationMethod method, unsigned int pointComponents = 3);

		/** \brief Curve copy constructor.
		 */
		Curve(const Curve& other);

		/** \brief Curve destructor.
		 */
		~Curve();

		/** \brief Returns the size of each point.
		 */
		unsigned int getPointComponents() const;

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
		void setPoint(unsigned int i, double time, double* value);

		/** \brief Adds a control point.
		 */
		void addPoint(double time, double* value);

		/** \brief Returns a const reference to the control point at the specified index.
		 */
		const Point& getPoint(unsigned int i) const;

		/** \brief Returns the interpolated value at the specified time.
		 */
		void getValue(double time, double loopTime, double* value);

		/** \brief Copies the value of the control point at the specified index into 'value'
		 */
		void copyValue(unsigned int i, double* value);

		/** \brief Finds the index nearest the specified time and within the range [min, max]
		 */
		int findIndex(double time, unsigned int min, unsigned int max) const;

		/** \brief Curve assignment operator.
		 */
		Curve& operator=(const Curve& other);

		/** \brief Performs a linear interpolation.
		 *
		 *  Defined as v[i] = start[i] + (end[i] - start[i]) * time;
		 */
		void linearInterpolation(double time, Point& start, Point& end, double* value);

		/** \brief Performs a spline interpolation.
		 */
		void splineInterpolation(double time, Point& p0, Point& p1, Point& p2, Point& p3, double* value);

		/** \brief Performs a bezier interpolation.
		 */
		void bezierInterpolation(double time, Point& start, Point& end, Point* in, Point* out, double* value);

		/** \brief Performs a hermite interpolation.
		 */
		void hermiteInterpolation(double time, Point& start, Point& end, Point* in, Point* out, double* value) const;

		/** \brief Performs a flat hermite interpolation.
		 */
		void flatHermiteInterpolation(double time, Point& start, Point& end, Point* in, Point* out, double* value) const;

	private:

		std::vector<Point> _points;	  ///< The vector of control points.
		unsigned int _pointSize;	  ///< The number of components per point.	
		InterpolationMethod _method;  ///< The interpolation method
	};

}

#endif // !CURVE_H_

