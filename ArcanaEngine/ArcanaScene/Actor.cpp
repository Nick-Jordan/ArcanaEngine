#include "Actor.h"

namespace Arcana
{

	Actor::Actor()
	{
	}

	Actor::Actor(const Actor& actor) : _shape(actor._shape), _transform(actor._transform)
	{

	}

	Actor::~Actor()
	{
	}


	void Actor::update(double elapsedTime)
	{

	}

	void Actor::render(ObjectRenderer& renderer)
	{

	}


	Transform& Actor::getTransform()
	{
		return _transform;
	}

	void Actor::setTransform(const Transform& transform)
	{
		_transform = Transform(transform);
	}

	Shape* Actor::getShape() const
	{
		return _shape;
	}

	void Actor::setShape(Shape* shape)
	{
		_shape = shape;
	}

}
