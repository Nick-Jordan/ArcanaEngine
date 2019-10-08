#include "Curve.h"

#include <memory>
#include <iostream>

namespace Arcana
{
	Curve::Curve() : _method(InterpolationMethod::Linear)
	{

	}

	Curve::Curve(InterpolationMethod method) : _method(method)
	{

	}

	Curve::Curve(const Curve& other) : _points(other._points), _method(other._method)
	{

	}
	Curve::~Curve()
	{

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

	void Curve::setPoint(unsigned int i, double time, const Vector4d& value)
	{
		if (i >= getNumPoints())
		{
			//potentially log something
			return;
		}

		_points[i]._time = time;
		_points[i]._value = value;
	}

	void Curve::addPoint(double time, const Vector4d& value)
	{
		if (_points.empty())
		{
			_points.push_back(Point(time, value));
			return;
		}

		for (std::vector<Point>::iterator iter = _points.begin(); iter != _points.end(); iter++)
		{
			if (time <= (*iter).getTime())
			{
				_points.insert(iter, Point(time, value));
				return;
			}
		}

		_points.push_back(Point(time, value));
	}

	void Curve::addPoint(const Vector4d& value)
	{
		if (_points.size() == 0)
		{
			_points.push_back(Point(0.0, value));
			return;
		}

		_points.push_back(Point(1.0, value));

		if (_points.size() > 2)
		{
			for (int i = 1; i < _points.size() - 1; i++)
			{
				_points[i]._time = (double)i / (double) (_points.size() - 1);
			}
		}
	}

	const Curve::Point& Curve::getPoint(unsigned int i) const
	{
		if (i >= getNumPoints())
		{
			return Point();
		}

		return _points[i];
	}

	Curve::Value Curve::getValue(double time)
	{
		Value value;

		switch (_method)
		{
		case Linear:
			linearInterpolation(time, value);
			break;
		case BSpline:
			bsplineInterpolation(time, value);
			break;
		case Bezier:
			bezierInterpolation(time, value);
			break;
		case Hermite:
			hermiteInterpolation(time, value);
			break;
		case Flat:
			flatHermiteInterpolation(time, value);
			break;
		case Step:
			stepInterpolation(time, value);
			break;
		};

		return value;
	}

	Vector4d Curve::getValuePosition(double time)
	{
		return getValue(time).Position;
	}

	Vector4d Curve::getValueTangent(double time)
	{
		return getValue(time).Position;
	}

	void Curve::linearInterpolation(double time, Value& value)
	{
		Point* start = nullptr;
		Point* end = nullptr;

		for (int i = _points.size() - 2; i >= 0; i--)
		{
			if (time >= _points[i].getTime())
			{
				start = &_points[i];
				end = &_points[i + 1];
				break;
			}
		}

		if (!start || !end)
			return;

		const Vector4d startValue = start->getValue();
		const Vector4d endValue = end->getValue();

		//WORKS
		value.Position = Vector4d::lerp(startValue, endValue, Math::range(time, start->getTime(), end->getTime(), 0.0, 1.0));
		value.Tangent = endValue - startValue;
	}

	void Curve::bsplineInterpolation(double time, Value& value)
	{
		Point* start = nullptr;
		Point* end = nullptr;
		Point* out = nullptr;
		Point* in = nullptr;

		for (int i = _points.size() - 2; i >= 0; i--)
		{
			if (time >= _points[i].getTime())
			{
				start = &_points[i];
				end = &_points[i + 1];

				if (i >= 1)
					in = &_points[i - 1];
				if (i < _points.size() - 2)
					out = &_points[i + 2];
				break;
			}
		}

		if (!start || !end)
			return;

		time = Math::range(time, start->getTime(), end->getTime(), 0.0, 1.0);

		double ti = 1.0 - time;
		double t2 = time * time;
		double t3 = time * time * time;

		const Vector4d startValue = start->getValue();
		const Vector4d endValue = end->getValue();
		const Vector4d outValue = out ? out->getValue() : end->getValue();
		const Vector4d inValue = in ? in->getValue() : start->getValue();

		//WORKS
		value.Position = (ti * ti * ti) / 6.0 * inValue + (3.0 * t3 - 6.0 * t2 + 4.0) / 6.0 * startValue
			+ (-3.0 * t3 + 3.0 * t2 + 3.0 * time + 1.0) / 6.0 * endValue + t3 / 6.0 * outValue;

		value.Tangent = -(ti * ti) / 2.0 * inValue + (9.0 * t2 - 12.0 * time) / 6.0 * startValue
			+ (-9.0 * t2 + 6.0 * time + 3.0) / 6.0 * endValue + t2 / 2.0 * outValue;
	}
	
	void Curve::bezierInterpolation(double time, Value& value)
	{
		Point* start = nullptr;
		Point* end = nullptr;
		Point* out = nullptr;
		Point* in = nullptr;

		for (int i = 1; i < _points.size(); i++)
		{
			if (time <= _points[i].getTime())
			{
				start = &_points[i - 1];
				end = &_points[i];

				if (i >= 2)
					in = &_points[i - 2];
				else
					in = start;

				if (i < _points.size() - 1)
					out = &_points[i + 1];
				else
					out = end;
				break;
			}
		}

		if (!start || !end)
			return;

		time = Math::range(time, in->getTime(), out->getTime(), 0.0, 1.0);

		double ti = 1.0 - time;

		const Vector4d startValue = start->getValue();
		const Vector4d endValue = end->getValue();
		const Vector4d outValue = out->getValue();
		const Vector4d inValue = in->getValue();

		//DOESN'T WORK
		value.Position = (ti * ti * ti) * inValue + (3 * time * ti * ti) * startValue
			+ (3.0 * time * time * ti) * endValue + (time * time * time) * outValue;
	}
	
	void Curve::hermiteInterpolation(double time, Value& value)
	{
		Point* start = nullptr;
		Point* end = nullptr;
		Point* out = nullptr;
		Point* in = nullptr;

		for (int i = _points.size() - 2; i >= 0; i--)
		{
			if (time >= _points[i].getTime())
			{
				start = &_points[i];
				end = &_points[i + 1];

				if (i >= 1)
					in = &_points[i - 1];
				if (i < _points.size() - 2)
					out = &_points[i + 2];
				break;
			}
		}

		if (!start || !end)
			return;

		if (!in)
			in = start;
		if (!out)
			out = end;

		time = Math::range(time, in->getTime(), out->getTime(), 0.0, 1.0);

		double t2 = time * time;
		double t3 = time * time * time;

		const Vector4d startValue = start->getValue();
		const Vector4d endValue = end->getValue();
		const Vector4d outValue = out ? out->getValue() : end->getValue();
		const Vector4d inValue = in ? in->getValue() : start->getValue();

		//f(p_0, v_0, v_1, p_1, t) = p_0(2 t ^ 3 - 3 t ^ 2 + 1) + v_1(t ^ 3 - t ^ 2) + v_0(t ^ 3 - 2 t ^ 2 + t) + p_1(3 t ^ 2 - 2 t ^ 3)

		//DOESN'T WORK
		value.Position = inValue * (2 * t3 - 3 * t2 + 1) + endValue * (t3 - t2) + startValue * (t3 - 2 * t2 + time) + outValue * (3 * t2 - 2 * t3);
	}

	void Curve::flatHermiteInterpolation(double time, Value& value)
	{
		Point* start = nullptr;
		Point* end = nullptr;

		for (int i = _points.size() - 2; i >= 0; i--)
		{
			if (time >= _points[i].getTime())
			{
				start = &_points[i];
				end = &_points[i + 1];
				break;
			}
		}

		if (!start || !end)
			return;

		time = Math::range(time, start->getTime(), end->getTime(), 0.0, 1.0);

		double t2 = time * time;
		double t3 = time * time * time;

		const Vector4d startValue = start->getValue();
		const Vector4d endValue = end->getValue();

		//f(p_0, v_0, v_1, p_1, t) = p_0(2 t ^ 3 - 3 t ^ 2 + 1) + v_1(t ^ 3 - t ^ 2) + v_0(t ^ 3 - 2 t ^ 2 + t) + p_1(3 t ^ 2 - 2 t ^ 3)

		//DOESN'T WORK
		value.Position = startValue * (2 * t3 - 3 * t2 + 1) + endValue * (3 * t2 - 2 * t3);
	}

	void Curve::stepInterpolation(double time, Value& value)
	{
		for (int i = _points.size() - 1; i >= 0; i--)
		{
			if (time >= _points[i].getTime())
			{
				//value = _points[i].getValue();
				break;
			}
		}
	}

	Curve& Curve::operator=(const Curve& other)
	{
		_points = other._points;
		_method = other._method;

		return *this;
	}



	//Point

	Curve::Point::Point() : _time(0.0), _value(0, 0, 0, 0)
	{

	}
	Curve::Point::Point(double time, const Vector4d& value)
		: _time(time), _value(0, 0, 0, 0)
	{
		setValue(value);
	}
	Curve::Point::Point(const Point& other) : _time(other._time), _value(0, 0, 0, 0)
	{
		setValue(other._value);
	}
	Curve::Point::~Point()
	{
	}

	double Curve::Point::getTime() const
	{
		return _time;
	}

	const Vector4d& Curve::Point::getValue() const
	{
		return _value;
	}

	void Curve::Point::setValue(const Vector4d& value)
	{
		_value = value;
	}

	Curve::Point& Curve::Point::operator=(const Point& other)
	{
		_time = other._time;
		_value = other._value;

		return *this;
	}
}
