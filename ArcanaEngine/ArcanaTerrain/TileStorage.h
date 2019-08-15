#ifndef TILE_STORAGE_H_
#define TILE_STORAGE_H_

#include "TerrainDefines.h"

#include "Object.h"
#include "Mutex.h"
#include "Task.h"
#include "Vector3.h"
#include "KeyValuePair.h"

namespace Arcana
{
	class ARCANA_TERRAIN_API TileStorage : public Object
	{
	public:

		class ARCANA_TERRAIN_API Slot
		{
		public:

			KeyValuePair<int32, KeyValuePair<int32, KeyValuePair<int32, int32> > > id;

			Task* task;

			Slot(TileStorage *owner);

			virtual ~Slot();

			TileStorage *getOwner();

			void lock();

			void unlock();

		private:

			TileStorage* _owner;

			Mutex _mutex;
		};

		TileStorage(uint32 tileSize, uint32 capacity);

		virtual ~TileStorage();

		Slot* createSlot();

		void deleteSlot(Slot *t);

		uint32 getTileSize();

		uint32 getCapacity();

		uint32 getFreeSlots();

	protected:

		TileStorage();

		void init(uint32 tileSize, uint32 capacity);

	protected:

		uint32 _tileSize;

		uint32 _capacity;

		std::vector<Slot*> _freeSlots;

	};
}

#endif // !TILE_STORAGE_H_