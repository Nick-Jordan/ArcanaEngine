#include "Texture.h"

#include "TextureInstance.h"

namespace Arcana
{

	Texture::Texture() 
		: _instance(nullptr), _type(UnknownType), 
		_format(UnknownFormat), _internalFormat(UnknownInternalFormat), 
		_pixelType(UnknownPixelType), _bitsPerPixel(0), _mipmap(false)
	{
	}

	Texture::Texture(const Texture& copy)
		: _instance(copy._instance), _type(copy._type),
		_format(copy._format), _internalFormat(copy._internalFormat),
		_pixelType(copy._pixelType), _bitsPerPixel(copy._bitsPerPixel), _mipmap(copy._mipmap)
	{
		_instance->reference();
	}

	Texture::~Texture()
	{
		if (_instance)
		{
			AE_RELEASE(_instance);
		}
	}

	Texture::Type Texture::getType() const
	{
		return _type;
	}

	GLuint Texture::getId() const
	{
		if (_instance)
			return _instance->getId();

		return 0;
	}

	uint32 Texture::getWidth() const
	{
		if (_instance)
			return _instance->getWidth();

		return 0;
	}

	uint32 Texture::getHeight() const
	{
		if (_instance)
			return _instance->getHeight();

		return 0;
	}

	uint32 Texture::getDepth() const
	{
		if (_instance)
			return _instance->getDepth();

		return 0;
	}

	uint32 Texture::getLayers() const
	{
		if(_instance)
			return _instance->getLayers();

		return 0;
	}

	bool Texture::isCompressed() const
	{
		return _internalFormat == COMPRESSED_SIGNED_RED_RGTC1
			|| _internalFormat == COMPRESSED_RG_RGTC2
			|| _internalFormat == COMPRESSED_SIGNED_RG_RGTC2
			|| _internalFormat == COMPRESSED_RGBA_BPTC_UNORM_ARB
			|| _internalFormat == COMPRESSED_SRGB_ALPHA_BPTC_UNORM_ARB
			|| _internalFormat == COMPRESSED_RGB_BPTC_SIGNED_FLOAT_ARB
			|| _internalFormat == COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT_ARB
			|| _internalFormat == COMPRESSED_RGB_S3TC_DXT1_EXT
			|| _internalFormat == COMPRESSED_RGBA_S3TC_DXT1_EXT
			|| _internalFormat == COMPRESSED_RGBA_S3TC_DXT3_EXT;
	}

	Texture::Format Texture::getFormat() const
	{
		return _format;
	}

	Texture::InternalFormat Texture::getInternalFormat() const
	{
		return _internalFormat;
	}

	Texture::PixelType Texture::getPixelType() const
	{
		return _pixelType;
	}

	int64 Texture::getBitsPerPixel() const
	{
		return _bitsPerPixel;
	}

	uint32 Texture::getComponents() const
	{
		switch (_format)
		{
		case Alpha:
		case Depth:
		case Red:
		case Green:
		case Blue:
		case RedInteger:
		case BlueInteger:
		case GreenInteger:
			return 1;
		case RG:
		case RGInteger:
			return 2;
		case RGB:
		case RGB565:
		case BGR:
		case RGBInteger:
		case BGRInteger:
			return 3;
		case RGBA:
		case RGBA4444:
		case RGBA5551:
		case BGRA:
		case RGBAInteger:
		case BGRAInteger:
			return 4;
		case StencilIndex:
			return 0;
		case DepthStencil:
			return 0;
		default:
			return 0;
		}
	}

	bool Texture::hasMipmap() const
	{
		return _mipmap;
	}

	bool Texture::setMipmap(bool mipmap)
	{
		if (mipmap)
		{
			return generateMipmap();
		}
		
		return invalidateMipmap();
	}

	bool Texture::generateMipmap()
	{
		if (!getId() || hasMipmap())
		{
			return false;
		}

		glBindTexture(_type, getId());
		glGenerateMipmap(_type);

		//glTexParameteri(_type, GL_TEXTURE_MIN_FILTER, _parameters.minFilter() == Linear ? GL_LINEAR_MIPMAP_LINEAR : GL_NEAREST_MIPMAP_LINEAR);

		//FIX  ^

		_mipmap = true;

		return true;
	}

	bool Texture::invalidateMipmap()
	{
		if (!_mipmap)
			return false;

		glBindTexture(_type, getId());

		//glTexParameteri(_type, GL_TEXTURE_MIN_FILTER, _parameters.minFilter());

		//FIX  ^

		_mipmap = false;

		return true;
	}

	int64 Texture::getFormatBitsPerPixel(Texture::Format format)
	{
		switch (format)
		{
		case Alpha:
		case Depth:
		case Red:
		case Green:
		case Blue:
		case RedInteger:
		case BlueInteger:
		case GreenInteger:
			return 8;
		case RGB565:
		case RGBA4444:
		case RGBA5551:
		case RG:
		case RGInteger:
			return 16;
		case RGB:
		case BGR:
		case RGBInteger:
		case BGRInteger:
			return 24;
		case RGBA:
		case BGRA:
		case RGBAInteger:
		case BGRAInteger:
			return 32;
		case StencilIndex:
			return -1;
		case DepthStencil:
			return -1;
		defaut:
			return -1;
		}
	}


	Texture& Texture::operator=(const Texture& copy)
	{
		_instance = copy._instance;
		_type = copy._type;
		_format = copy._format;
		_internalFormat = copy._internalFormat;
		_pixelType = copy._pixelType;
		_bitsPerPixel = copy._bitsPerPixel;
		_mipmap = copy._mipmap;

		_instance->reference();

		return *this;
	}

}
