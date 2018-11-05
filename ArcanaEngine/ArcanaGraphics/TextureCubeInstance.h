#ifndef TEXTURE_CUBE_INSTANCE_H_
#define TEXTURE_CUBE_INSTANCE_H_

#include "GraphicsDefines.h"

#include "TextureInstance.h"

namespace Arcana
{

	class ARCANA_GRAPHICS_API TextureCubeInstance : public TextureInstance
	{
	public:

		TextureCubeInstance(Texture::Format format, uint32 width, uint32 height, Texture::InternalFormat iformat, Texture::PixelType pixelType,
			void* pixels[6], const Texture::Parameters& parameters);

		virtual ~TextureCubeInstance();

		virtual uint32 getWidth() const override;

		virtual uint32 getHeight() const override;

		void initialize(Texture::Format format, uint32 width, uint32 height, Texture::InternalFormat iformat, Texture::PixelType pixelType,
			void* pixels[6], const Texture::Parameters& parameters);

	protected:

		uint32 _width;
		uint32 _height;
	};

}

#endif // !TEXTURE_1D_INSTANCE_H_

