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
	INITIALIZE_CATEGORY(Arcana, PhysicsLog);

	PhysicsWorld::PhysicsWorld() :  _foundation(nullptr), _physics(nullptr), _dispatcher(nullptr), _scene(nullptr)
	{

	}

	PhysicsWorld::~PhysicsWorld()
	{
		AE_RELEASE(_foundation);
		AE_RELEASE(_physics);
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
		_foundation = PxCreateFoundation(PX_PHYSICS_VERSION, _allocator, _errorCallback);

		//gPvd = PxCreatePvd(*gFoundation);
		//physx::PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
		//gPvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

		_physics = PxCreatePhysics(PX_PHYSICS_VERSION, *_foundation, physx::PxTolerancesScale(), true, nullptr);

		physx::PxSceneDesc sceneDesc(_physics->getTolerancesScale());
		sceneDesc.gravity = PXVEC3(Vector3f(0.0f, -9.81f, 0.0f));
		sceneDesc.flags |= physx::PxSceneFlag::eENABLE_CCD;
		sceneDesc.filterShader = testCCDFilterShader;
		_dispatcher = physx::PxDefaultCpuDispatcherCreate(2);
		sceneDesc.cpuDispatcher = _dispatcher;
		sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;
		_scene = _physics->createScene(sceneDesc);

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

				actor->getSceneComponent()->useAbsolutePosition(true);
				actor->getSceneComponent()->useAbsoluteRotation(true);
				//actor->getSceneComponent()->useAbsoluteScale(true);

				Transform t = component->getWorldTransform();

				physx::PxMaterial* material = _physics->createMaterial(0.5f, 0.5f, 0.06f);

				//test (just a box)
				physx::PxShape* shape = _physics->createShape(physx::PxBoxGeometry(t.getScale().x, t.getScale().y, t.getScale().z), *material);

				physx::PxTransform transform(PXVEC3(t.getTranslation()), PXQUAT(t.getRotation()));
				physx::PxRigidActor* body = nullptr;
				if (component->isStatic())
				{
					body = _physics->createRigidStatic(transform);
				}
				else
				{
					physx::PxRigidDynamic* dynamicBody = _physics->createRigidDynamic(transform);
					dynamicBody->setRigidBodyFlag(physx::PxRigidBodyFlag::eENABLE_CCD, true);
					physx::PxRigidBodyExt::updateMassAndInertia(*dynamicBody, 10.0f);

					body = dynamicBody;


					//fun test
					if (actor->getName() == "greenDynamicLight")
						light1 = dynamicBody;
					if (actor->getName() == "whiteDynamicLight")
						light2 = dynamicBody;
				}

				body->attachShape(*shape);
				_scene->addActor(*body);

				test.emplace(actor, body);

				AE_RELEASE(shape);
				AE_RELEASE(material);
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
