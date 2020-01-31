#ifndef PHYSICS_WORLD_H_
#define PHYSICS_WORLD_H_

#include "PhysicsDefines.h"

#include "ArcanaLog.h"
#include "Object.h"
#include "World.h"

#include "PxPhysicsAPI.h"

namespace Arcana
{
	REGISTER_CATEGORY_ENGINE(ARCANA_PHYSICS_API, PhysicsLog, none);

	class ARCANA_PHYSICS_API PhysicsErrorCallback : public physx::PxErrorCallback
	{
	public:

		virtual void reportError(physx::PxErrorCode::Enum code, const char* message, const char* file,
			int line)
		{
			LOGF(Error, PhysicsLog, "Physics error code: %d, message: %s, file: %s, line: %d", code, message, file, line);
		}
	};

#define PXVEC3(v) physx::PxVec3(v.x, v.y, v.z)
#define PXQUAT(q) physx::PxQuat(q.x, q.y, q.z, q.w)

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

		physx::PxDefaultAllocator _allocator;
		PhysicsErrorCallback _errorCallback;

		physx::PxFoundation* _foundation;
		physx::PxPhysics* _physics;

		physx::PxDefaultCpuDispatcher* _dispatcher;
		physx::PxScene* _scene;

		std::map<Actor*, physx::PxRigidActor*> test;

		//physx::PxPvd* _pvd; //debugger
	};
}

#endif // !PHYSICS_WORLD_H_

