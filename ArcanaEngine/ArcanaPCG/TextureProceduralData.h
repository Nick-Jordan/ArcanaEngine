#ifndef TEXTURE_PROCEDURAL_DATA_H_
#define TEXTURE_PROCEDURAL_DATA_H_

#include "PCGDefines.h"

#include "ProceduralData.h"
#include "Texture.h"
#include "Shader.h"
#include "Framebuffer.h"

namespace Arcana
{
	class ARCANA_PCG_API TextureProceduralData : public ProceduralData
	{
	public:

		TextureProceduralData(const Shader& shader, Texture::Format format, uint32 width, uint32 height,
			Texture::InternalFormat iformat, Texture::PixelType pixelType, 
			const Texture::Parameters& parameters = Texture::Parameters(), bool generateMipmap = false);

		virtual ~TextureProceduralData();

		virtual void generate(const ProceduralParameters& params, const Seed& seed) override;

		Texture* getTexture();

	private:

		Shader _shader;
		Texture* _texture;
		Framebuffer* _framebuffer;
	};
}

#endif // !TEXTURE_PROCEDURAL_DATA_H_

