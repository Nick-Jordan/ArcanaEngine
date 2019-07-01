#include "Texture2DInstance.h"

namespace Arcana
{
	Texture2DInstance::Texture2DInstance(Texture::Format format, uint32 width, uint32 height, Texture::InternalFormat iformat, Texture::PixelType pixelType,
		const void* pixels, const Texture::Parameters& parameters)
	{
		initialize(format, width, height, iformat, pixelType, pixels, parameters);
	}

	Texture2DInstance::~Texture2DInstance()
	{

	}

	uint32 Texture2DInstance::getWidth() const
	{
		return _width;
	}

	uint32 Texture2DInstance::getHeight() const
	{
		return _height;
	}

	void Texture2DInstance::initialize(Texture::Format format, uint32 width, uint32 height, Texture::InternalFormat iformat, Texture::PixelType pixelType,
		const void* pixels, const Texture::Parameters& parameters)
	{
		_width = width;
		_height = height;

		TextureInstance::initialize(Texture::Type::Texture2D, iformat, parameters);

		//if (isCompressed() && s.compressedSize() > 0) {
		//	glCompressedTexImage1D(texture->_type, 0, iformat, width, 0, s.compressedSize(), pixels.data(0));
		//}
		//else
		{
			//s.set();
			glTexImage2D(Texture::Type::Texture2D, 0, iformat, width, height, 0, format, pixelType, pixels);
			//s.unset();
		}
		//pixels.unbind(BufferType::PixelUnpackBuffer());
	}
}
