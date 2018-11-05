#include "TextureRectangleInstance.h"

namespace Arcana
{
	TextureRectangleInstance::TextureRectangleInstance(Texture::Format format, uint32 width, uint32 height, Texture::InternalFormat iformat, Texture::PixelType pixelType,
		const void* pixels, const Texture::Parameters& parameters)
	{
		initialize(format, width, height, iformat, pixelType, pixels, parameters);
	}

	TextureRectangleInstance::~TextureRectangleInstance()
	{

	}

	uint32 TextureRectangleInstance::getWidth() const
	{
		return _width;
	}

	uint32 TextureRectangleInstance::getHeight() const
	{
		return _height;
	}

	void TextureRectangleInstance::initialize(Texture::Format format, uint32 width, uint32 height, Texture::InternalFormat iformat, Texture::PixelType pixelType,
		const void* pixels, const Texture::Parameters& parameters)
	{
		_width = width;
		_height = height;

		TextureInstance::initialize(Texture::Type::TextureRectangle, iformat, parameters);

		glBindTexture(Texture::Type::TextureRectangle, _id);

		//if (isCompressed() && s.compressedSize() > 0) {
		//	glCompressedTexImage1D(texture->_type, 0, iformat, width, 0, s.compressedSize(), pixels.data(0));
		//}
		//else
		{
			//s.set();
			glTexImage2D(Texture::Type::TextureRectangle, 0, iformat, width, height, 0, format, pixelType, pixels);
			//s.unset();
		}
		//pixels.unbind(BufferType::PixelUnpackBuffer());
	}
}
