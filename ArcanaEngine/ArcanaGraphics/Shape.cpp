#include "Shape.h"

#include "ObjectRenderer.h"

namespace Arcana
{
	Shape::Shape(Mesh* mesh) : _mesh(mesh), _userData(nullptr)
	{

	}

	Shape::~Shape()
	{

	}

	Mesh* Shape::getMesh()
	{
		return _mesh;
	}

	void* Shape::getUserData() const
	{
		return _userData;
	}

	void Shape::setUserData(void* userData)
	{
		_userData = userData;
	}


	Shape& Shape::operator=(const Shape& shape)
	{
		_mesh = shape._mesh;
		_userData = shape._userData;

		return *this;
	}
}
