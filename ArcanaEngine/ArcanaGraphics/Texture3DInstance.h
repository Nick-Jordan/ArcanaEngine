#ifndef TEXTURE_3D_INSTANCE_H_
#define TEXTURE_3D_INSTANCE_H_

#include "GraphicsDefines.h"

#include "TextureInstance.h"

namespace Arcana
{

	class ARCANA_GRAPHICS_API Texture3DInstance : public TextureInstance
	{
	public:

		Texture3DInstance(Texture::Format format, uint32 width, uint32 height, uint32 depth, Texture::InternalFormat iformat, Texture::PixelType pixelType,
			const void* pixels, const Texture::Parameters& parameters);

		virtual ~Texture3DInstance();

		virtual uint32 getWidth() const override;

		virtual uint32 getHeight() const override;

		virtual uint32 getDepth() const override;

		void initialize(Texture::Format format, uint32 width, uint32 height, uint32 depth, Texture::InternalFormat iformat, Texture::PixelType pixelType,
			const void* pixels, const Texture::Parameters& parameters);

	protected:

		uint32 _width;
		uint32 _height;
		uint32 _depth;
	};

}

#endif // !TEXTURE_1D_INSTANCE_H_

