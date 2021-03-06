#ifndef TEXTURE_TILE_STORAGE_H_
#define TEXTURE_TILE_STORAGE_H_

#include "TerrainDefines.h"
#include "TileStorage.h"

#include "Texture.h"

namespace Arcana
{
	class ARCANA_TERRAIN_API TextureTileStorage : public TileStorage
	{
	public:

		class ARCANA_TERRAIN_API TextureSlot : public Slot
		{
		public:

			Texture* texture;

			const int32 l;

			TextureSlot(TileStorage *owner, int32 index, Texture* texture, int32 l);

			virtual ~TextureSlot();

			inline int32 getIndex()
			{
				return index;
			}

			inline int32 getWidth()
			{
				return texture->getWidth();
			}

			inline int getHeight()
			{
				return texture->getHeight();
			}

			virtual void update(int32 x, int32 y, int32 w, int32 h, Texture::Format f, Texture::PixelType t, const void* pixels);

		private:

			int32 index;

			friend class TextureTileStorage;
		};


		TextureTileStorage(int32 tileSize, int32 nTiles, Texture::Format f,
			Texture::InternalFormat internalf, Texture::PixelType t,
			const Texture::Parameters& params);

		virtual ~TextureTileStorage();

		uint32 getTextureCount();

		Texture* getTexture(int32 index);

	protected:

		TextureTileStorage();

		void init(int32 tileSize, int32 nTiles, Texture::Format f,
			Texture::InternalFormat internalf, Texture::PixelType t,
			const Texture::Parameters& params);

	private:

		std::vector<Texture*> _textures;
	};

}

#endif // !TEXTURE_TILE_STORAGE_H_
