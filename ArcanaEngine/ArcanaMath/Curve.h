#ifndef CURVE_H_
#define CURVE_H_

#include "ArcanaMath.h"

#include <vector>

namespace Arcana
{

	class ARCANA_MATH_API Curve
	{
	public:
		
		enum InterpolationMethod
		{
			Linear,
			Spline,
			Bezier,
			Hermite,
			Flat,
			Step
		};

		class ARCANA_MATH_API Point
		{
			friend class Curve;

		public:

			Point();
			Point(unsigned int size, double time, double* value);
			Point(const Point& other);
			~Point();

			double getTime() const;

			double* getValue() const;

			double getValue(unsigned int component) const;

			void setValue(double* value);

			unsigned int getPointSize() const;

			Point& operator=(const Point& other);

		private:

			double _time;
			double* _value;

			unsigned int _size;
		};

		Curve();
		Curve(InterpolationMethod method, unsigned int pointComponents = 3);
		Curve(const Curve& other);
		~Curve();

		unsigned int getPointComponents() const;

		unsigned int getNumPoints() const;

		InterpolationMethod getInterpolationMethod() const;

		void setInterpolationMethod(InterpolationMethod method);

		double getTime(unsigned int i) const;

		void setPoint(unsigned int i, double time, double* value);

		void addPoint(double time, double* value);

		const Point& getPoint(unsigned int i) const;

		void getValue(double time, double loopTime, double* value);

		void copyValue(unsigned int i, double* value);

		int findIndex(double time, unsigned int min, unsigned int max) const;

		Curve& operator=(const Curve& other);


		void linearInterpolation(double time, Point& start, Point& end, double* value);

		void splineInterpolation(double time, Point& p0, Point& p1, Point& p2, Point& p3, double* value);

		void bezierInterpolation(double time, Point& start, Point& end, Point* in, Point* out, double* value);

		void hermiteInterpolation(double time, Point& start, Point& end, Point* in, Point* out, double* value) const;

		void flatHermiteInterpolation(double time, Point& start, Point& end, Point* in, Point* out, double* value) const;

	private:

		std::vector<Point> _points;
		unsigned int _pointSize;
		InterpolationMethod _method;
	};

}

#endif // !CURVE_H_

