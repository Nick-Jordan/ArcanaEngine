#ifndef FIELD_H_
#define FIELD_H_

#include "Vector3.h"
#include "Quaternion.h"
#include "ArcanaMath.h"

#include <vector>

namespace Arcana
{
	template<typename T>
	class Field
	{
	public:

		Field() {};

		~Field() {};

		virtual T get(double x, double y, double z) const = 0;

		T get(const Vector3d& position) const
		{
			return get(position.x, position.y, position.z);
		};
	};

	typedef Field<Vector3d> VectorField;
	typedef Field<double> ScalarField;
}

#endif // !FIELD_H_