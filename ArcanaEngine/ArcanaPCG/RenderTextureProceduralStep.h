#ifndef RENDER_TEXTURE_PROCEDURAL_STEP_H_
#define RENDER_TEXTURE_PROCEDURAL_STEP_H_

#include "PCGDefines.h"

#include "ProceduralStep.h"
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

	class ARCANA_PCG_API RenderTextureProceduralStep : public ProceduralStep<Texture, TextureProceduralParameters>
	{
	public:

		virtual void perform(
			const TextureProceduralParameters& params,
			ProceduralStep<Texture, TextureProceduralParameters>* previous,
			Texture** object) override;
	};
}

#endif // !RENDER_TEXTURE_PROCEDURAL_STEP_H_

