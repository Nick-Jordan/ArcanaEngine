#ifndef PHYSICS_WORLD_H_
#define PHYSICS_WORLD_H_

#include "PhysicsDefines.h"

#include "ArcanaLog.h"
#include "Object.h"
#include "World.h"

#include "PhysicsController.h"

namespace Arcana
{
	class ARCANA_PHYSICS_API PhysicsWorld : public World::Listener //inherit from some kind of WorldListener to listen for adding and deleting actors
	{
	public:

		PhysicsWorld();

		~PhysicsWorld();

		void initialize();

		virtual void worldUpdated(World* world, double elapsedTime) override;

		virtual void actorAdded(World* world, Actor* actor) override;

		virtual void actorDestroyed(World* world, Actor* actor) override;

	private:

		

		physx::PxDefaultCpuDispatcher* _dispatcher;
		physx::PxScene* _scene;

		std::map<Actor*, physx::PxRigidActor*> test;

		//physx::PxPvd* _pvd; //debugger
	};
}

#endif // !PHYSICS_WORLD_H_

