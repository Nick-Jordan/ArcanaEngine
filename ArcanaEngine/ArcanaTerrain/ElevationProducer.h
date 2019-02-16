#ifndef ELEVATION_PRODUCER_H_
#define ELEVATION_PRODUCER_H_

#include "TerrainDefines.h"

#include "TileProducer.h"
#include "TextureTileStorage.h"

namespace Arcana
{

	class ARCANA_TERRAIN_API ElevationProducer : public TileProducer
	{
	public:

		ElevationProducer(TileCache* cache, int32 gridMeshSize, bool flipDiagonals = false);

		virtual ~ElevationProducer();

		virtual int32 getBorder() const override;

	protected:

		ElevationProducer();

		virtual void runCreateTile(int32 level, int32 tx, int32 ty, TileStorage::Slot *data) override;
	//test
	public:
		static Vector2d sampleSphericalMap(double x, double y, double z);
	
		static void generate_terrain(TextureTileStorage::TextureSlot* data, uint32 tileSize, Vector2d coord0, Vector2d coord1, int level, int tx, int ty);

	private:

		int32 _face;
		int32 _gridMeshSize;
		bool _flipDiagonals;
	};

}

#endif // !ELEVATION_PRODUCER_H_

