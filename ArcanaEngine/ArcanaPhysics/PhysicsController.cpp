#include "PhysicsController.h"

namespace Arcana
{
	INITIALIZE_CATEGORY(Arcana, PhysicsLog);

	PhysicsController& PhysicsController::instance()
	{
		static PhysicsController physicsController;
		return physicsController;
	}

	PhysicsController::PhysicsController() : _foundation(nullptr), _physics(nullptr), _initialized(false)
	{
		initialize();
	}

	PhysicsController::~PhysicsController()
	{
		AE_RELEASE(_foundation);
		AE_RELEASE(_physics);
	}

	void PhysicsController::initialize()
	{
		if (_initialized)
			return;

		_initialized = true;

		_foundation = PxCreateFoundation(PX_PHYSICS_VERSION, _allocator, _errorCallback);

		//gPvd = PxCreatePvd(*gFoundation);
		//physx::PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
		//gPvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

		_physics = PxCreatePhysics(PX_PHYSICS_VERSION, *_foundation, physx::PxTolerancesScale(), true, nullptr);
	}

	physx::PxPhysics* PhysicsController::getPhysicsCore()
	{
		return _physics;
	}
}