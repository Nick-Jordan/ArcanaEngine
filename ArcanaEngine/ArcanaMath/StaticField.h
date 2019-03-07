#ifndef STATIC_FIELD_H_
#define STATIC_FIELD_H_

#include "Field.h"

#include <vector>

namespace Arcana
{
	template<typename T>
	class StaticField : public Field<T>
	{
	public:

		StaticField();

		StaticField(unsigned int width, unsigned int height, unsigned int depth, T initial);

		~StaticField();

		void setValue(unsigned int i, unsigned int j, unsigned int k, T value);

		void setBoundaryValue(const T& boundary);

		const T& getBoundaryValue() const;

		T getValue(int i, int j, int k) const;

		virtual T get(double x, double y, double z) const override;

		unsigned int getWidth() const;

		unsigned int getHeight() const;

		unsigned int getDepth() const;

	private:

		std::vector<std::vector<std::vector<T>>> _field;
		T _boundary;
	public://test
		Quaterniond _rotation;
		Vector3d _scale;
		Vector3d _translation;
	private:
		unsigned int _width;
		unsigned int _height;
		unsigned int _depth;
	};

	typedef StaticField<Vector3d> StaticVectorField;
	typedef StaticField<double> StaticScalarField;
}

#include "StaticField.inl"

#endif // !STATIC_FIELD_H_