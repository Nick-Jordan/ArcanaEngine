#include "TextureCubeArrayInstance.h"

namespace Arcana
{
	TextureCubeArrayInstance::TextureCubeArrayInstance(Texture::Format format, uint32 width, uint32 height, uint32 layers, Texture::InternalFormat iformat, Texture::PixelType pixelType,
		const void* pixels, const Texture::Parameters& parameters)
	{
		initialize(format, width, height, layers, iformat, pixelType, pixels, parameters);
	}

	TextureCubeArrayInstance::~TextureCubeArrayInstance()
	{

	}

	uint32 TextureCubeArrayInstance::getWidth() const
	{
		return _width;
	}

	uint32 TextureCubeArrayInstance::getHeight() const
	{
		return _height;
	}

	uint32 TextureCubeArrayInstance::getLayers() const
	{
		return _layers;
	}

	void TextureCubeArrayInstance::initialize(Texture::Format format, uint32 width, uint32 height, uint32 layers, Texture::InternalFormat iformat, Texture::PixelType pixelType,
		const void* pixels, const Texture::Parameters& parameters)
	{
		_width = width;
		_height = height;
		_layers = layers;

		TextureInstance::initialize(Texture::Type::TextureCubeArray, iformat, parameters);

		glBindTexture(Texture::Type::TextureCubeArray, _id);

		//if (isCompressed() && s.compressedSize() > 0) {
		//	glCompressedTexImage1D(texture->_type, 0, iformat, width, 0, s.compressedSize(), pixels.data(0));
		//}
		//else
		{
			//s.set();
			glTexImage3D(Texture::Type::TextureCubeArray, 0, iformat, width, height, 6* layers, 0, format, pixelType, pixels);
			//s.unset();
		}
		//pixels.unbind(BufferType::PixelUnpackBuffer());
	}
}
