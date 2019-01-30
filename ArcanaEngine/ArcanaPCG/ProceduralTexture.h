#ifndef PROCEDURAL_TEXTURE_H_
#define PROCEDURAL_TEXTURE_H_

#include "PCGDefines.h"

#include "ProceduralObject.h"
#include "TextureProceduralData.h"

namespace Arcana
{
	class ARCANA_PCG_API ProceduralTexture : public ProceduralObject
	{
	public:

		ProceduralTexture(const Shader& shader, Texture::Format format, uint32 width, uint32 height,
			Texture::InternalFormat iformat, Texture::PixelType pixelType, 
			const Texture::Parameters& parameters = Texture::Parameters(), bool generateMipmap = false);

		virtual ~ProceduralTexture();

		Texture* get();

	private:

		TextureProceduralData* _textureData;
	};
}

#endif // !PROCEDURAL_TEXTURE_H_

