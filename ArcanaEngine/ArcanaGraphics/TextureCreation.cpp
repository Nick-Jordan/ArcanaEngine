#include "Texture.h"

#include "ArcanaLog.h"

//Texture Instances
#include "Texture1DInstance.h"
#include "Texture2DInstance.h"
#include "Texture3DInstance.h"
#include "TextureCubeInstance.h"
#include "Texture1DArrayInstance.h"
#include "Texture2DArrayInstance.h"
#include "TextureCubeArrayInstance.h"
#include "TextureRectangleInstance.h"

namespace Arcana
{
	Texture* Texture::createDefault()
	{
		Image<uint8> image;
		image.init(ImageFormat::RGBA, 256, 256, Vector4<uint8>(255));

		return Texture::createFromImage(&image, Texture::RGBA, Texture::RGBA8, Texture::UnsignedByte);
	}

	Texture* Texture::create1D(Format format, uint32 width, InternalFormat iformat, PixelType pixelType,
		const void* pixels, const Parameters& parameters, bool generateMipmap)   //Buffer?
	{
		Texture* texture = new Texture();

		texture->_type = Texture1D;
		texture->_format = format;
		texture->_internalFormat = iformat;
		texture->_pixelType = pixelType;
		texture->_bitsPerPixel = getFormatBitsPerPixel(format);
		texture->_parameters = parameters;

		texture->_instance = new Texture1DInstance(format, width, iformat, pixelType, pixels, parameters);
		texture->_instance->reference();

		if (generateMipmap)
		{
			texture->generateMipmap();
		}

		texture->reference();

		return texture;
	}

	Texture* Texture::create2D(Format format, uint32 width, uint32 height, InternalFormat iformat, PixelType pixelType,
		const void* pixels, const Parameters& parameters, bool generateMipmap)
	{
		Texture* texture = new Texture();

		texture->_type = Texture2D;
		texture->_format = format;
		texture->_internalFormat = iformat;
		texture->_pixelType = pixelType;
		texture->_bitsPerPixel = getFormatBitsPerPixel(format);
		texture->_parameters = parameters;

		texture->_instance = new Texture2DInstance(format, width, height, iformat, pixelType, pixels, parameters);
		texture->_instance->reference();

		if (generateMipmap)
		{
			texture->generateMipmap();
		}

		texture->reference();

		return texture;
	}

	Texture* Texture::create3D(Format format, uint32 width, uint32 height, uint32 depth, InternalFormat iformat, PixelType pixelType,
		const void* pixels, const Parameters& parameters, bool generateMipmap)
	{
		Texture* texture = new Texture();

		texture->_type = Texture3D;
		texture->_format = format;
		texture->_internalFormat = iformat;
		texture->_pixelType = pixelType;
		texture->_bitsPerPixel = getFormatBitsPerPixel(format);
		texture->_parameters = parameters;

		texture->_instance = new Texture3DInstance(format, width, height, depth, iformat, pixelType, pixels, parameters);
		texture->_instance->reference();

		if (generateMipmap)
		{
			texture->generateMipmap();
		}

		texture->reference();

		return texture;
	}

	Texture* Texture::createCube(Format format, uint32 width, uint32 height, InternalFormat iformat, PixelType pixelType,
		void* pixels[6], const Parameters& parameters, bool generateMipmap)
	{
		Texture* texture = new Texture();

		texture->_type = TextureCube;
		texture->_format = format;
		texture->_internalFormat = iformat;
		texture->_pixelType = pixelType;
		texture->_bitsPerPixel = getFormatBitsPerPixel(format);
		texture->_parameters = parameters;

		texture->_instance = new TextureCubeInstance(format, width, height, iformat, pixelType, pixels, parameters);
		texture->_instance->reference();

		if (generateMipmap)
		{
			texture->generateMipmap();
		}

		texture->reference();

		return texture;
	}

	Texture* Texture::create1DArray(Format format, uint32 width, uint32 layers, InternalFormat iformat, PixelType pixelType,
		const void* pixels, const Parameters& parameters, bool generateMipmap)
	{
		Texture* texture = new Texture();

		texture->_type = Texture1DArray;
		texture->_format = format;
		texture->_internalFormat = iformat;
		texture->_pixelType = pixelType;
		texture->_bitsPerPixel = getFormatBitsPerPixel(format);
		texture->_parameters = parameters;

		texture->_instance = new Texture1DArrayInstance(format, width, layers, iformat, pixelType, pixels, parameters);
		texture->_instance->reference();

		if (generateMipmap)
		{
			texture->generateMipmap();
		}

		texture->reference();

		return texture;
	}

	Texture* Texture::create2DArray(Format format, uint32 width, uint32 height, uint32 layers, InternalFormat iformat, PixelType pixelType,
		const void* pixels, const Parameters& parameters, bool generateMipmap)
	{
		Texture* texture = new Texture();

		texture->_type = Texture2DArray;
		texture->_format = format;
		texture->_internalFormat = iformat;
		texture->_pixelType = pixelType;
		texture->_bitsPerPixel = getFormatBitsPerPixel(format);
		texture->_parameters = parameters;

		texture->_instance = new Texture2DArrayInstance(format, width, height, layers, iformat, pixelType, pixels, parameters);
		texture->_instance->reference();

		if (generateMipmap)
		{
			texture->generateMipmap();
		}

		texture->reference();

		return texture;
	}

	Texture* Texture::createCubeArray(Format format, uint32 width, uint32 height, uint32 layers, InternalFormat iformat, PixelType pixelType,
		const void* pixels, const Parameters& parameters, bool generateMipmap)
	{
		Texture* texture = new Texture();

		texture->_type = TextureCubeArray;
		texture->_format = format;
		texture->_internalFormat = iformat;
		texture->_pixelType = pixelType;
		texture->_bitsPerPixel = getFormatBitsPerPixel(format);
		texture->_parameters = parameters;

		texture->_instance = new TextureCubeArrayInstance(format, width, height, layers, iformat, pixelType, pixels, parameters);
		texture->_instance->reference();

		if (generateMipmap)
		{
			texture->generateMipmap();
		}

		texture->reference();

		return texture;
	}

	Texture* Texture::create2DMultisample(Format format, uint32 width, uint32 height, uint32 samples, InternalFormat iformat, PixelType pixelType,
		bool fixedLocations)
	{
		LOG(Error, CoreEngine, "Creating 2D multisampled textures is currently unimplemented");

		return nullptr;
	}

	Texture* Texture::create2DMultisampleArray(Format format, uint32 width, uint32 height, uint32 layers, uint32 samples, InternalFormat iformat, PixelType pixelType,
		bool fixedLocations)
	{
		LOG(Error, CoreEngine, "Creating 2D multisampled array textures is currently unimplemented");

		return nullptr;
	}

	Texture* Texture::createRectangle(Format format, uint32 width, uint32 height, InternalFormat iformat, PixelType pixelType,
		const void* pixels, const Parameters& parameters, bool generateMipmap)
	{
		Texture* texture = new Texture();

		texture->_type = TextureRectangle;
		texture->_format = format;
		texture->_internalFormat = iformat;
		texture->_pixelType = pixelType;
		texture->_bitsPerPixel = getFormatBitsPerPixel(format);
		texture->_parameters = parameters;

		texture->_instance = new TextureRectangleInstance(format, width, height, iformat, pixelType, pixels, parameters);
		texture->_instance->reference();

		if (generateMipmap)
		{
			texture->generateMipmap();
		}

		texture->reference();

		return texture;
	}

	Texture* Texture::createBuffer()
	{
		LOG(Error, CoreEngine, "Creating buffer textures is currently unimplemented");

		return nullptr;
	}


	//updating

	void Texture::update2DArray(int32 level, int32 x, int32 y, int32 l, int32 w, int32 h, int32 d, Format f, PixelType t, const void* pixels)
	{
		bind();
		glTexSubImage3D(GL_TEXTURE_2D_ARRAY, level, x, y, l, w, h, d, f, t, pixels);
	}
}