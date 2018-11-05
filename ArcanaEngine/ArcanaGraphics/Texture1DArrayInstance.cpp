#include "Texture1DArrayInstance.h"

namespace Arcana
{
	Texture1DArrayInstance::Texture1DArrayInstance(Texture::Format format, uint32 width, uint32 layers, Texture::InternalFormat iformat, Texture::PixelType pixelType,
		const void* pixels, const Texture::Parameters& parameters)
	{
		initialize(format, width, layers, iformat, pixelType, pixels, parameters);
	}

	Texture1DArrayInstance::~Texture1DArrayInstance()
	{

	}

	uint32 Texture1DArrayInstance::getWidth() const
	{
		return _width;
	}

	uint32 Texture1DArrayInstance::getLayers() const
	{
		return _layers;
	}

	void Texture1DArrayInstance::initialize(Texture::Format format, uint32 width, uint32 layers, Texture::InternalFormat iformat, Texture::PixelType pixelType,
		const void* pixels, const Texture::Parameters& parameters)
	{
		_width = width;
		_layers = layers;

		TextureInstance::initialize(Texture::Type::Texture1DArray, iformat, parameters);

		glBindTexture(Texture::Type::Texture1DArray, _id);

		//if (isCompressed() && s.compressedSize() > 0) {
		//	glCompressedTexImage1D(texture->_type, 0, iformat, width, 0, s.compressedSize(), pixels.data(0));
		//}
		//else
		{
			//s.set();
			glTexImage2D(Texture::Type::Texture1DArray, 0, iformat, width, layers, 0, format, pixelType, pixels);
			//s.unset();
		}
		//pixels.unbind(BufferType::PixelUnpackBuffer());
	}
}
