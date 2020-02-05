#include "PhysicsWorld.h"

#include "Vector3.h"
#include "CollisionComponent.h"

//fun test
#include "Input.h"
#include "Random.h"

#include "ResourceManager.h"
#include "ResourceCreator.h"

/*#include <PxPhysXCommon.h>
#include <PxDefaultSimulationFilterShader.h>
#include <PxDefaultCpuDispatcher.h>
#include <PxExtensionsAPI.h>
#include <PxRigidStatic.h>
#include <PxBroadcastingAllocator.h>
#include <PxProfileZoneManager.h>
#include <PxCooking.h>
#include <PxControllerManager.h>
#include <PsMutex.h>
#include <PxSimulationEventCallback.h>*/

namespace Arcana
{
	PhysicsWorld::PhysicsWorld() : _dispatcher(nullptr), _scene(nullptr)
	{

	}

	PhysicsWorld::~PhysicsWorld()
	{
		AE_RELEASE(_dispatcher);
		AE_RELEASE(_scene);
	}

	static physx::PxFilterFlags testCCDFilterShader(
		physx::PxFilterObjectAttributes attributes0,
		physx::PxFilterData filterData0,
		physx::PxFilterObjectAttributes attributes1,
		physx::PxFilterData filterData1,
		physx::PxPairFlags& pairFlags,
		const void* constantBlock,
		physx::PxU32 constantBlockSize)
	{
		pairFlags = physx::PxPairFlag::eSOLVE_CONTACT;
		pairFlags |= physx::PxPairFlag::eDETECT_DISCRETE_CONTACT;
		pairFlags |= physx::PxPairFlag::eDETECT_CCD_CONTACT;
		return physx::PxFilterFlags();
	}

	void PhysicsWorld::initialize()
	{
		physx::PxSceneDesc sceneDesc(PhysicsController::instance().getPhysicsCore()->getTolerancesScale());
		sceneDesc.gravity = PXVEC3(Vector3f(0.0f, -9.81f, 0.0f));
		sceneDesc.flags |= physx::PxSceneFlag::eENABLE_CCD;
		sceneDesc.filterShader = testCCDFilterShader;
		_dispatcher = physx::PxDefaultCpuDispatcherCreate(2);
		sceneDesc.cpuDispatcher = _dispatcher;
		_scene = PhysicsController::instance().getPhysicsCore()->createScene(sceneDesc);

		/*PxPvdSceneClient* pvdClient = gScene->getScenePvdClient();
		if (pvdClient)
		{
			pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
			pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
			pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
		}*/
	}

	physx::PxRigidDynamic* light1;
	physx::PxRigidDynamic* light2;
	bool keyDown = false;
	bool lastKeyState = false;

	void PhysicsWorld::worldUpdated(World* world, double elapsedTime)
	{
		_scene->simulate(elapsedTime);// / 1000.0f);
		_scene->fetchResults(true);

		for (auto i = test.begin(); i != test.end(); i++)
		{
			auto pair = *i;

			physx::PxTransform transform = pair.second->getGlobalPose();

			pair.first->getSceneComponent()->setPosition(Vector3d(transform.p.x, transform.p.y, transform.p.z));
			pair.first->getSceneComponent()->setRotation(Quaterniond(transform.q.w, transform.q.x, transform.q.y, transform.q.z));
		}


		//fun test
		keyDown = Input::isKeyPressed(Keys::K);
		
		if (keyDown && keyDown != lastKeyState)
		{
			Vector3d random = Random<double>::random(Vector3d::one() * -20000.0, Vector3d::one() * 20000.0);
			light1->addForce(PXVEC3(random));
			random = Random<double>::random(Vector3d::one() * -20000.0, Vector3d::one() * 20000.0);
			light2->addForce(PXVEC3(random));
		}

		lastKeyState = keyDown;
	}

	void PhysicsWorld::actorAdded(World* world, Actor* actor)
	{
		Array<CollisionComponent*> components;
		actor->getComponents<CollisionComponent>(components);

		for (auto i = components.createConstIterator(); i; i++)
		{
			CollisionComponent* component = *i;

			if (component) //check enabled
			{
				CollisionShape* shape = component->getShape();

				if (!shape || !shape->getShape())
					continue;

				actor->getSceneComponent()->useAbsolutePosition(true);
				actor->getSceneComponent()->useAbsoluteRotation(true);
				//actor->getSceneComponent()->useAbsoluteScale(true);

				Transform t = component->getWorldTransform();

				physx::PxTransform transform(PXVEC3(t.getTranslation()), PXQUAT(t.getRotation()));
				physx::PxRigidActor* body = nullptr;
				if (component->isStatic())
				{
					body = PhysicsController::instance().getPhysicsCore()->createRigidStatic(transform);
				}
				else
				{
					physx::PxRigidDynamic* dynamicBody = PhysicsController::instance().getPhysicsCore()->createRigidDynamic(transform);
					dynamicBody->setRigidBodyFlag(physx::PxRigidBodyFlag::eENABLE_CCD, true);
					physx::PxRigidBodyExt::updateMassAndInertia(*dynamicBody, 10.0f);

					body = dynamicBody;


					//fun test
					if (actor->getName() == "greenDynamicLight")
						light1 = dynamicBody;
					if (actor->getName() == "whiteDynamicLight")
						light2 = dynamicBody;
				}

				body->attachShape(*(shape->getShape()));
				_scene->addActor(*body);

				test.emplace(actor, body);
			}
		}
	}

	void PhysicsWorld::actorDestroyed(World* world, Actor* actor)
	{

	}

	class PhysicsWorldResource : public ResourceCreator<PhysicsWorld>
	{
	public:

		PhysicsWorldResource(const GlobalObjectID& id, const std::string& type, const ResourceData& data, Scheduler* dependencyScheduler)
			: ResourceCreator<PhysicsWorld>(id, type, data, dependencyScheduler)
		{
			initialize();
		}
	};

	Resource::Type<PhysicsWorldResource> physicsWorldResource("physicsWorld");
}
