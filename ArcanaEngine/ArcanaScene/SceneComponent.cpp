#include "SceneComponent.h"

#include "Actor.h"

namespace Arcana
{
	SceneComponent::SceneComponent()
	{
		initialize();
	}

	SceneComponent::SceneComponent(const SceneComponent& component)
		: ActorComponent(component),
			_absolutePosition(component._absolutePosition),
			_absoluteRotation(component._absoluteRotation),
			_absoluteScale(component._absoluteScale),
			_velocity(component._velocity),
			_relativeTransform(component._relativeTransform)
	{

	}

	SceneComponent::~SceneComponent()
	{

	}

	void SceneComponent::initialize()
	{
		ActorComponent::initialize();

		_absolutePosition = false;
		_absoluteRotation = false;
		_absoluteScale = false;

		_velocity = Vector3d::zero();

		_relativeTransform = Transform();
	}

	void SceneComponent::useAbsolutePosition(bool absolute)
	{
		_absolutePosition = absolute;
	}

	bool SceneComponent::hasAbsolutePosition() const
	{
		return _absolutePosition;
	}

	void SceneComponent::useAbsoluteScale(bool absolute)
	{
		_absoluteScale = absolute;
	}

	bool SceneComponent::hasAbsoluteScale() const
	{
		return _absoluteScale;
	}

	void SceneComponent::useAbsoluteRotation(bool absolute)
	{
		_absoluteRotation = absolute;
	}

	bool SceneComponent::hasAbsoluteRotation() const
	{
		return _absoluteRotation;
	}

	void SceneComponent::setPosition(const Vector3d& position)
	{
		_relativeTransform.setTranslation(position);
	}

	void SceneComponent::translate(const Vector3d& delta)
	{
		_relativeTransform.translate(delta);
	}

	void SceneComponent::setRotation(const Quaterniond& rotation)
	{
		_relativeTransform.setRotation(rotation);
	}

	void SceneComponent::rotate(const Quaterniond& delta)
	{
		_relativeTransform.rotate(delta);
	}

	void SceneComponent::setScale(const Vector3d& scale)
	{
		_relativeTransform.setScale(scale);
	}

	void SceneComponent::scale(const Vector3d& delta)
	{
		_relativeTransform.scale(delta);
	}

	void SceneComponent::setTransform(const Transform& transform)
	{
		_relativeTransform.set(transform);
	}

	void SceneComponent::transform(const Transform& transform)
	{
		_relativeTransform.transform(transform);
	}

	Transform SceneComponent::getWorldTransform() const
	{
		Transform relative = _relativeTransform;

		if (getOwner())
		{
			Transform owner = getOwner()->getTransform();

			if (!_absolutePosition)
			{
				relative.translate(owner.getTranslation());
			}

			if (!_absoluteRotation)
			{
				relative.rotate(owner.getRotation());
			}

			if (!_absoluteScale)
			{
				relative.scale(owner.getScale());
			}
		}

		return relative;
	}

	Transform& SceneComponent::getRelativeTransform()
	{
		return _relativeTransform;
	}

	const Vector3d& SceneComponent::getWorldPosition() const
	{
		Vector3d relative = _relativeTransform.getTranslation();

		if (getOwner() && !_absolutePosition)
		{
			relative += getOwner()->getTransform().getTranslation();
		}

		return relative;
	}

	const Vector3d& SceneComponent::getRelativePosition() const
	{
		return _relativeTransform.getTranslation();
	}

	const Quaterniond& SceneComponent::getWorldRotation() const
	{
		Quaterniond relative = _relativeTransform.getRotation();

		if (getOwner() && !_absoluteRotation)
		{
			relative *= getOwner()->getTransform().getRotation();
		}

		return relative;
	}

	const Quaterniond& SceneComponent::getRelativeRotation() const
	{
		return _relativeTransform.getRotation();
	}

	const Vector3d& SceneComponent::getWorldScale() const
	{
		Vector3d relative = _relativeTransform.getScale();

		if (getOwner() && !_absoluteScale)
		{
			relative = relative * getOwner()->getTransform().getScale();
		}

		return relative;
	}

	const Vector3d& SceneComponent::getRelativeScale() const
	{
		return _relativeTransform.getScale();
	}

	const Vector3d& SceneComponent::getWorldVelocity() const
	{
		//apply owner velocity

		return _velocity;
	}

	const Vector3d& SceneComponent::getRelativeVelocity() const
	{
		return _velocity;
	}

	Vector3d SceneComponent::getForwardVector()
	{
		return _relativeTransform.getForwardVector();
	}
	Vector3d SceneComponent::getBackVector()
	{
		return _relativeTransform.getBackVector();
	}
	Vector3d SceneComponent::getUpVector()
	{
		return _relativeTransform.getUpVector();
	}
	Vector3d SceneComponent::getDownVector()
	{
		return _relativeTransform.getDownVector();
	}
	Vector3d SceneComponent::getLeftVector()
	{
		return _relativeTransform.getLeftVector();
	}
	Vector3d SceneComponent::getRightVector()
	{
		return _relativeTransform.getRightVector();
	}
}
