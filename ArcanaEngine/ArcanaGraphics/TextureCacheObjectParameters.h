#ifndef TEXTURE_CACHE_OBJECT_PARAMETERS_H_
#define TEXTURE_CACHE_OBJECT_PARAMETERS_H_

#include "GraphicsDefines.h"

#include "Texture.h"

namespace Arcana
{
	class ARCANA_GRAPHICS_API TextureCacheObjectParameters
	{
	public:
	
		Texture::Format type;
		uint32 width;
		uint32 height;
		uint32 depth;
		uint32 layers;
		Texture::InternalFormat internalFormat;
		Texture::PixelType pixelType;
		Texture::Parameters parameters;
		bool generateMipmap;
	};
}

#endif // !TEXTURE_CACHE_OBJECT_PARAMETERS_H_