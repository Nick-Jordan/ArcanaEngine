#include "TextureTileStorage.h"

#include "Lock.h"

namespace Arcana
{
	TextureTileStorage::TextureSlot::TextureSlot(TileStorage *owner, int32 index, Texture* texture, int32 l) :
		Slot(owner), texture(texture), l(l), index(index)
	{
	}

	TextureTileStorage::TextureSlot::~TextureSlot()
	{
		AE_RELEASE(texture);
	}

	void TextureTileStorage::TextureSlot::update(int32 x, int32 y, int32 w, int32 h, Texture::Format f, Texture::PixelType t, const void* pixels)
	{
		texture->update2DArray(0, x, y, l, w, h, 1, f, t, pixels);
	}

	TextureTileStorage::TextureTileStorage() : TileStorage()
	{
	}

	TextureTileStorage::TextureTileStorage(int32 tileSize, int32 nTiles, Texture::Format f,
		Texture::InternalFormat internalf, Texture::PixelType t,
		const Texture::Parameters& params) :
		TileStorage(tileSize, nTiles)
	{
		init(tileSize, nTiles, f, internalf, t, params);
	}

	void TextureTileStorage::init(int32 tileSize, int32 nTiles, Texture::Format f,
		Texture::InternalFormat internalf, Texture::PixelType t,
		const Texture::Parameters &params)
	{
		TileStorage::init(tileSize, nTiles);

		GLint l;
		glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS, &l);
		int32 maxLayers = l;//Texture2DArray::getMaxLayers();
		int32 nTextures = nTiles / maxLayers + (nTiles % maxLayers == 0 ? 0 : 1);

		for (int32 i = 0; i < nTextures; ++i) 
		{
			int32 nLayers = i == nTextures - 1 && nTiles % maxLayers != 0 ? nTiles % maxLayers : maxLayers;
			Texture* texture = Texture::create2DArray(f, tileSize, tileSize, nLayers, internalf, t,
				nullptr, Texture::Parameters());

			_textures.push_back(texture);

			for (int j = 0; j < nLayers; ++j) 
			{
				_freeSlots.push_back(new TextureSlot(this, i, _textures[i], j));
			}
		}
	}

	TextureTileStorage::~TextureTileStorage()
	{

	}

	uint32 TextureTileStorage::getTextureCount()
	{
		return (uint32)_textures.size();
	}

	Texture* TextureTileStorage::getTexture(int32 index)
	{
		return _textures[index];
	}
}
