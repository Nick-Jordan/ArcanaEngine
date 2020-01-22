#include "World.h"

#include "CameraComponent.h"

#include "Profiler.h"
#include "ResourceCreator.h"
#include "ResourceManager.h"

namespace Arcana
{
	World::World() : _cameraActor(nullptr)
	{
	}
	World::World(const std::string& id) :_id(id), _cameraActor(nullptr)
	{
		initialize();
	}
	World::~World()
	{
		_renderer.finalize();

		/*for (auto i = _actors.createIterator(); i; i++)
		{
			AE_RELEASE(*i);
		}

		_actors.empty();*/

		_cameraActor = nullptr;
	}

	bool World::destroyActor(Actor* actor)
	{
		if (actor)
		{
			if (!actor->isInWorld(this))
			{
				LOG(Warning, CoreEngine, "World cannot destroy another world's actor");
				return false;
			}

			if (actor->isPendingDestroy())
			{
				LOG(Warning, CoreEngine, "Actor already pending destruction");
				return false;
			}

			actor->destroyCallback().executeIfBound();
			actor->destroyed();

			for (uint32 i = 0; i < actor->getNumChildren(); i++)
			{
				Actor* child = actor->getChild(i);
				if (child)
				{
					child->destroy();
				}
			}

			//deal with cameraActor

			actor->setParent(nullptr);

			Array<ActorComponent*> components;
			actor->getComponents(components);

			for (auto i = components.createIterator(); i; i++)
			{
				ActorComponent* component = *i;

				if (component)
				{
					component->unregisterComponent();
				}
			}

			actor->markForDestruction();
			actor->markComponentsForDestruction();

			return true;
		}

		return false;
	}

	void World::addActor(Actor* actor)
	{
		if (!actor)
			return;

		if (actor->_world != this)
		{
			//remove actor from other world

			actor->_world = this;
		}

		if (actor->hasActiveComponent<CameraComponent>())
		{
			_cameraActor = actor;
		}

		actor->reference();
		_actors.add(actor);

		actor->addComponent(actor->_sceneComponent);

		Array<ActorComponent*> components;
		actor->getComponents(components);
		for (auto i = components.createConstIterator(); i; i++)
		{
			ActorComponent* comp = *i;
			if (comp && !comp->isRegistered())
			{
				comp->registerComponent();
			}
		}
	}

	Actor* World::getActor(uint32 index) const
	{
		if (index < getNumActors())
		{
			return _actors[index];
		}

		return nullptr;
	}

	Actor* World::getActor(const std::string& name) const
	{
		return nullptr;
	}

	uint32 World::getNumActors() const
	{
		return _actors.size();
	}

	void World::componentAdded(Actor* actor, ActorComponent* component)
	{
		CameraComponent* camera = dynamic_cast<CameraComponent*>(component);

		if (camera)
		{
			_cameraActor = actor;
		}
	}

	void World::updateActors(double elapsedTime)
	{
		for (auto i = _actors.createConstIterator(); i; i++)
		{
			Actor* actor = *i;

			if (actor->isActive())
			{
				actor->update(elapsedTime);
			}
		}
	}

	void World::renderActors()
	{
		RenderData data;

		{
			PROFILE("getCameraView");
			if (_cameraActor)
			{
				_cameraActor->getCameraView(data.View, data.Projection, data.EyePosition);
			}
		}

		{
			PROFILE("Actor Render");
			for (auto i = _actors.createConstIterator(); i; i++)
			{
				Actor* actor = *i;

				if (!_cameraActor)
				{
					actor->getCameraView(data.View, data.Projection, data.EyePosition);
				}

				actor->render(_renderer);
			}
		}

		{
			PROFILE("ObjectRenderer Render");
			_renderer.render(data);
		}

		{
			PROFILE("Actor destroy check");
			for (auto i = _actors.createIterator(); i; i++)
			{
				Actor* actor = *i;

				if (actor->isPendingDestroy())
				{
					actor->allowDestruction();

					Array<ActorComponent*> components;
					actor->getComponents(components);

					for (auto i = components.createIterator(); i; i++)
					{
						ActorComponent* component = *i;

						if (component)
						{
							component->allowDestruction();
						}
					}
					_actors.remove(actor);
				}
			}
		}
	}


	ObjectRenderer& World::getRenderer()
	{
		return _renderer;
	}

	const GlobalObjectID& World::getId() const
	{
		return _id;
	}

	void World::setId(const GlobalObjectID& id)
	{
		_id = id;
	}

	void World::initialize()
	{
		_renderer.initialize();
	}

	class WorldResource : public ResourceCreator<World>
	{
	public:

		WorldResource(const GlobalObjectID& id, const std::string& type, const ResourceData& data)
			: ResourceCreator<World>(id, type, data)
		{
			setId(id);

			for (auto iter = data.getAdditionalData().begin(); iter != data.getAdditionalData().end(); iter++)
			{
				auto dataPoint = *iter;

				const ResourceData& dataPointResourceData = dataPoint.value;

				std::string actorName = "test_actor_" + id.getName();

				LoadResourceTask<Actor>* task = ResourceManager::instance().buildResource<Actor>(GlobalObjectID(actorName), dataPoint.key, dataPointResourceData);

				if (task)
				{
					task->wait();
					actorTasks.add(task);
				}
			}

			for (auto iter = data.getResourceDependencies().begin(); iter != data.getResourceDependencies().end(); iter++)
			{
				auto dataPoint = *iter;

				LoadResourceTask<Actor>* task = ResourceManager::instance().loadResource<Actor>(data.getResourceDependency(dataPoint.Name));

				if (task)
				{
					task->wait();
					actorTasks.add(task);
				}
			}
		}

		virtual void syncInitialize() override
		{
			initialize();

			for (auto i = actorTasks.createConstIterator(); i; i++)
			{
				auto task = *i;

				Actor* actor = task->get();
				if (actor)
				{
					addActor(actor);
				}
			}
		}

	private:

		Array<LoadResourceTask<Actor>*> actorTasks;
	};

	Resource::Type<WorldResource, true> worldResource("world");
}