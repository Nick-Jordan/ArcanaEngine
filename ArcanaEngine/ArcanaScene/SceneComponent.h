#ifndef SCENE_COMPONENT_H_
#define SCENE_COMPONENT_H_

#include "SceneDefines.h"

#include "ActorComponent.h"
#include "Transform.h"

namespace Arcana
{
	class ARCANA_SCENE_API SceneComponent : public ActorComponent
	{
	public:

		SceneComponent();

		SceneComponent(const SceneComponent& component);

		virtual ~SceneComponent();
		

		virtual void initialize() override;


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


		Transform getWorldTransform() const;

		Transform& getRelativeTransform();

		const Vector3d& getWorldPosition() const;

		const Vector3d& getRelativePosition() const;

		const Quaterniond& getWorldRotation() const;

		const Quaterniond& getRelativeRotation() const;

		const Vector3d& getWorldScale() const;

		const Vector3d& getRelativeScale() const;

		const Vector3d& getWorldVelocity() const;

		const Vector3d& getRelativeVelocity() const;


		Vector3d getForwardVector();

		Vector3d getBackVector();

		Vector3d getUpVector();

		Vector3d getDownVector();

		Vector3d getLeftVector();

		Vector3d getRightVector();


		//move component? with collision checks?

	private:

		bool _absolutePosition;
		bool _absoluteRotation;
		bool _absoluteScale;
		//bool update on transform

		Vector3d _velocity;

		Transform _relativeTransform;
	};

}

#endif // !SCENE_COMPONENT_H_

