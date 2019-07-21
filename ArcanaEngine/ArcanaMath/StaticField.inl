
namespace Arcana
{
	template<typename T>
	StaticField<T>::StaticField()
		: _width(0), _height(0), _depth(0)
	{

	}

	template<typename T>
	StaticField<T>::StaticField(unsigned int width, unsigned int height, unsigned int depth, T initial)
		: _width(width), _height(height), _depth(depth), _boundary((T)0)
	{
		_field.resize(width);
		for (unsigned int i = 0; i < width; i++)
		{
			_field[i].resize(height);
			for (unsigned int j = 0; j < height; j++)
			{
				_field[i][j] = std::vector<T>(depth, initial);
			}
		}
	}

	template<typename T>
	StaticField<T>::~StaticField()
	{
		_field.clear();
	}

	template<typename T>
	void StaticField<T>::setValue(unsigned int i, unsigned int j, unsigned int k, T value)
	{
		if (i < _width && j < _height && k < _depth)
		{
			_field[i][j][k] = value;
		}
	}

	template<typename T>
	void StaticField<T>::setBoundaryValue(const T& boundary)
	{
		_boundary = boundary;
	}

	template<typename T>
	const T& StaticField<T>::getBoundaryValue() const
	{
		return _boundary;
	}

	template<typename T>
	T StaticField<T>::getValue(int i, int j, int k) const
	{
		if (i < _width && j < _height && k < _depth
			&& i >= 0 && j >= 0 && k >= 0)
		{
			return _field[i][j][k];
		}
		
		return _boundary;
	}

	template<typename T>
	T StaticField<T>::get(double x, double y, double z) const
	{
		x = x - _translation.x;
		y = y - _translation.y;
		z = z - _translation.z;

		unsigned int x1 = x;
		unsigned int y1 = y;
		unsigned int z1 = z;
		unsigned int x2 = x1 + 1;
		unsigned int y2 = y1 + 1;
		unsigned int z2 = z1 + 1;

		double temp;
		double xFactor = modf(x, &temp);
		double yFactor = modf(y, &temp);
		double zFactor = modf(z, &temp);
		double xFactorI = 1.0 - xFactor;
		double yFactorI = 1.0 - yFactor;
		double zFactorI = 1.0 - zFactor;

		double a = xFactorI * yFactorI * zFactorI;
		double b = xFactorI * yFactorI * zFactor;
		double c = xFactorI * yFactor * zFactorI;
		double d = xFactorI * yFactor * zFactor;
		double e = xFactor * yFactorI * zFactorI;
		double f = xFactor * yFactorI * zFactor;
		double g = xFactor * yFactor * zFactorI;
		double h = xFactor * yFactor * zFactor;

		return getValue(x1, y1, z1) * a + getValue(x1, y1, z2) * b + getValue(x1, y2, z1) * c
			+ getValue(x1, y2, z2) * d + getValue(x2, y1, z1) * e + getValue(x2, y1, z2) * f
			+ getValue(x2, y2, z1) * g + getValue(x2, y2, z2) * h;
	}

	template<typename T>
	unsigned int StaticField<T>::getWidth() const
	{
		return _width;
	}

	template<typename T>
	unsigned int StaticField<T>::getHeight() const
	{
		return _height;
	}

	template<typename T>
	unsigned int StaticField<T>::getDepth() const
	{
		return _depth;
	}
}