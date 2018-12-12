#include "TileProducer.h"

#include "TextureTileStorage.h"

namespace Arcana
{
	TileProducer::TileProducer(const std::string& type, TileCache* cache) : Object(type)
	{
		init(cache);
	}

	TileProducer::TileProducer()
	{
	}

	TileProducer::~TileProducer()
	{
		std::map<int, TileProducer*>::iterator i = _cache->_producers.find(_id);
		AE_ASSERT(i != _cache->_producers.end());
		_cache->_producers.erase(i);

		AE_RELEASE(_cache);
	}

	void TileProducer::init(TileCache* cache)
	{
		_cache = cache;
		_cache->reference();
		_rootQuadSize = 0.0;
		_id = cache->_nextProducerId++;
		_cache->_producers.insert(std::make_pair(_id, this));
	}

	void TileProducer::runCreateTile(int32 level, int32 tx, int32 ty, TileStorage::Slot* data)
	{

	}

	double TileProducer::getRootQuadSize() const
	{
		return _rootQuadSize;
	}

	void TileProducer::setRootQuadSize(double size)
	{
		_rootQuadSize = size;
	}

	int32 TileProducer::getBorder() const
	{
		return 0;
	}

	int32 TileProducer::getId() const
	{
		return _id;
	}

	TileCache* TileProducer::getCache()
	{
		return _cache;
	}

	bool TileProducer::hasTile(int32 level, int32 tx, int32 ty)
	{
		return true;
	}

	bool TileProducer::hasChildren(int32 level, int32 tx, int32 ty)
	{
		return hasTile(level + 1, 2 * tx, 2 * ty);
	}

	Tile* TileProducer::findTile(int32 level, int32 tx, int32 ty, bool checkUnused, bool done)
	{
		Tile* tile = _cache->findTile(_id, level, tx, ty, checkUnused);
		if (done && tile != nullptr && !tile->getCreateTask()->isDone()) 
		{
			tile = nullptr;
		}
		return tile;
	}

	Tile* TileProducer::getTile(int32 level, int32 tx, int32 ty)
	{
		Tile* tile = _cache->getTile(_id, level, tx, ty);

		return tile;
	}

	void TileProducer::setTileUnused(Tile* tile)
	{
		_cache->setTileUnused(tile);
	}

	void TileProducer::update()
	{

	}

	Task* TileProducer::createTileTask(int32 level, int32 tx, int32 ty, TileStorage::Slot *data)
	{
		CreateTile* t = new CreateTile(this, level, tx, ty, data);

		return t;
	}


	CreateTile::CreateTile(TileProducer *owner, int32 level, int32 tx, int32 ty, TileStorage::Slot *data) :
		Task("CreateTile"), owner(owner), level(level), tx(tx), ty(ty), data(data)
	{
		data->lock();
		data->task = this;
		data->unlock();
	}

	CreateTile::~CreateTile()
	{
	}

	void CreateTile::run()
	{
		data->lock();
		if (data->task == this) 
		{	
			owner->runCreateTile(level, tx, ty, data);
			data->id = std::make_pair(owner->getId(), std::make_pair(level, std::make_pair(tx, ty)));
			_done = true;
		}
		data->unlock();
	}
}
