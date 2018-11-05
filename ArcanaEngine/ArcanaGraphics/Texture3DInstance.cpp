#include "Texture3DInstance.h"

namespace Arcana
{
	Texture3DInstance::Texture3DInstance(Texture::Format format, uint32 width, uint32 height, uint32 depth, Texture::InternalFormat iformat, Texture::PixelType pixelType,
		const void* pixels, const Texture::Parameters& parameters)
	{
		initialize(format, width, height, depth, iformat, pixelType, pixels, parameters);
	}

	Texture3DInstance::~Texture3DInstance()
	{

	}

	uint32 Texture3DInstance::getWidth() const
	{
		return _width;
	}

	uint32 Texture3DInstance::getHeight() const
	{
		return _height;
	}

	uint32 Texture3DInstance::getDepth() const
	{
		return _depth;
	}

	void Texture3DInstance::initialize(Texture::Format format, uint32 width, uint32 height, uint32 depth, Texture::InternalFormat iformat, Texture::PixelType pixelType,
		const void* pixels, const Texture::Parameters& parameters)
	{
		_width = width;
		_height = height;
		_depth = depth;

		TextureInstance::initialize(Texture::Type::Texture3D, iformat, parameters);

		glBindTexture(Texture::Type::Texture3D, _id);

		//if (isCompressed() && s.compressedSize() > 0) {
		//	glCompressedTexImage1D(texture->_type, 0, iformat, width, 0, s.compressedSize(), pixels.data(0));
		//}
		//else
		{
			//s.set();
			glTexImage3D(Texture::Type::Texture3D, 0, iformat, width, height, depth, 0, format, pixelType, pixels);
			//s.unset();
		}
		//pixels.unbind(BufferType::PixelUnpackBuffer());
	}
}
