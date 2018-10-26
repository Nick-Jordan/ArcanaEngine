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
		
		if (_mesh)
		{
			_mesh->reference();
		}
	}

	Mesh* Shape::getMesh()
	{
		return _mesh;
	}

	Shape& Shape::operator=(const Shape& shape)
	{
		_mesh = shape._mesh;
		setUserData(shape.getUserData());

		return *this;
	}
}
