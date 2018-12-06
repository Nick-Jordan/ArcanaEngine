#include "Tile.h"

#include "ArcanaLog.h"

namespace Arcana
{
	Tile::Tile(int32 producerId, int32 level, int32 tx, int32 ty, Task* createTask, TileStorage::Slot* data)
		: _id(producerId, level, tx, ty), _createTask(createTask), _data(data), _users(0)
	{

	}

	Tile::~Tile()
	{
		AE_RELEASE(_createTask);
	}

	Tile::Id Tile::getId() const
	{
		return _id;
	}

	int32 Tile::getProducerId() const
	{
		return _id.producerId;
	}

	int32 Tile::getLevel() const
	{
		return _id.level;
	}

	int32 Tile::getX() const
	{
		return _id.tx;
	}

	int32 Tile::getY() const
	{
		return _id.ty;
	}

	TileStorage::Slot* Tile::getData(bool check)
	{
		if (_id.producerId != _data->id.first
			|| _id.level != _data->id.second.first
			|| _id.tx != _data->id.second.second.first
			|| _id.ty != _data->id.second.second.second)
		{
			return nullptr;
		}

		if (check)
		{
			if (_createTask->isDone())
			{
				return _data;
			}
			return nullptr;
		}

		return _data;
	}

	Task* Tile::getCreateTask() const
	{
		return _createTask;
	}

	void Tile::use()
	{
		_users++;
	}

	void Tile::unuse()
	{
		_users--;
	}

	uint32 Tile::getUsers() const
	{
		return _users;
	}
}
