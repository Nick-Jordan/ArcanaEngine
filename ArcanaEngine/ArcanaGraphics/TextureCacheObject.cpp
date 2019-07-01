#include "TextureCacheObject.h"

#include "TextureCacheObjectParameters.h"

namespace Arcana
{
	TextureCacheObject::TextureCacheObject() : _parameters(TextureCacheObjectParameters())
	{
		//_parameters.format = Texture::RGBA;
		_parameters.width = 256;
		_parameters.height = 256;
		_parameters.depth = 0;
		_parameters.layers = 0;
		_parameters.internalFormat = Texture::RGBA8;
		_parameters.pixelType = Texture::UnsignedByte;
		_parameters.generateMipmap = false;
	}
		
	TextureCacheObject::~TextureCacheObject()
	{
	}
				
	const TextureCacheObjectParameters& TextureCacheObject::getParameters() const
	{
		return _parameters;
	}
		
	void TextureCacheObject::setParameters(const TextureCacheObjectParameters& parameters)
	{
		_parameters = parameters;
	}
}