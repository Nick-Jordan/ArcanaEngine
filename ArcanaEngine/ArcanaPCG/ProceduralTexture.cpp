#include "ProceduralTexture.h"

namespace Arcana
{

	ProceduralTexture::ProceduralTexture(const Shader& shader, Texture::Format format, uint32 width, uint32 height,
		Texture::InternalFormat iformat, Texture::PixelType pixelType, const Texture::Parameters& parameters, bool generateMipmap) 
		: ProceduralObject("ProceduralTexture", "procedural_texture")
	{
		_textureData = new TextureProceduralData(shader, format, width, height, iformat, pixelType, parameters, generateMipmap);
		_textureData->reference();
		registerData(_textureData);
	}

	ProceduralTexture::~ProceduralTexture()
	{
		AE_RELEASE(_textureData);
	}

	Texture* ProceduralTexture::get()
	{
		if (_textureData)
		{
			return _textureData->getTexture();
		}
	}
}
