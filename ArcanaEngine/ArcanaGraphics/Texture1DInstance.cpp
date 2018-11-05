#include "Texture1DInstance.h"

namespace Arcana
{
	Texture1DInstance::Texture1DInstance(Texture::Format format, uint32 width, Texture::InternalFormat iformat, Texture::PixelType pixelType,
		const void* pixels, const Texture::Parameters& parameters)
	{
		initialize(format, width, iformat, pixelType, pixels, parameters);
	}

	Texture1DInstance::~Texture1DInstance()
	{

	}

	uint32 Texture1DInstance::getWidth() const
	{
		return _width;
	}

	void Texture1DInstance::initialize(Texture::Format format, uint32 width, Texture::InternalFormat iformat, Texture::PixelType pixelType,
		const void* pixels, const Texture::Parameters& parameters)
	{
		_width = width;

		TextureInstance::initialize(Texture::Type::Texture1D, iformat, parameters);

		glBindTexture(Texture::Type::Texture1D, _id);

		//if (isCompressed() && s.compressedSize() > 0) {
		//	glCompressedTexImage1D(texture->_type, 0, iformat, width, 0, s.compressedSize(), pixels.data(0));
		//}
		//else
		{
			//s.set();
			glTexImage1D(Texture::Type::Texture1D, 0, iformat, width, 0, format, pixelType, pixels);
			//s.unset();
		}
		//pixels.unbind(BufferType::PixelUnpackBuffer());
	}
}
