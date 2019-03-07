#ifndef DYNAMIC_FIELD_H_
#define DYNAMIC_FIELD_H_

#include "Field.h"

#include <vector>

namespace Arcana
{
	template<typename T>
	class DynamicField : public Field<T>
	{
	public:

		typedef T(*DynamicFieldEvaluation)(double x, double y, double z);

		DynamicField() : _evaluation(nullptr) {};

		DynamicField(DynamicFieldEvaluation eval) : _evaluation(eval) {};

		~DynamicField() {};

		virtual T get(double x, double y, double z) const
		{
			if (_evaluation)
			{
				return _evaluation(x, y, z);
			}

			return (T)0;
		};

	private:

		DynamicFieldEvaluation _evaluation;
	};

	typedef DynamicField<Vector3d> DynamicVectorField;
	typedef DynamicField<double> DynamicScalarField;
}

#endif // !DYNAMIC_FIELD_H_