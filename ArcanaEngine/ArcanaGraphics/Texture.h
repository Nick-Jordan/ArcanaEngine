#ifndef TEXTURE_H_
#define TEXTURE_H_

#include "GraphicsDefines.h"

#include "Object.h"
#include "Types.h"
#include "Image.h"

#include "opengl/include.h"

namespace Arcana
{
	class ARCANA_GRAPHICS_API TextureInstance;

	class ARCANA_GRAPHICS_API Texture : public Object
	{
	public:

		enum Type
		{
			UnknownType = -1,

			Texture1D = GL_TEXTURE_1D,
			Texture2D = GL_TEXTURE_2D,
			Texture3D = GL_TEXTURE_3D,
			TextureCube = GL_TEXTURE_CUBE_MAP,
			Texture1DArray = GL_TEXTURE_1D_ARRAY,
			Texture2DArray = GL_TEXTURE_2D_ARRAY,
			TextureCubeArray = GL_TEXTURE_CUBE_MAP_ARRAY,
			Texture2DMultisample = GL_TEXTURE_2D_MULTISAMPLE,
			Texture2DMutlisampleArray = GL_TEXTURE_2D_MULTISAMPLE_ARRAY,
			TextureRectangle = GL_TEXTURE_RECTANGLE,
			TextureBuffer = GL_TEXTURE_BUFFER
		};

		enum Format
		{
			UnknownFormat = -1,

			RGB = GL_RGB,
			RGB888 = RGB,
			RGB565 = GL_RGB565,
			RGBA = GL_RGBA,
			RGBA8888 = RGBA,
			RGBA4444,
			RGBA5551,
			Alpha = GL_ALPHA,
			Depth = GL_DEPTH_COMPONENT,
			Red = GL_RED,
			Green = GL_GREEN,
			Blue = GL_BLUE,
			StencilIndex = GL_STENCIL_INDEX,
			DepthStencil = GL_DEPTH_STENCIL,
			RG = GL_RG,
			BGR = GL_BGR,
			BGRA = GL_BGRA,
			RedInteger = GL_RED_INTEGER,
			BlueInteger = GL_BLUE_INTEGER,
			GreenInteger = GL_GREEN_INTEGER,
			RGInteger = GL_RG_INTEGER,
			RGBInteger = GL_RGB_INTEGER,
			RGBAInteger = GL_RGBA_INTEGER,
			BGRInteger = GL_BGR_INTEGER,
			BGRAInteger = GL_BGRA_INTEGER,
		};

		enum InternalFormat
		{
			UnknownInternalFormat = -1,

			R8 = GL_R8,
			R8_SNORM = GL_R8_SNORM,
			R16 = GL_R16,
			R16_SNORM = GL_R16_SNORM,
			RG8 = GL_RG8,
			RG8_SNORM = GL_RG8_SNORM,
			RG16 = GL_RG16,
			RG16_SNORM = GL_RG16_SNORM,
			R3_G3_B2 = GL_R3_G3_B2,
			RGB4 = GL_RGB4,
			RGB5 = GL_RGB5,
			RGB8 = GL_RGB8,
			RGB8_SNORM = GL_RGB8_SNORM,
			RGB10 = GL_RGB10,
			RGB12 = GL_RGB12,
			RGB16 = GL_RGB16,
			RGB16_SNORM = GL_RGB16_SNORM,
			RGBA2 = GL_RGBA2,
			RGBA4 = GL_RGBA4,
			RGB5_A1 = GL_RGB5_A1,
			RGBA8 = GL_RGBA8,
			RGBA8_SNORM = GL_RGBA8_SNORM,
			RGB10_A2 = GL_RGB10_A2,
			RGB10_A2UI = GL_RGB10_A2UI,
			RGBA12 = GL_RGBA12,
			RGBA16 = GL_RGBA16,
			RGBA16_SNORM = GL_RGBA16_SNORM,
			SRGB8 = GL_SRGB8,
			SRGB8_ALPHA8 = GL_SRGB8_ALPHA8,
			R16F = GL_R16F,
			RG16F = GL_RG16F,
			RGB16F = GL_RGB16F,
			RGBA16F = GL_RGBA16F,
			R32F = GL_R32F,
			RG32F = GL_RG32F,
			RGB32F = GL_RGB32F,
			RGBA32F = GL_RGBA32F,
			R11F_G11F_B10F = GL_R11F_G11F_B10F,
			RGB9_E5 = GL_RGB9_E5,
			R8I = GL_R8I,
			R8UI = GL_R8UI,
			R16I = GL_R16I,
			R16UI = GL_R16UI,
			R32I = GL_R32I,
			R32UI = GL_R32UI,
			RG8I = GL_RG8I,
			RG8UI = GL_RG8UI,
			RG16I = GL_RG8UI,
			RG16UI = GL_RG16UI,
			RG32I = GL_RG32I,
			RG32UI = GL_RG32UI,
			RGB8I = GL_RGB8I,
			RGB8UI = GL_RGB8UI,
			RGB16I = GL_RGB16I,
			RGB16UI = GL_RGB16UI,
			RGB32I = GL_RGB32I,
			RGB32UI = GL_RGB32UI,
			RGBA8I = GL_RGBA8I,
			RGBA8UI = GL_RGBA8UI,
			RGBA16I = GL_RGBA16I,
			RGBA16UI = GL_RGBA16UI,
			RGBA32I = GL_RGBA32I,
			RGBA32UI = GL_RGBA32UI,
			DEPTH_COMPONENT16 = GL_DEPTH_COMPONENT16,
			DEPTH_COMPONENT24 = GL_DEPTH_COMPONENT24,
			DEPTH_COMPONENT32F = GL_DEPTH_COMPONENT32F,
			DEPTH32F_STENCIL8 = GL_DEPTH32F_STENCIL8,
			DEPTH24_STENCIL8 = GL_DEPTH24_STENCIL8,
			COMPRESSED_SIGNED_RED_RGTC1 = GL_COMPRESSED_SIGNED_RED_RGTC1,
			COMPRESSED_RG_RGTC2 = GL_COMPRESSED_RG_RGTC2,
			COMPRESSED_SIGNED_RG_RGTC2 = GL_COMPRESSED_SIGNED_RG_RGTC2,
			COMPRESSED_RGBA_BPTC_UNORM_ARB = GL_COMPRESSED_RGBA_BPTC_UNORM_ARB,
			COMPRESSED_SRGB_ALPHA_BPTC_UNORM_ARB = GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM_ARB,
			COMPRESSED_RGB_BPTC_SIGNED_FLOAT_ARB = GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT_ARB,
			COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT_ARB = GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT_ARB,
			COMPRESSED_RGB_S3TC_DXT1_EXT = GL_COMPRESSED_RGB_S3TC_DXT1_EXT,
			COMPRESSED_RGBA_S3TC_DXT1_EXT = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT,
			COMPRESSED_RGBA_S3TC_DXT3_EXT = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT,
		};

		enum PixelType
		{
			UnknownPixelType = -1,

			UnsignedByte = GL_UNSIGNED_BYTE,
			Byte = GL_BYTE,
			UnsignedShort = GL_UNSIGNED_SHORT,
			Short = GL_SHORT,
			UnsignedInt = GL_UNSIGNED_INT,
			Int = GL_INT,
			Half = GL_HALF_FLOAT,
			Float = GL_FLOAT,
			UnsignedByte_3_3_2 = GL_UNSIGNED_BYTE_3_3_2,
			UnsignedByte_2_3_3_REV = GL_UNSIGNED_BYTE_2_3_3_REV,
			UnsignedShort_5_6_5 = GL_UNSIGNED_SHORT_5_6_5,
			UnsignedShort_5_6_5_REV = GL_UNSIGNED_SHORT_5_6_5_REV,
			UnsignedShort_4_4_4_4 = GL_UNSIGNED_SHORT_4_4_4_4,
			UnsignedShort_4_4_4_4_REV = GL_UNSIGNED_SHORT_4_4_4_4_REV,
			UnsignedShort_5_5_5_1 = GL_UNSIGNED_SHORT_5_5_5_1,
			UnsignedShort_1_5_5_5_REV = GL_UNSIGNED_SHORT_1_5_5_5_REV,
			UnsignedInt_8_8_8_8 = GL_UNSIGNED_INT_8_8_8_8,
			UnsignedInt_8_8_8_8_REV = GL_UNSIGNED_INT_8_8_8_8_REV,
			UnsignedInt_10_10_10_2 = GL_UNSIGNED_INT_10_10_10_2,
			UnsignedInt_2_10_10_10_REV = GL_UNSIGNED_INT_2_10_10_10_REV,
			UnsignedInt_24_8 = GL_UNSIGNED_INT_24_8,
			UnsignedInt_10F_11F_11F_REV = GL_UNSIGNED_INT_10F_11F_11F_REV,
			UnsignedInt_5_9_9_9_REV = GL_UNSIGNED_INT_5_9_9_9_REV,
			Float32UnsignedInt_24_8_REV = GL_FLOAT_32_UNSIGNED_INT_24_8_REV
		};

		enum CubeFace
		{
			PositiveX = GL_TEXTURE_CUBE_MAP_POSITIVE_X,
			NegativeX = GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
			PositiveY = GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
			NegativeY = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
			PositiveZ = GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
			NegativeZ = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
		};

		class ARCANA_GRAPHICS_API Parameters
		{
		public:


		};

		Texture();

		Texture(const Texture& copy);

		virtual ~Texture();

		Type getType() const;

		GLuint getId() const;

		uint32 getWidth() const;

		uint32 getHeight() const;

		uint32 getDepth() const;

		uint32 getLayers() const;

		bool isCompressed() const;

		Format getFormat() const;

		InternalFormat getInternalFormat() const;

		PixelType getPixelType() const;

		int64 getBitsPerPixel() const;

		uint32 getComponents() const;

		bool hasMipmap() const;

		bool setMipmap(bool mipmap);


		Texture& operator=(const Texture& copy);

	private:

		bool generateMipmap();

		bool invalidateMipmap();

		static int64 getFormatBitsPerPixel(Format format);

	public:

		//Texture Creation

		static Texture* createDefault();

		static Texture* create1D(Format format, uint32 width, InternalFormat iformat, PixelType pixelType, 
			const void* pixels, const Parameters& parameters = Parameters(), bool generateMipmap = false);   //Buffer instead of const void*?

		static Texture* create2D(Format format, uint32 width, uint32 height, InternalFormat iformat, PixelType pixelType,
			const void* pixels, const Parameters& parameters = Parameters(), bool generateMipmap = false);

		static Texture* create3D(Format format, uint32 width, uint32 height, uint32 depth, InternalFormat iformat, PixelType pixelType,
			const void* pixels, const Parameters& parameters = Parameters(), bool generateMipmap = false);

		static Texture* createCube(Format format, uint32 width, uint32 height, InternalFormat iformat, PixelType pixelType,
			void* pixels[6], const Parameters& parameters = Parameters(), bool generateMipmap = false);

		static Texture* create1DArray(Format format, uint32 width, uint32 layers, InternalFormat iformat, PixelType pixelType,
			const void* pixels, const Parameters& parameters = Parameters(), bool generateMipmap = false);

		static Texture* create2DArray(Format format, uint32 width, uint32 height, uint32 layers, InternalFormat iformat, PixelType pixelType,
			const void* pixels, const Parameters& parameters = Parameters(), bool generateMipmap = false);

		static Texture* createCubeArray(Format format, uint32 width, uint32 height, uint32 layers, InternalFormat iformat, PixelType pixelType,
			const void* pixels, const Parameters& parameters = Parameters(), bool generateMipmap = false);

		static Texture* create2DMultisample(Format format, uint32 width, uint32 height, uint32 samples, InternalFormat iformat, PixelType pixelType,
			bool fixedLocations);

		static Texture* create2DMultisampleArray(Format format, uint32 width, uint32 height, uint32 layers, uint32 samples, InternalFormat iformat, PixelType pixelType,
			bool fixedLocations);

		static Texture* createRectangle(Format format, uint32 width, uint32 height, InternalFormat iformat, PixelType pixelType,
			const void* pixels, const Parameters& parameters = Parameters(), bool generateMipmap = false);

		static Texture* createBuffer();

		template<typename ImagePixelType>
		static Texture* createFromImage(Image<ImagePixelType>* image, Format format, InternalFormat iformat, PixelType pixelType, 
			const Parameters& parameters = Parameters(), bool generateMipmap = false);

	private:

		TextureInstance* _instance;

		Type _type;
		Format _format;
		InternalFormat _internalFormat;
		PixelType _pixelType;
		int64 _bitsPerPixel;
		Parameters _parameters;
		bool _mipmap;
	};

	template<typename ImagePixelType>
	Texture* Texture::createFromImage(Image<ImagePixelType>* image, Format format, InternalFormat iformat, PixelType pixelType,
		const Parameters& parameters, bool generateMipmap)
	{
		return Texture::create2D(format, image->getWidth(), image->getHeight(), iformat, pixelType, image->getPixelsPtr(), parameters, generateMipmap);
	}
}

#endif // !TEXTURE_H_
