#ifndef TEXTURE_UTILS_H_
#define TEXTURE_UTILS_H_

#include "GraphicsDefines.h"

#include "Texture.h"

namespace Arcana
{
	class ARCANA_GRAPHICS_API TextureUtils
	{
	public:

		//temp
		static Texture* createImageCubeTexture(std::string files[6], bool fixAlignment = false);
	};
}

#endif // !TEXTURE_UTILS_H_
