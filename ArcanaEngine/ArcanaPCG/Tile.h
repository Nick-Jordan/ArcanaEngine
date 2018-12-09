#ifndef TILE_H_
#define TILE_H_

#include "PCGDefines.h"

#include "Object.h"
#include "Vector3.h"
#include "Task.h"
#include "TileStorage.h"
#include "Vector4.h"
#include "GlobalObjectID.h"

#include <functional>

namespace Arcana
{

	class ARCANA_PCG_API Tile : public Object
	{
		friend class TileProducer;

	public:

		class ARCANA_PCG_API Id
		{
		public:

			const int32 producerId;
			const int32 level;
			const int32 tx;
			const int32 ty;

			Id() : producerId(0), level(0), tx(0), ty(0) {
				_id = GlobalObjectID(std::to_string(producerId) + std::to_string(level) + std::to_string(tx) + std::to_string(ty));
			};
			Id(int32 producerId, int32 level, int32 tx, int32 ty) : producerId(producerId), level(level), tx(tx), ty(ty) {
				_id = GlobalObjectID(std::to_string(producerId) + std::to_string(level) + std::to_string(tx) + std::to_string(ty));
			};
			~Id() {};

			bool operator<(const Id& rhs) const
			{
				return _id.getId() < rhs._id.getId();
			};

			bool operator==(const Id& rhs) const
			{
				return producerId == rhs.producerId && level == rhs.level && tx == rhs.tx && ty == rhs.ty;
			}

		private:

			GlobalObjectID _id;

		};

		Tile(int32 producerId, int32 level, int32 tx, int32 ty, Task* createTask, TileStorage::Slot* data);

		~Tile();

		Id getId() const;

		int32 getProducerId() const;

		int32 getLevel() const;

		int32 getX() const;

		int32 getY() const;

		TileStorage::Slot* getData(bool check = true);

		Task* getCreateTask() const;

		void use();

		void unuse();

		uint32 getUsers() const;

	private:

		Id _id;
		Task* _createTask;
		TileStorage::Slot* _data;

		uint32 _users;
	};
}

#endif // !TILE_H_

