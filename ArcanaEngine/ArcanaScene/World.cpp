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
			//check world

			//check pending destruction

			//actor->destroyed()

			//destroy children

			//set owner null

			//remove actor

			//unregister components

			//mark actor and components pending destroy

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

		if (_cameraActor)
		{
			_cameraActor->getCameraMatrices(view, proj);
		}

		for (auto i = _actors.createConstIterator(); i; i++)
		{
			Actor* actor = *i;
			
			if (!_cameraActor)
			{
				actor->getCameraMatrices(view, proj);
			}

			actor->render(_renderer, view, proj);
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