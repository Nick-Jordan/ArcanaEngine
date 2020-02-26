#ifndef MOVEMENT_COMPONENT_H_
#define MOVEMENT_COMPONENT_H_

#include "SceneDefines.h"

#include "SceneComponent.h"

namespace Arcana
{
	class ARCANA_SCENE_API MovementComponent : public ActorComponent
	{
	public:

		MovementComponent();

		~MovementComponent();

		virtual void registered() override;

		SceneComponent* getMovingSceneComponent() const;

		void updateMovement(const Vector3d& delta, const Quaterniond& rotation);

	private:

		SceneComponent* _sceneComponent;

		//plane constraint

		Vector3d _velocity;
	};
}

#endif // !MOVEMENT_COMPONENT_H_

