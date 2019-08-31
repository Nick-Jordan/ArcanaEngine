#ifndef RENDER_TEXTURE_GENERATOR_H_
#define RENDER_TEXTURE_GENERATOR_H_

#include "PCGDefines.h"

#include "ProceduralGenerator.h"
#include "Texture.h"
#include "Framebuffer.h"
#include "Shader.h"

namespace Arcana
{
	struct TextureProceduralParameters
	{
		std::string VertexShader;
		std::string FragmentShader;
		Texture::Format Format;
		uint32 Width;
		uint32 Height;
		Texture::InternalFormat InternalFormat;
		Texture::PixelType PixelType;

		virtual void setUniforms(Shader& shader) const {};
	};
	class ARCANA_PCG_API RenderTextureGenerator : public ProceduralGenerator<Texture, TextureProceduralParameters>
	{
	public:

		virtual void generateObject(const TextureProceduralParameters& params, Texture** object) override;
	};
}

#endif // !RENDER_TEXTURE_GENERATOR_H_