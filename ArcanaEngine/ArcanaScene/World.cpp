#include "World.h"

#include "CameraComponent.h"

namespace Arcana
{
	World::World() : _id("world"), _cameraActor(nullptr)
	{

	}
	World::World(const std::string& id) : _id(id), _cameraActor(nullptr)
	{

	}
	World::~World()
	{
		for (auto i = _actors.createIterator(); i; i++)
		{
			AE_RELEASE(*i);
		}

		_actors.empty();

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

			actor->destroyed();

			for (uint32 i = 0; i < actor->getNumChildren(); i++)
			{
				Actor* child = actor->getChild(i);
				if (child)
				{
					child->destroy();
				}
			}

			//set owner null
			//remove actor

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
		if (actor->hasActiveComponent<CameraComponent>())
		{
			_cameraActor = actor;
		}

		actor->reference();
		_actors.add(actor);
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
		LOG(Info, CoreEngine, "Render Actors Called");
		LOG(Info, CoreEngine, "Queuing Actor meshes");

		Matrix4f view = Matrix4f::IDENTITY;
		Matrix4f proj = Matrix4f::IDENTITY;
		Vector3d eyePosition = Vector3d::zero();

		if (_cameraActor)
		{
			_cameraActor->getCameraView(view, proj, eyePosition);
		}

		for (auto i = _actors.createConstIterator(); i; i++)
		{
			Actor* actor = *i;
			
			if (!_cameraActor)
			{
				actor->getCameraView(view, proj, eyePosition);
			}

			actor->render(_renderer, view, proj, eyePosition);
		}


		_renderer.render();
	}


	ObjectRenderer& World::getRenderer()
	{
		return _renderer;
	}

	const GlobalObjectID& World::getId() const
	{
		return _id;
	}
}