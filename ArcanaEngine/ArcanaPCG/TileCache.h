#ifndef TILE_CACHE_H_
#define TILE_CACHE_H_

#include "PCGDefines.h"

#include <map>
#include <vector>
#include <string>
#include <list>
#include "Object.h"
#include "Scheduler.h"
#include "TileStorage.h"
#include "Tile.h"

namespace Arcana
{
	class ARCANA_PCG_API TileProducer;

	class ARCANA_PCG_API TileCache : public Object
	{
		friend class TileProducer;
		friend class CreateTile;

	public:

		typedef std::map<Tile::Id, Tile*> Cache;


		TileCache(std::string name, TileStorage* storage, Scheduler* scheduler);

		virtual ~TileCache();

		TileStorage* getStorage();

		Scheduler* getScheduler();

		Tile* findTile(int32 producerId, int32 level, int32 tx, int32 ty, bool checkUnused = false, bool use = false);

		Tile* getTile(int32 producerId, int32 level, int32 tx, int32 ty);

		void setTileUnused(Tile* tile);
	
	private:

		std::string _name;
		int32 _nextProducerId;
		std::map<int32, TileProducer*> _producers;
		TileStorage* _storage;
		Scheduler* _scheduler;

		Cache _usedTiles;
		Cache _unusedTiles;
		
		Mutex _mutex;
	};
}

#endif // !TILE_CACHE_H_
