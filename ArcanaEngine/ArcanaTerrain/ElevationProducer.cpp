#include "ElevationProducer.h"

#include "ArcanaLog.h"

#include "Sleep.h"
#include <thread>
#include <chrono>

namespace Arcana
{
	ElevationProducer::ElevationProducer(TileCache* cache, int32 gridMeshSize, bool flipDiagonals)
		: TileProducer("ElevationProducer", cache), _gridMeshSize(gridMeshSize), _flipDiagonals(flipDiagonals)
	{

	}

	ElevationProducer::ElevationProducer()
	{
	}


	ElevationProducer::~ElevationProducer()
	{
	}

	int32 ElevationProducer::getBorder() const
	{
		return 2;
	}

	void ElevationProducer::runCreateTile(int32 level, int32 tx, int32 ty, TileStorage::Slot* data)
	{
		if (!data)
			return;

		uint32 tileSize = data->getOwner()->getTileSize();

		TextureTileStorage::TextureSlot* textureData = dynamic_cast<TextureTileStorage::TextureSlot*>(data);

		/*TextureTileStorage::TextureSlot* parentData = nullptr;
		if (level > 0)
		{
			Tile* t = findTile(level - 1, tx / 2, ty / 2);
			if (t)
			{
				parentData = dynamic_cast<TextureTileStorage::TextureSlot*>(t->getData());
			}
		}*/

		if (textureData)
		{
			//auto start = std::chrono::high_resolution_clock::now();

			//std::this_thread::sleep_for(std::chrono::milliseconds(100));

			double l = 1000.0 * 2.0 / (double)pow(2, level);
			double ox = -1000.0 + l * (double)tx;
			double oy = -1000.0 + l * (double)ty;
			


			textureData->update(0, 0, tileSize, tileSize, Texture::Red, Texture::Float, data);
		}
	}
}