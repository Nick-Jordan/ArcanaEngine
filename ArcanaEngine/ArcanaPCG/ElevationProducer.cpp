#include "ElevationProducer.h"

#include "TextureTileStorage.h"
#include "Noise.h"

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
			float* pixelData = new float[tileSize * tileSize * 4];

			for (int32 i = 0; i < tileSize * tileSize; i++)
			{
				pixelData[i * 4 + 0] = 0.0f;
				pixelData[i * 4 + 1] = 2.0f;
				pixelData[i * 4 + 2] = 0.0f;
				pixelData[i * 4 + 3] = 0.0f;
			}

			textureData->update(0, 0, tileSize, tileSize, Texture::RGBA, Texture::Float, pixelData);
		}
	}
}