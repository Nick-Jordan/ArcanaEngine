
namespace Arcana
{
	template<typename T>
	HeightField<T>::HeightField(uint32 rows, uint32 columns)
		: _heightfield(nullptr), _columns(columns), _rows(rows)
	{
		_heightfield = new T[rows * columns];
	}

	template<typename T>
	HeightField<T>::~HeightField()
	{
		AE_DELETE_ARRAY(_heightfield);
	}

	template<typename T>
	T* HeightField<T>::getData() const
	{
		return _heightfield;
	}

	template<typename T>
	T HeightField<T>::getHeight(double column, double row) const
	{
		column = column < 0.0 ? 0.0 : (column > (_columns - 1) ? (_columns - 1) : column);
		row = row < 0.0 ? 0.0 : (row > (_rows - 1) ? (_rows - 1) : row);

		uint32 x1 = column;
		uint32 y1 = row;
		uint32 x2 = x1 + 1;
		uint32 y2 = y1 + 1;
		double tmp;
		double xFactor = modf(column, &tmp);
		double yFactor = modf(row, &tmp);
		double xFactorI = 1.0 - xFactor;
		double yFactorI = 1.0 - yFactor;

		if (x2 >= _columns && y2 >= _rows)
		{
			return _heightfield[x1 + y1 * _columns];
		}
		else if (x2 >= _columns)
		{
			return _heightfield[x1 + y1 * _columns] * yFactorI + _heightfield[x1 + y2 * _columns] * yFactor;
		}
		else if (y2 >= _rows)
		{
			return _heightfield[x1 + y1 * _columns] * xFactorI + _heightfield[x2 + y1 * _columns] * xFactor;
		}

		double a = xFactorI * yFactorI;
		double b = xFactorI * yFactor;
		double c = xFactor * yFactor;
		double d = xFactor * yFactorI;
		return _heightfield[x1 + y1 * _columns] * a + _heightfield[x1 + y2 * _columns] * b +
			_heightfield[x2 + y2 * _columns] * c + _heightfield[x2 + y1 * _columns] * d;
	}

	template<typename T>
	void HeightField<T>::setHeight(uint32 x, uint32 y, T value)
	{
		if (x < _rows && y < _columns)
		{
			_heightfield[x + y * _columns] = value;
		}
	}

	template<typename T>
	uint32 HeightField<T>::getNumRows() const
	{
		return _rows;
	}

	template<typename T>
	uint32 HeightField<T>::getNumColumns() const
	{
		return _columns;
	}
}