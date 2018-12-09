#include "TileCache.h"

#include "TileProducer.h"

namespace Arcana
{
	TileCache::TileCache(std::string name, TileStorage* storage, Scheduler* scheduler) : Object("TileCache")
	{
		_nextProducerId = 0;
		_storage = storage;
		_scheduler = scheduler;
		_name = name;

		_storage->reference();
		_scheduler->reference();
	}

	TileCache::~TileCache()
	{
		AE_RELEASE(_storage);
		AE_RELEASE(_scheduler);
	}

	TileStorage* TileCache::getStorage()
	{
		return _storage;
	}

	Scheduler* TileCache::getScheduler()
	{
		return _scheduler;
	}

	Tile* TileCache::findTile(int32 producerId, int32 level, int32 tx, int32 ty, bool checkUnused, bool use)
	{
		Tile::Id id(producerId, level, tx, ty);

		Lock lock(_mutex);

		Cache::iterator i = _usedTiles.find(id);

		Tile* tile = nullptr;

		if (i != _usedTiles.end())
		{
			tile = (*i).second;
		}

		if (checkUnused)
		{
			i = _unusedTiles.find(id);

			if (i != _unusedTiles.end())
			{
				tile = (*i).second;

				if (tile && use)
				{
					_unusedTiles.erase(i);
					_usedTiles.emplace(tile->getId(), tile);
					tile->use();
				}
			}
		}

		return tile;
	}

	Tile* TileCache::getTile(int32 producerId, int32 level, int32 tx, int32 ty)
	{
		Tile* tile = findTile(producerId, level, tx, ty, true, true);

		Lock lock(_mutex);

		//LOGF(Debug, CoreEngine, "Tile: %p, %d, %d, %d, %d", tile, producerId, level, tx, ty);
		
		if (!tile)
		{
			TileStorage::Slot* data = _storage->createSlot();

			if (data == nullptr && _unusedTiles.size() > 0) 
			{
				std::map<Tile::Id, Tile*>::iterator i = _unusedTiles.begin();
				tile = (*i).second;
				data = tile->getData();
				_unusedTiles.erase(i);
				AE_DELETE(tile);
			}

			if (data == nullptr)
			{
				tile = nullptr;
			}
			else
			{
				Task* task = _producers[producerId]->createTileTask(level, tx, ty, data);

				if (task)
				{
					task->reference();
					tile = new Tile(producerId, level, tx, ty, task, data);
					tile->reference();
					_unusedTiles.emplace(tile->getId(), tile);
					_scheduler->schedule(task);
				}
			}
		}

		//LOGF(Debug, CoreEngine, "%s: tiles: %d used, %d reusable, total %d", _name.c_str(), _usedTiles.size(), _unusedTiles.size(), _storage->getCapacity());

		return tile;
	}

	void TileCache::setTileUnused(Tile* tile)
	{
		if (tile)
		{
			std::map<Tile::Id, Tile*>::iterator i = _usedTiles.find(tile->getId());

			if (i != _unusedTiles.end())
			{
				tile->unuse();

				if (tile->getUsers() == 0)
				{
					_usedTiles.erase(i);
					_unusedTiles.emplace(tile->getId(), tile);
				}
			}
		}
	}
}
