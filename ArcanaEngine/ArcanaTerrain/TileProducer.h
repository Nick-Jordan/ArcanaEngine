#ifndef TILE_PRODUCER_H_
#define TILE_PRODUCER_H_

#include "TerrainDefines.h"

#include "Object.h"
#include "TileCache.h"
#include "TileStorage.h"
#include "Vector4.h"
#include "Vector2.h"
#include "Task.h"

namespace Arcana
{
	class ARCANA_TERRAIN_API TileProducer : public Object
	{
		friend class CreateTile;
		friend class TileCache;

	public:

		TileProducer(const std::string& type, TileCache* cache);

		virtual ~TileProducer();

		double getRootQuadSize() const;

		void setRootQuadSize(double size);

		virtual int32 getBorder() const;

		int32 getId() const;

		virtual TileCache* getCache();

		virtual bool hasTile(int32 level, int32 tx, int32 ty);

		bool hasChildren(int32 level, int32 tx, int32 ty);

		virtual Tile* findTile(int32 level, int32 tx, int32 ty, bool checkUnused = false, bool done = false);

		virtual Tile* getTile(int32 level, int32 tx, int32 ty);

		virtual void setTileUnused(Tile* tile);

		virtual void update();

		Task* createTileTask(int32 level, int32 tx, int32 ty, TileStorage::Slot* data);

	protected:

		TileProducer();

		void init(TileCache* cache);

		virtual void runCreateTile(int32 level, int32 tx, int32 ty, TileStorage::Slot* data);

	private:

		double _rootQuadSize;

		int32 _id;

		TileCache* _cache;

		Mutex _mutex;
	};

	class ARCANA_TERRAIN_API CreateTile : public Task
	{
	public:

		TileProducer *owner;

		int32 level;
		int32 tx;
		int32 ty;

		TileStorage::Slot *data;

		CreateTile(TileProducer *owner, int32 level, int32 tx, int32 ty, TileStorage::Slot *data);

		virtual ~CreateTile();

		virtual void run() override;

		virtual void done() override;
	};
}

#endif // !TILE_PRODUCER_H_

