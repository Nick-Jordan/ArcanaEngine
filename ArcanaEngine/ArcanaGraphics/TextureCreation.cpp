#include "Texture.h"
#include "TextureManager.h"

#include "ArcanaLog.h"

namespace Arcana
{
	Texture* Texture::createDefault()
	{
		Image<uint8> image;
		image.init(ImageFormat::RGBA, 2, 2, Vector4<uint8>(160));
		image.setPixel(0, 0, Color(0, 0, 200, 255));
		image.setPixel(1, 1, Color(0, 0, 200, 255));

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
		texture->_width = width;

		texture->createId();
		texture->bind();
		parameters.set(Texture1D);

		glTexImage1D(Texture1D, 0, iformat, width, 0, format, pixelType, pixels);
		
		if (generateMipmap)
		{
			texture->generateMipmap();
		}

		texture->unbind();
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
		texture->_width = width;
		texture->_height = height;

		texture->createId();
		texture->bind();
		parameters.set(Texture2D);

		glTexImage2D(Texture2D, 0, iformat, width, height, 0, format, pixelType, pixels);

		if (generateMipmap)
		{
			texture->generateMipmap();
		}

		texture->unbind();
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
		texture->_width = width;
		texture->_height = height;
		texture->_depth = depth;

		texture->createId();
		texture->bind();
		parameters.set(Texture3D);

		glTexImage3D(Texture3D, 0, iformat, width, height, depth, 0, format, pixelType, pixels);

		if (generateMipmap)
		{
			texture->generateMipmap();
		}

		texture->unbind();
		texture->reference();

		return texture;
	}

	Texture* Texture::createCube(Format format, uint32 width, uint32 height, InternalFormat iformat, PixelType pixelType,
		const void* pixels[6], const Parameters& parameters, bool generateMipmap)
	{
		Texture* texture = new Texture();

		texture->_type = TextureCube;
		texture->_format = format;
		texture->_internalFormat = iformat;
		texture->_pixelType = pixelType;
		texture->_bitsPerPixel = getFormatBitsPerPixel(format);
		texture->_parameters = parameters;
		texture->_width = width;
		texture->_height = height;

		const Texture::CubeFace faces[6] = 
		{
			Texture::CubeFace::PositiveX,
			Texture::CubeFace::NegativeX,
			Texture::CubeFace::PositiveY,
			Texture::CubeFace::NegativeY,
			Texture::CubeFace::PositiveZ,
			Texture::CubeFace::NegativeZ
		};

		texture->createId();
		texture->bind();
		parameters.set(TextureCube);

		for (int i = 0; i < 6; ++i)
		{
			glTexImage2D(faces[i], 0, iformat, width, height, 0, format, pixelType, pixels ? pixels[i] : nullptr);
		}

		if (generateMipmap)
		{
			texture->generateMipmap();
		}

		texture->unbind();
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
		texture->_width = width;
		texture->_layers = layers;

		texture->createId();
		texture->bind();
		parameters.set(Texture1DArray);

		glTexImage2D(Texture1DArray, 0, iformat, width, layers, 0, format, pixelType, pixels);

		if (generateMipmap)
		{
			texture->generateMipmap();
		}

		texture->unbind();
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
		texture->_width = width;
		texture->_height = height;
		texture->_layers = layers;

		texture->createId();
		texture->bind();
		parameters.set(Texture2DArray);

		glTexImage3D(Texture2DArray, 0, iformat, width, height, layers, 0, format, pixelType, pixels);

		if (generateMipmap)
		{
			texture->generateMipmap();
		}

		texture->unbind();
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
		texture->_width = width;
		texture->_height = height;
		texture->_layers = layers;

		texture->createId();
		texture->bind();
		parameters.set(TextureCubeArray);

		glTexImage3D(TextureCubeArray, 0, iformat, width, height, 6 * layers, 0, format, pixelType, pixels);

		if (generateMipmap)
		{
			texture->generateMipmap();
		}

		texture->unbind();
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
		texture->_width = width;
		texture->_height = height;

		texture->createId();
		texture->bind();
		parameters.set(TextureRectangle);

		glTexImage2D(TextureRectangle, 0, iformat, width, height, 0, format, pixelType, pixels);

		if (generateMipmap)
		{
			texture->generateMipmap();
		}

		texture->unbind();
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