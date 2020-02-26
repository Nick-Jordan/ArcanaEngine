#include "FPSCharacter.h"

#include "Globals.h"
#include "CharacterMovementComponent.h"

#include "ResourceManager.h"
#include "ResourceCreator.h"

namespace Arcana
{

	FPSCharacter::FPSCharacter()
	{
	}


	FPSCharacter::~FPSCharacter()
	{
	}

	void FPSCharacter::begin()
	{
		Character::begin();

		//addComponent(new CharacterMovementComponent());
	}

	void FPSCharacter::setupInputComponent(InputComponent* inputComponent)
	{
		inputComponent->bindAxisMapping("MoveForwards", this, &FPSCharacter::moveForward);
		inputComponent->bindAxisMapping("MoveRight", this, &FPSCharacter::moveRight);
		inputComponent->bindAxisMapping("MoveUp", this, &FPSCharacter::moveUp);

		inputComponent->bindAxisMapping("LookHorizontal", this, &FPSCharacter::lookHorizontal);
		inputComponent->bindAxisMapping("LookVertical", this, &FPSCharacter::lookVertical);
		inputComponent->bindAxisMapping("Roll", this, &FPSCharacter::roll);

		inputComponent->bindActionMapping("Jump", KeyEvent::Pressed, false, this, &FPSCharacter::startJump);
		inputComponent->bindActionMapping("Jump", KeyEvent::Released, false, this, &FPSCharacter::stopJump);
	}

	void FPSCharacter::moveForward(float input)
	{
		if (input != 0.0f && getController())
		{
			Vector3f dir = getSceneComponent()->getForwardVector().cast<float>();

			addMovementInput(dir, input);
		}
	}

	void FPSCharacter::moveRight(float input)
	{
		if (input != 0.0f && getController())
		{
			Vector3f dir = getSceneComponent()->getRightVector().cast<float>();

			addMovementInput(dir, input);
		}

	}

	void FPSCharacter::moveUp(float input)
	{
		if (input != 0.0f && getController())
		{
			Vector3f dir = getSceneComponent()->getUpVector().cast<float>();

			addMovementInput(dir, input);
		}
	}

	void FPSCharacter::lookHorizontal(float input)
	{
		Character::addYawInput(input - GEngine->getStationaryCursorPosition().x);
	}

	void FPSCharacter::lookVertical(float input)
	{
		Character::addPitchInput(GEngine->getStationaryCursorPosition().y - input);
	}

	void FPSCharacter::roll(float input)
	{
		Character::addRollInput(input*0.1);
	}

	void FPSCharacter::startJump()
	{
		LOG(Info, CoreEngine, "started jump");
	}

	void FPSCharacter::stopJump()
	{
		LOG(Info, CoreEngine, "stopped jump");
	}

	class FPSCharacterResource : public ResourceCreator<FPSCharacter>
	{
	public:

		FPSCharacterResource(const GlobalObjectID& id, const std::string& type, const ResourceData& data, Scheduler* dependencyScheduler)
			: ResourceCreator<FPSCharacter>(id, type, data, dependencyScheduler)
		{
			//figure out how to do inheritance (i.e. load actor params)


			initialize(id.getName());

			//actor parameters

			setMobility(Actor::convertStringToMobility(data.getStringParameter("mobility")));
			setActive(true);//test

			//for (auto iter = data.getAdditionalData().begin(); iter != data.getAdditionalData().end(); iter++)
			for (int32 i = 0; i < data.getAdditionalData().size(); i++)
			{
				auto dataPoint = data.getAdditionalData()[i];

				const ResourceData& dataPointResourceData = dataPoint.value;

				if (dataPoint.key == "child")
				{
					std::string actorName = "test_actorchild_" + id.getName();

					LoadResourceTask<Actor>* task = ResourceManager::instance().buildResource<Actor>(GlobalObjectID(actorName), dataPoint.key, dataPointResourceData, dependencyScheduler);

					if (task)
					{
						task->wait();
						childTasks.add(task);
					}
				}
				else if (dataPoint.key == "transform")
				{
					LoadResourceTask<Transform>* buildTask = ResourceManager::instance().buildResource<Transform>(GlobalObjectID(id.getName() + "::transform"), dataPoint.key, dataPointResourceData, dependencyScheduler);
					buildTask->wait();
					setTransform(*buildTask->get());
				}
				else
				{
					std::string actorCompName = "test_actorcomponent_" + id.getName();

					LoadResourceTask<ActorComponent>* task = ResourceManager::instance().buildResource<ActorComponent>(GlobalObjectID(actorCompName), dataPoint.key, dataPointResourceData, dependencyScheduler);

					if (task)
					{
						task->wait();
						componentTasks.add(task);
					}
				}
			}

			for (auto iter = data.getResourceDependencies().begin(); iter != data.getResourceDependencies().end(); iter++)
			{
				auto dataPoint = *iter;

				if (dataPoint.Type == "actor")
				{
					std::string actorName = "test_actorchild_" + id.getName();

					LoadResourceTask<Actor>* task = ResourceManager::instance().loadResource<Actor>(data.getResourceDependency(dataPoint.Name), dependencyScheduler);

					if (task)
					{
						task->wait();
						childTasks.add(task);
					}
				}
				else
				{
					std::string actorCompName = "test_actorcomponent_" + id.getName();

					LoadResourceTask<ActorComponent>* task = ResourceManager::instance().loadResource<ActorComponent>(data.getResourceDependency(dataPoint.Name), dependencyScheduler);

					if (task)
					{
						task->wait();
						componentTasks.add(task);
					}
				}
			}
		}

		void syncInitialize() override
		{
			for (auto i = componentTasks.createConstIterator(); i; i++)
			{
				auto task = *i;

				ActorComponent* comp = task->get();
				if (comp)
				{
					addComponent(comp);
				}
			}

			for (auto i = childTasks.createConstIterator(); i; i++)
			{
				auto task = *i;

				Actor* child = task->get();
				if (child)
				{
					addChild(child);
				}
			}
		}

	private:

		Array<LoadResourceTask<ActorComponent>*> componentTasks;
		Array<LoadResourceTask<Actor>*> childTasks;
	};

	Resource::Type<FPSCharacterResource, true> fpsCharacterResource("fpsCharacter");
}
