#include "Shape.h"

#include "ObjectRenderer.h"

namespace Arcana
{
	Shape::Shape() : Object("Shape"), _mesh(nullptr)
	{

	}

	Shape::~Shape()
	{
		if (_mesh)
		{
			AE_RELEASE(_mesh);
		}
	}

	void Shape::setMesh(Mesh* mesh)
	{
		_mesh = mesh;
	}

	Mesh* Shape::getMesh()
	{
		return _mesh;
	}

	Shape& Shape::operator=(const Shape& shape)
	{
		Object::operator=(shape);

		_mesh = shape._mesh;

		return *this;
	}
}
