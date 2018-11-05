#include "Texture2DArrayInstance.h"

namespace Arcana
{
	Texture2DArrayInstance::Texture2DArrayInstance(Texture::Format format, uint32 width, uint32 height, uint32 layers, Texture::InternalFormat iformat, Texture::PixelType pixelType,
		const void* pixels, const Texture::Parameters& parameters)
	{
		initialize(format, width, height, layers, iformat, pixelType, pixels, parameters);
	}

	Texture2DArrayInstance::~Texture2DArrayInstance()
	{

	}

	uint32 Texture2DArrayInstance::getWidth() const
	{
		return _width;
	}

	uint32 Texture2DArrayInstance::getHeight() const
	{
		return _height;
	}

	uint32 Texture2DArrayInstance::getLayers() const
	{
		return _layers;
	}

	void Texture2DArrayInstance::initialize(Texture::Format format, uint32 width, uint32 height, uint32 layers, Texture::InternalFormat iformat, Texture::PixelType pixelType,
		const void* pixels, const Texture::Parameters& parameters)
	{
		_width = width;
		_height = height;
		_layers = layers;

		TextureInstance::initialize(Texture::Type::Texture2DArray, iformat, parameters);

		glBindTexture(Texture::Type::Texture2DArray, _id);

		//if (isCompressed() && s.compressedSize() > 0) {
		//	glCompressedTexImage1D(texture->_type, 0, iformat, width, 0, s.compressedSize(), pixels.data(0));
		//}
		//else
		{
			//s.set();
			glTexImage3D(Texture::Type::Texture2DArray, 0, iformat, width, height, layers, 0, format, pixelType, pixels);
			//s.unset();
		}
		//pixels.unbind(BufferType::PixelUnpackBuffer());
	}
}
