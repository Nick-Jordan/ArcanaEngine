#include "Shape.h"

#include "ObjectRenderer.h"

namespace Arcana
{
	Shape::Shape(ObjectRenderer& renderer) : _renderer(renderer), _mesh(nullptr), _userData(nullptr)
	{

	}

	Shape::~Shape()
	{

	}

	Mesh* Shape::getMesh()
	{
		return _mesh;
	}

	ObjectRenderer& Shape::getRenderer() const
	{
		return _renderer;
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
		_renderer = shape._renderer;
		_mesh = shape._mesh;
		_userData = shape._userData;

		return *this;
	}
}
