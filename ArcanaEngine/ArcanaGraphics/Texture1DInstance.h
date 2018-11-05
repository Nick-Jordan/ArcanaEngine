#ifndef TEXTURE_1D_INSTANCE_H_
#define TEXTURE_1D_INSTANCE_H_

#include "GraphicsDefines.h"

#include "TextureInstance.h"

namespace Arcana
{

	class ARCANA_GRAPHICS_API Texture1DInstance : public TextureInstance
	{
	public:

		Texture1DInstance(Texture::Format format, uint32 width, Texture::InternalFormat iformat, Texture::PixelType pixelType,
			const void* pixels, const Texture::Parameters& parameters);

		virtual ~Texture1DInstance();

		virtual uint32 getWidth() const override;

		void initialize(Texture::Format format, uint32 width, Texture::InternalFormat iformat, Texture::PixelType pixelType,
			const void* pixels, const Texture::Parameters& parameters);

	protected:

		uint32 _width;
	};

}

#endif // !TEXTURE_1D_INSTANCE_H_

