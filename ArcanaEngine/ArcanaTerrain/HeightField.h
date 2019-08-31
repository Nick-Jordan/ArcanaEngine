#ifndef HEIGHTFIELD_H_
#define HEIGHTFIELD_H_

#include "Object.h"
#include "Types.h"

namespace Arcana
{
	template<typename T>
	class HeightField : public Object
	{
	public:

		HeightField(uint32 rows, uint32 columns);

		~HeightField();

		T* getData() const;

		T getHeight(double column, double row) const;

		void setHeight(uint32 x, uint32 y, T value);

		uint32 getNumRows() const;

		uint32 getNumColumns() const;

	private:

		T* _heightfield;
		uint32 _columns;
		uint32 _rows;
	};
}

#include "HeightField.inl"

#endif // !HEIGHTFIELD_H_