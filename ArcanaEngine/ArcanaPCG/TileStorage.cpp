#include "TileStorage.h"

namespace Arcana
{
	TileStorage::TileStorage(uint32 tileSize, uint32 capacity) : Object("TileStorage")
	{
		init(tileSize, capacity);
	}

	TileStorage::TileStorage() : Object("TileStorage"), _tileSize(0), _capacity(0)
	{

	}


	TileStorage::~TileStorage()
	{
		std::vector<Slot*>::iterator i;
		for (i = i = _freeSlots.begin(); i != _freeSlots.end(); i++)
		{
			AE_DELETE(*i);
		}
		_freeSlots.clear();
	}

	TileStorage::Slot* TileStorage::createSlot()
	{
		std::vector<Slot*>::iterator i = _freeSlots.begin();
		if (i != _freeSlots.end())
		{
			Slot* s = *i;
			_freeSlots.erase(i);
			return s;
		}
		
		return nullptr;
	}

	void TileStorage::deleteSlot(Slot *t)
	{
		_freeSlots.push_back(t);
	}

	uint32 TileStorage::getTileSize()
	{
		return _tileSize;
	}

	uint32 TileStorage::getCapacity()
	{
		return _capacity;
	}

	uint32 TileStorage::getFreeSlots()
	{
		return (uint32)_freeSlots.size();
	}

	void TileStorage::init(uint32 tileSize, uint32 capacity)
	{
		_tileSize = tileSize;
		_capacity = capacity;
	}


	TileStorage::Slot::Slot(TileStorage* owner) : _owner(owner), task(task)
	{

	}

	TileStorage::Slot::~Slot()
	{

	}

	TileStorage* TileStorage::Slot::getOwner()
	{
		return _owner;
	}

	void TileStorage::Slot::lock()
	{
		_mutex.lock();
	}

	void TileStorage::Slot::unlock()
	{
		_mutex.unlock();
	}

}
