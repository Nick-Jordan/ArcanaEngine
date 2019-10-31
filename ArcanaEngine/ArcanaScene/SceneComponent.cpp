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
			_relativeTransform(component._relativeTransform),
			_attachment(component._attachment),
			_mobility(Mobility::Dynamic)
	{
		if(_attachment)
			_attachment->reference();
	}

	SceneComponent::~SceneComponent()
	{
		AE_RELEASE(_attachment);

		_relativeTransform.removeListener(this);
	}

	void SceneComponent::initialize()
	{
		ActorComponent::initialize();

		_absolutePosition = false;
		_absoluteRotation = false;
		_absoluteScale = false;

		_velocity = Vector3d::zero();

		_relativeTransform = Transform();

		_attachment = nullptr;

		_dirtyTransform = true;

		_relativeTransform.addListener(this);
	}


	void SceneComponent::attach(SceneComponent* parent, AttachmentRule attachmentType)
	{
		attach(parent, TransformAttachmentRules(attachmentType, attachmentType, attachmentType));
	}

	void SceneComponent::attach(SceneComponent* parent, const TransformAttachmentRules& attachmentRules)
	{
		if (parent)
		{
			if (parent == _attachment && parent->_attachedChildren.contains(this))
			{
				return;
			}

			if (parent == this)
			{
				LOG(Warning, CoreEngine, "SceneComponent: attach(), component cannot be attached to itself!");
				return;
			}

			if (parent->isAttachedTo(this))
			{
				LOG(Warning, CoreEngine, "SceneComponent: attach(), parent is already attached to this component!");
				return;
			}

			_attachment = parent;
			//on attachment callback;
			parent->_attachedChildren.add(this);

			_attachmentRules.positionRule = attachmentRules.positionRule;
			_attachmentRules.rotationRule = attachmentRules.rotationRule;
			_attachmentRules.scaleRule = attachmentRules.scaleRule;

			dirtyTransform();
		}
	}

	void SceneComponent::detach(bool maintainPosition)
	{
		if (_attachment != nullptr)
		{
			_attachment->_attachedChildren.remove(this);

			_attachment = nullptr;

			if (maintainPosition)
			{
				_relativeTransform = _finalTransform;
			}

			dirtyTransform();
		}
	}

	bool SceneComponent::isAttachedTo(SceneComponent* component) const
	{
		if (component != nullptr)
		{
			for (const SceneComponent* comp = this->_attachment; comp != nullptr; comp = comp->_attachment)
			{
				if (component == comp)
				{
					return true;
				}
			}
		}
		return false;
	}

	SceneComponent* SceneComponent::getAttachParent() const
	{
		return _attachment;
	}


	void SceneComponent::useAbsolutePosition(bool absolute)
	{
		_absolutePosition = absolute;

		dirtyTransform();
	}

	bool SceneComponent::hasAbsolutePosition() const
	{
		return _absolutePosition;
	}

	void SceneComponent::useAbsoluteScale(bool absolute)
	{
		_absoluteScale = absolute;

		dirtyTransform();
	}

	bool SceneComponent::hasAbsoluteScale() const
	{
		return _absoluteScale;
	}

	void SceneComponent::useAbsoluteRotation(bool absolute)
	{
		_absoluteRotation = absolute;

		dirtyTransform();
	}

	bool SceneComponent::hasAbsoluteRotation() const
	{
		return _absoluteRotation;
	}

	void SceneComponent::setPosition(const Vector3d& position)
	{
		_relativeTransform.setTranslation(position);

		dirtyTransform();
	}

	void SceneComponent::translate(const Vector3d& delta)
	{
		_relativeTransform.translate(delta);

		dirtyTransform();
	}

	void SceneComponent::setRotation(const Quaterniond& rotation)
	{
		_relativeTransform.setRotation(rotation);

		dirtyTransform();
	}

	void SceneComponent::rotate(const Quaterniond& delta)
	{
		_relativeTransform.rotate(delta);

		dirtyTransform();
	}

	void SceneComponent::setScale(const Vector3d& scale)
	{
		_relativeTransform.setScale(scale);

		dirtyTransform();
	}

	void SceneComponent::scale(const Vector3d& delta)
	{
		_relativeTransform.scale(delta);

		dirtyTransform();
	}

	void SceneComponent::setTransform(const Transform& transform)
	{
		_relativeTransform.set(transform);

		dirtyTransform();
	}

	void SceneComponent::transform(const Transform& transform)
	{
		_relativeTransform.transform(transform);

		dirtyTransform();
	}

	Transform SceneComponent::getWorldTransform()
	{
		updateTransform();

		Transform relative = _finalTransform;

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

	Transform SceneComponent::getRelativeTransform()
	{
		updateTransform();

		return _finalTransform;
	}

	Transform& SceneComponent::getLocalRelativeTransform()
	{
		return _relativeTransform;
	}

	const Vector3d& SceneComponent::getWorldPosition()
	{
		updateTransform();

		Vector3d relative = _finalTransform.getTranslation();

		if (getOwner() && !_absolutePosition)
		{
			relative += getOwner()->getTransform().getTranslation();
		}

		return relative;
	}

	const Vector3d& SceneComponent::getRelativePosition()
	{
		updateTransform();

		return _finalTransform.getTranslation();
	}

	const Quaterniond& SceneComponent::getWorldRotation()
	{
		updateTransform();

		Quaterniond relative = _finalTransform.getRotation();

		if (getOwner() && !_absoluteRotation)
		{
			relative *= getOwner()->getTransform().getRotation();
		}

		return relative;
	}

	const Quaterniond& SceneComponent::getRelativeRotation()
	{
		updateTransform();

		return _finalTransform.getRotation();
	}

	const Vector3d& SceneComponent::getWorldScale()
	{
		updateTransform();

		Vector3d relative = _finalTransform.getScale();

		if (getOwner() && !_absoluteScale)
		{
			relative = relative * getOwner()->getTransform().getScale();
		}

		return relative;
	}

	const Vector3d& SceneComponent::getRelativeScale()
	{
		updateTransform();

		return _finalTransform.getScale();
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
		updateTransform();

		return _finalTransform.getForwardVector();
	}
	Vector3d SceneComponent::getBackVector()
	{
		updateTransform();

		return _finalTransform.getBackVector();
	}
	Vector3d SceneComponent::getUpVector()
	{
		updateTransform();

		return _finalTransform.getUpVector();
	}
	Vector3d SceneComponent::getDownVector()
	{
		updateTransform();

		return _finalTransform.getDownVector();
	}
	Vector3d SceneComponent::getLeftVector()
	{
		updateTransform();

		return _finalTransform.getLeftVector();
	}
	Vector3d SceneComponent::getRightVector()
	{
		updateTransform();

		return _finalTransform.getRightVector();
	}

	void SceneComponent::setMobility(Mobility mobility)
	{
		_mobility = mobility;
	}

	Mobility SceneComponent::getMobility() const
	{
		return _mobility;
	}

	void SceneComponent::dirtyTransform()
	{
		_dirtyTransform = true;

		for (auto i = _attachedChildren.createIterator(); i; i++)
		{
			(*i)->dirtyTransform();
		}
	}

	void SceneComponent::cleanTransform()
	{
		_dirtyTransform = false;

		for (auto i = _attachedChildren.createIterator(); i; i++)
		{
			(*i)->cleanTransform();
		}
	}

	void SceneComponent::updateTransform()
	{
		if (_dirtyTransform)
		{
			_finalTransform = getAttachmentTransform();
			cleanTransform();
		}
	}

	Transform SceneComponent::getAttachmentTransform() const
	{
		Transform finalTransform = _relativeTransform;

		if (_attachment)
		{
			switch (_attachmentRules.positionRule)
			{
			case AttachmentRule::KeepWorld:
			{
				finalTransform.setTranslation(_relativeTransform.getTranslation());
				break;
			}
			break;

			case AttachmentRule::KeepRelative:
			{
				Transform parent = _attachment->getWorldTransform();
				Transform relative = _relativeTransform.getRelativeTransform(parent);

				finalTransform.setTranslation(relative.getTranslation());
			}
			break;

			case AttachmentRule::SnapTo:
			{
				Transform parent = _attachment->getWorldTransform();

				finalTransform.setTranslation(parent.getTranslation());
			}
			break;

			default:
				break;
			}


			switch (_attachmentRules.rotationRule)
			{
			case AttachmentRule::KeepWorld:
			{
				finalTransform.setRotation(_relativeTransform.getRotation());
				break;
			}
			break;

			case AttachmentRule::KeepRelative:
			{
				Transform parent = _attachment->getWorldTransform();
				Transform relative = _relativeTransform.getRelativeTransform(parent);

				finalTransform.setRotation(relative.getRotation());
			}
			break;

			case AttachmentRule::SnapTo:
			{
				Transform parent = _attachment->getWorldTransform();

				finalTransform.setRotation(parent.getRotation());
			}
			break;

			default:
				break;
			}

			switch (_attachmentRules.scaleRule)
			{
			case AttachmentRule::KeepWorld:
			{
				finalTransform.setScale(_relativeTransform.getScale());
				break;
			}
			break;

			case AttachmentRule::KeepRelative:
			{
				Transform parent = _attachment->getWorldTransform();
				Transform relative = _relativeTransform.getRelativeTransform(parent);

				finalTransform.setScale(relative.getScale());
			}
			break;

			case AttachmentRule::SnapTo:
			{
				Transform parent = _attachment->getWorldTransform();

				finalTransform.setScale(parent.getScale());
			}
			break;

			default:
				break;
			}
		}

		return finalTransform;
	}

	void SceneComponent::transformChanged(Transform* transform)
	{
		dirtyTransform();
	}
}
