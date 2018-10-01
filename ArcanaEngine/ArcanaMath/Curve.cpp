#include "Curve.h"

#include <memory>
#include <iostream>

namespace Arcana
{
	Curve::Curve() : _pointSize(0)
	{

	}

	Curve::Curve(InterpolationMethod method, unsigned int pointComponents) : _method(method), _pointSize(pointComponents)
	{

	}

	Curve::Curve(const Curve& other) : _points(other._points), _pointSize(other._pointSize), _method(other._method)
	{

	}
	Curve::~Curve()
	{

	}

	unsigned int Curve::getPointComponents() const
	{
		return _pointSize;
	}

	unsigned int Curve::getNumPoints() const
	{
		return _points.size();
	}

	Curve::InterpolationMethod Curve::getInterpolationMethod() const
	{
		return _method;
	}

	void Curve::setInterpolationMethod(InterpolationMethod method)
	{
		_method = method;
	}

	double Curve::getTime(unsigned int i) const
	{
		if (i < getNumPoints())
		{
			return _points[i].getTime();
		}

		return -1.0;
	}

	void Curve::setPoint(unsigned int i, double time, double* value)
	{
		if (i < getNumPoints())
		{
			//potentially log something
			return;
		}

		_points[i]._time = time;

		size_t size = sizeof(double) * _points[i].getPointSize();

		if (value)
		{
			memcpy(_points[i]._value, value, size);
		}
	}

	void Curve::addPoint(double time, double* value)
	{
		if (_points.empty())
		{
			_points.push_back(Point(_pointSize, time, value));
			return;
		}

		for (std::vector<Point>::iterator iter = _points.begin(); iter != _points.end(); iter++)
		{
			if (time <= (*iter).getTime())
			{
				_points.insert(iter, Point(_pointSize, time, value));
				return;
			}
		}

		_points.push_back(Point(_pointSize, time, value));
	}

	const Curve::Point& Curve::getPoint(unsigned int i) const
	{
		if (i < getNumPoints())
		{
			return Point();
		}

		return _points[i];
	}

	void Curve::getValue(double time, double loopTime, double* value)
	{
		if (getNumPoints() == 1)
		{
			copyValue(0, value);
			return;
		}

		double startTime = _points.front().getTime();
		double endTime = _points.back().getTime();

		unsigned int min = 0;
		unsigned int max = getNumPoints() - 1;
		double localTime = time;
		if (startTime > 0.0 || endTime < 1.0)
		{
			min = findIndex(startTime, 0, max);
			max = findIndex(endTime, min, max);

			localTime = _points[min].getTime() + (_points[max].getTime() - _points[min].getTime()) * time;
		}

		if (loopTime == 0.0)
		{
			if (localTime < _points[min].getTime())
			{
				localTime = _points[min].getTime();
			}
			else if (localTime > _points[max].getTime())
			{
				localTime = _points[max].getTime();
			}
		}

		if (localTime == _points[min].getTime())
		{
			copyValue(min, value);
			return;
		}
		if (localTime == _points[max].getTime())
		{
			copyValue(max, value);
			return;
		}

		double t;

		Point* start;
		Point* end;
		Point* in = nullptr;
		Point* out = nullptr;
		unsigned int index;

		if (localTime > _points[max].getTime())
		{
			index = max;
			start = &_points[max];
			end = &_points[min];

			t = (localTime - start->getTime()) / loopTime;
		}
		else if (localTime < _points[min].getTime())
		{
			index = min;
			start = &_points[min];
			end = &_points[max];

			t = (start->getTime() - localTime) / loopTime;
		}
		else
		{
			index = findIndex(localTime, min, max);

			start = &_points[index];
			end = &_points[index == max ? index : index + 1];

			t = localTime;
		}

		if (index > 0)
		{
			in = &_points[index - 1];
		}
		if (index < getNumPoints() - 2)
		{
			out = &_points[index + 2];
		}

		switch (_method)
		{
		case Linear:
			break;
		case Spline:
			return;
		case Bezier:
			bezierInterpolation(t, *start, *end, in, out, value);
			return;
		case Hermite:
			hermiteInterpolation(t, *start, *end, in, out, value);
			return;
		case Flat:
			flatHermiteInterpolation(t, *start, *end, in, out, value);
			return;
		case Step:
			memcpy(value, start->getValue(), _pointSize * sizeof(double));
			return;
		};

		linearInterpolation(t, *start, *end, value);
	}

	void Curve::copyValue(unsigned int i, double* value)
	{
		if (i < getNumPoints())
		{
			memcpy(value, _points[i].getValue(), sizeof(double) * _pointSize);
		}
	}

	int Curve::findIndex(double time, unsigned int min, unsigned int max) const
	{
		unsigned int mid;

		do
		{
			mid = (min + max) / 2;

			bool b = true;
			if (mid < getNumPoints() - 1)
				b = time < _points[mid + 1].getTime();

			if (time >= _points[mid].getTime() && b)
				return mid;
			else if (time < _points[mid].getTime())
				max = mid - 1;
			else
				min = mid + 1;
		} while (min <= max);

		return max;
	}


	void Curve::linearInterpolation(double time, Point& start, Point& end, double* value)
	{
		if (!value) return;

		double* startValue = start.getValue();
		double* endValue = end.getValue();

		for (unsigned int i = 0; i < _pointSize; i++)
		{
			value[i] = Math::lerp(startValue[i], endValue[i], time);
		}
	}

	void Curve::splineInterpolation(double time, Point& p0, Point& p1, Point& p2, Point& p3, double* value)
	{

	}

	void Curve::bezierInterpolation(double time, Point& start, Point& end, Point* in, Point* out, double* value)
	{
		if (!value) return;

		double ti = 1 - time;

		double* startValue = start.getValue();
		double* endValue = end.getValue();
		double* outValue = out ? out->getValue() : end.getValue();
		double* inValue = in ? in->getValue() : start.getValue();

		for (unsigned int i = 0; i < _pointSize; i++)
		{
			if (startValue[i] == endValue[i])
			{
				value[i] = startValue[i];
			}
			else
			{
				value[i] = ti * (ti * (ti * inValue[i] + time * startValue[i]) + time * (ti * startValue[i] + time * endValue[i]))
					+ time * (ti * (ti * startValue[i] + time * endValue[i]) + time * (ti * endValue[i] + time * outValue[i]));
			}
		}
	}

	void Curve::hermiteInterpolation(double time, Point& start, Point& end, Point* in, Point* out, double* value) const
	{
		if (!value) return;

		double t2 = time * time;
		double t3 = time * time * time;

		double* startValue = start.getValue();
		double* endValue = end.getValue();
		double* outValue = out ? out->getValue() : end.getValue();
		double* inValue = in ? in->getValue() : start.getValue();

		//f(p_0, v_0, v_1, p_1, t) = p_0(2 t ^ 3 - 3 t ^ 2 + 1) + v_1(t ^ 3 - t ^ 2) + v_0(t ^ 3 - 2 t ^ 2 + t) + p_1(3 t ^ 2 - 2 t ^ 3)

		for (unsigned int i = 0; i < _pointSize; i++)
		{
			if (startValue[i] == endValue[i])
			{
				value[i] = startValue[i];
			}
			else
			{
				value[i] = inValue[i] * (2 * t3 - 3 * t2 + 1) + endValue[i] * (t3 - t2) + startValue[i] * (t3 - 2 * t2 + time) + outValue[i] * (3 * t2 - 2 * t3);
			}
		}
	}

	void Curve::flatHermiteInterpolation(double time, Point& start, Point& end, Point* in, Point* out, double* value) const
	{
		if (!value) return;

		double t2 = time * time;
		double t3 = time * time * time;

		double* startValue = start.getValue();
		double* endValue = end.getValue();

		//f(p_0, v_0, v_1, p_1, t) = p_0(2 t ^ 3 - 3 t ^ 2 + 1) + v_1(t ^ 3 - t ^ 2) + v_0(t ^ 3 - 2 t ^ 2 + t) + p_1(3 t ^ 2 - 2 t ^ 3)

		for (unsigned int i = 0; i < _pointSize; i++)
		{
			if (startValue[i] == endValue[i])
			{
				value[i] = startValue[i];
			}
			else
			{
				value[i] = startValue[i] * (2 * t3 - 3 * t2 + 1) + endValue[i] * (3 * t2 - 2 * t3);
			}
		}
	}


	Curve& Curve::operator=(const Curve& other)
	{
		_points = other._points;
		_pointSize = other._pointSize;
		_method = other._method;

		return *this;
	}



	//Point

	Curve::Point::Point() : _size(0), _time(0.0), _value(nullptr)
	{

	}
	Curve::Point::Point(unsigned int size, double time, double* value)
		: _size(size), _time(time), _value(nullptr)
	{
		setValue(value);

		if (value)
		{
			delete[] value;
			value = nullptr;
		}
	}
	Curve::Point::Point(const Point& other) : _size(other._size), _time(other._time), _value(nullptr)
	{
		setValue(other._value);
	}
	Curve::Point::~Point()
	{
		if (_value)
		{
			delete[] _value;
			_value = nullptr;
		}
	}

	double Curve::Point::getTime() const
	{
		return _time;
	}

	double* Curve::Point::getValue() const
	{
		return _value;
	}

	double Curve::Point::getValue(unsigned int component) const
	{
		if (_value && component < _size)
		{
			return _value[component];
		}
		return 0.0;
	}

	void Curve::Point::setValue(double* value)
	{
		if (value)
		{
			if (!_value)
			{
				_value = new double[_size];
			}
			memcpy(_value, value, _size * sizeof(double));
		}
	}

	unsigned int Curve::Point::getPointSize() const
	{
		return _size;
	}

	Curve::Point& Curve::Point::operator=(const Point& other)
	{
		_size = other._size;
		_time = other._time;

		if (other._value)
		{
			memcpy(_value, other._value, _size * sizeof(double));
		}

		return *this;
	}
}
