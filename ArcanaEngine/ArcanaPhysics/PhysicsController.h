#ifndef PHYSICS_CONTROLLER_H_
#define PHYSICS_CONTROLLER_H_

#include "PhysicsDefines.h"

#include "CoreDefines.h"
#include "ArcanaLog.h"
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

	class ARCANA_PHYSICS_API PhysicsController
	{
		friend class PhysicsWorld;
		friend class CollisionShape;
		friend class PhysicsMaterial;

	public:

		static PhysicsController& instance();

		void initialize();

	private:

		PhysicsController();

		~PhysicsController();

		physx::PxPhysics* getPhysicsCore();

	private:

		bool _initialized;

		physx::PxDefaultAllocator _allocator;
		PhysicsErrorCallback _errorCallback;

		physx::PxFoundation* _foundation;
		physx::PxPhysics* _physics;
	};
}

#endif // !PHYSICS_CONTROLLER_H_