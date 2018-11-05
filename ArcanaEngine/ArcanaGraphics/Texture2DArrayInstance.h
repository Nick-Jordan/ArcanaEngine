#ifndef TEXTURE_2D_ARRAY_INSTANCE_H_
#define TEXTURE_2D_ARRAY_INSTANCE_H_

#include "GraphicsDefines.h"

#include "TextureInstance.h"

namespace Arcana
{

	class ARCANA_GRAPHICS_API Texture2DArrayInstance : public TextureInstance
	{
	public:

		Texture2DArrayInstance(Texture::Format format, uint32 width, uint32 height, uint32 layers, Texture::InternalFormat iformat, Texture::PixelType pixelType,
			const void* pixels, const Texture::Parameters& parameters);

		virtual ~Texture2DArrayInstance();

		virtual uint32 getWidth() const override;

		virtual uint32 getHeight() const override;

		virtual uint32 getLayers() const override;

		void initialize(Texture::Format format, uint32 width, uint32 height, uint32 layers, Texture::InternalFormat iformat, Texture::PixelType pixelType,
			const void* pixels, const Texture::Parameters& parameters);

	protected:

		uint32 _width;
		uint32 _height;
		uint32 _layers;
	};

}

#endif // !TEXTURE_1D_INSTANCE_H_

