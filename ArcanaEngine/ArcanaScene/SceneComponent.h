#ifndef SCENE_COMPONENT_H_
#define SCENE_COMPONENT_H_

#include "SceneDefines.h"

#include "ActorComponent.h"
#include "Transform.h"

#include "ActorAttachment.h"

namespace Arcana
{
	enum ARCANA_SCENE_API Mobility
	{
		Static,
		Stationary,
		Dynamic
	};

	Mobility convertStringToMobility(const std::string& str);

	class ARCANA_SCENE_API SceneComponent : public ActorComponent, Transform::Listener
	{
	public:

		SceneComponent();

		SceneComponent(const SceneComponent& component);

		virtual ~SceneComponent();
		
		virtual void initialize() override;


		void attach(SceneComponent* parent, AttachmentRule attachmentType = AttachmentRule::KeepRelative);

		void attach(SceneComponent* parent, const TransformAttachmentRules& attachmentRules);

		void detach(bool maintainPosition);

		bool isAttachedTo(SceneComponent* component) const;

		SceneComponent* getAttachParent() const;


		void useAbsolutePosition(bool absolute);

		bool hasAbsolutePosition() const;

		void useAbsoluteScale(bool absolute);

		bool hasAbsoluteScale() const;

		void useAbsoluteRotation(bool absolute);

		bool hasAbsoluteRotation() const;


		void setPosition(const Vector3d& position);

		void translate(const Vector3d& delta);

		void setRotation(const Quaterniond& rotation);

		void rotate(const Quaterniond& delta);

		void setScale(const Vector3d& scale);

		void scale(const Vector3d& delta);


		void setTransform(const Transform& transform);

		void transform(const Transform& transform);


		Transform getWorldTransform();

		Transform getRelativeTransform();

		Transform& getLocalRelativeTransform();

		const Vector3d& getWorldPosition();

		const Vector3d& getRelativePosition();

		const Quaterniond& getWorldRotation();

		const Quaterniond& getRelativeRotation();

		const Vector3d& getWorldScale();

		const Vector3d& getRelativeScale();

		const Vector3d& getWorldVelocity() const;

		const Vector3d& getRelativeVelocity() const;


		Vector3d getForwardVector();

		Vector3d getBackVector();

		Vector3d getUpVector();

		Vector3d getDownVector();

		Vector3d getLeftVector();

		Vector3d getRightVector();

		void setMobility(Mobility mobility);

		Mobility getMobility() const;

		//move component? with collision checks?

	private:

		void dirtyTransform();

		void cleanTransform();

		void updateTransform();

		Transform getAttachmentTransform() const;

		virtual void transformChanged(Transform* transform) override;

	private:

		SceneComponent* _attachment;
		TransformAttachmentRules _attachmentRules;

		Array<SceneComponent*> _attachedChildren;

		bool _absolutePosition;
		bool _absoluteRotation;
		bool _absoluteScale;
		bool _dirtyTransform;
		Mobility _mobility;
		//bool update on transform

		Vector3d _velocity;

		Transform _relativeTransform;
		Transform _finalTransform;
	};

}

#endif // !SCENE_COMPONENT_H_

