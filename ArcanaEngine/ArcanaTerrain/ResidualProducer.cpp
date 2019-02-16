#include "ResidualProducer.h"

#include "Sleep.h"
#include <thread>
#include <chrono>

namespace Arcana
{
	ResidualProducer::ResidualProducer(TileCache* cache, std::string name, std::string ext)
		: TileProducer("ResidualProducer", cache), _name(name), _ext(ext)
	{

	}

	ResidualProducer::ResidualProducer()
	{
	}


	ResidualProducer::~ResidualProducer()
	{
	}

	int32 ResidualProducer::getBorder() const
	{
		return 2;
	}

	void ResidualProducer::runCreateTile(int32 level, int32 tx, int32 ty, TileStorage::Slot* data)
	{
		if (!data)
			return;

		uint32 tileSize = data->getOwner()->getTileSize();

		TextureTileStorage::TextureSlot* textureData = dynamic_cast<TextureTileStorage::TextureSlot*>(data);

		if (textureData)
		{
			if (_ext == "dat")
			{
				FileInputStream stream;
				if (stream.open("resources/tiles2/" + _name + "-" + std::to_string(level) + "-" + std::to_string(tx) + "-" + std::to_string(ty) + "." + _ext))
				{
					float* inputData = new float[stream.size()];
					stream.read(inputData, stream.size());

					textureData->update(0, 0, tileSize, tileSize, Texture::Red, Texture::Float, inputData);
				}
			}
			else
			{

				Image<uint8> image;

				if (image.init("resources/tiles2/" + _name + "-" + std::to_string(level) + "-" + std::to_string(tx) + "-" + std::to_string(ty) + "." + _ext))
				{
					image.flip(Image<uint8>::Vertical);

					uint8* inputData = new uint8[tileSize * tileSize * 4];
					memcpy(inputData, image.getPixelsPtr(), tileSize * tileSize * 4);

					textureData->update(0, 0, tileSize, tileSize, Texture::RGBA, Texture::UnsignedByte, inputData);
				}
			}
		}
	}
}