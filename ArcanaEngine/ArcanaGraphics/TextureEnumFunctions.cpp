#include "Texture.h"

#include "StringUtils.h"

namespace Arcana
{
	Texture::Format Texture::getTextureFormat(const std::string& format)
	{
		if (StringUtils::equalsIgnoreCase(format, "rgb"))
		{
			return RGB;
		}
		if (StringUtils::equalsIgnoreCase(format, "rgb888"))
		{
			return RGB888;
		}
		if (StringUtils::equalsIgnoreCase(format, "rgb565"))
		{
			return RGB565;
		}
		if (StringUtils::equalsIgnoreCase(format, "rgba"))
		{
			return RGBA;
		}
		if (StringUtils::equalsIgnoreCase(format, "rgba8888"))
		{
			return RGBA8888;
		}
		if (StringUtils::equalsIgnoreCase(format, "rgba4444"))
		{
			return RGBA4444;
		}
		if (StringUtils::equalsIgnoreCase(format, "rgba5551"))
		{
			return RGBA5551;
		}
		if (StringUtils::equalsIgnoreCase(format, "alpha"))
		{
			return Alpha;
		}
		if (StringUtils::equalsIgnoreCase(format, "depth"))
		{
			return Depth;
		}
		if (StringUtils::equalsIgnoreCase(format, "red"))
		{
			return Red;
		}
		if (StringUtils::equalsIgnoreCase(format, "green"))
		{
			return Green;
		}
		if (StringUtils::equalsIgnoreCase(format, "blue"))
		{
			return Blue;
		}
		if (StringUtils::equalsIgnoreCase(format, "stencilindex"))
		{
			return StencilIndex;
		}
		if (StringUtils::equalsIgnoreCase(format, "depthstencil"))
		{
			return DepthStencil;
		}
		if (StringUtils::equalsIgnoreCase(format, "rg"))
		{
			return RG;
		}
		if (StringUtils::equalsIgnoreCase(format, "BGR"))
		{
			return BGR;
		}
		if (StringUtils::equalsIgnoreCase(format, "BGRA"))
		{
			return BGRA;
		}
		if (StringUtils::equalsIgnoreCase(format, "redinteger"))
		{
			return RedInteger;
		}
		if (StringUtils::equalsIgnoreCase(format, "blueinteger"))
		{
			return BlueInteger;
		}
		if (StringUtils::equalsIgnoreCase(format, "greeninteger"))
		{
			return GreenInteger;
		}
		if (StringUtils::equalsIgnoreCase(format, "rginteger"))
		{
			return RGInteger;
		}
		if (StringUtils::equalsIgnoreCase(format, "greeninteger"))
		{
			return RGBInteger;
		}
		if (StringUtils::equalsIgnoreCase(format, "rgbainteger"))
		{
			return RGBAInteger;
		}
		if (StringUtils::equalsIgnoreCase(format, "bgrinteger"))
		{
			return BGRInteger;
		}
		if (StringUtils::equalsIgnoreCase(format, "bgrainteger"))
		{
			return BGRAInteger;
		}

		return UnknownFormat;
	}

	Texture::InternalFormat Texture::getTextureInternalFormat(const std::string& format)
	{
		if (StringUtils::equalsIgnoreCase(format, "R8"))
		{
			return R8;
		}
		if (StringUtils::equalsIgnoreCase(format, "R8_SNORM"))
		{
			return R8_SNORM;
		}
		if (StringUtils::equalsIgnoreCase(format, "R16"))
		{
			return R16;
		}
		if (StringUtils::equalsIgnoreCase(format, "R16_SNORM"))
		{
			return R16_SNORM;
		}
		if (StringUtils::equalsIgnoreCase(format, "RG8"))
		{
			return RG8;
		}
		if (StringUtils::equalsIgnoreCase(format, "RG8_SNORM"))
		{
			return RG8_SNORM;
		}
		if (StringUtils::equalsIgnoreCase(format, "RG16"))
		{
			return RG16;
		}
		if (StringUtils::equalsIgnoreCase(format, "RG16_SNORM"))
		{
			return RG16_SNORM;
		}
		if (StringUtils::equalsIgnoreCase(format, "R3_G3_B2"))
		{
			return R3_G3_B2;
		}
		if (StringUtils::equalsIgnoreCase(format, "RGB4"))
		{
			return RGB4;
		}
		if (StringUtils::equalsIgnoreCase(format, "RGB5"))
		{
			return RGB5;
		}
		if (StringUtils::equalsIgnoreCase(format, "RGB8"))
		{
			return RGB8;
		}
		if (StringUtils::equalsIgnoreCase(format, "RGB8_SNORM"))
		{
			return RGB8_SNORM;
		}
		if (StringUtils::equalsIgnoreCase(format, "RGB10"))
		{
			return RGB10;
		}
		if (StringUtils::equalsIgnoreCase(format, "RGB12"))
		{
			return RGB12;
		}
		if (StringUtils::equalsIgnoreCase(format, "RGB16"))
		{
			return RGB16;
		}
		if (StringUtils::equalsIgnoreCase(format, "RGB16_SNORM"))
		{
			return RGB16_SNORM;
		}
		if (StringUtils::equalsIgnoreCase(format, "RGBA2"))
		{
			return RGBA2;
		}
		if (StringUtils::equalsIgnoreCase(format, "RGBA4"))
		{
			return RGBA4;
		}
		if (StringUtils::equalsIgnoreCase(format, "RGB5_A1"))
		{
			return RGB5_A1;
		}
		if (StringUtils::equalsIgnoreCase(format, "RGBA8"))
		{
			return RGBA8;
		}
		if (StringUtils::equalsIgnoreCase(format, "RGBA8_SNORM"))
		{
			return RGBA8_SNORM;
		}
		if (StringUtils::equalsIgnoreCase(format, "RGB10_A2"))
		{
			return RGB10_A2;
		}
		if (StringUtils::equalsIgnoreCase(format, "RGB10_A2UI"))
		{
			return RGB10_A2UI;
		}
		if (StringUtils::equalsIgnoreCase(format, "RGBA12"))
		{
			return RGBA12;
		}
		if (StringUtils::equalsIgnoreCase(format, "RGBA16"))
		{
			return RGBA16;
		}
		if (StringUtils::equalsIgnoreCase(format, "SRGB8"))
		{
			return SRGB8;
		}
		if (StringUtils::equalsIgnoreCase(format, "SRGB8_ALPHA8"))
		{
			return SRGB8_ALPHA8;
		}
		if (StringUtils::equalsIgnoreCase(format, "R16F"))
		{
			return R16F;
		}
		if (StringUtils::equalsIgnoreCase(format, "RGBA2"))
		{
			return RGBA2;
		}
		if (StringUtils::equalsIgnoreCase(format, "RGBA4"))
		{
			return RGBA4;
		}
		if (StringUtils::equalsIgnoreCase(format, "RGB5_A1"))
		{
			return RGB5_A1;
		}
		if (StringUtils::equalsIgnoreCase(format, "RGBA8_SNORM"))
		{
			return RGBA8_SNORM;
		}
		if (StringUtils::equalsIgnoreCase(format, "RGB10_A2"))
		{
			return RGB10_A2;
		}
		if (StringUtils::equalsIgnoreCase(format, "RGB5_A1"))
		{
			return RGB5_A1;
		}
		if (StringUtils::equalsIgnoreCase(format, "RGBA8"))
		{
			return RGBA8;
		}
		if (StringUtils::equalsIgnoreCase(format, "RGBA8_SNORM"))
		{
			return RGBA8_SNORM;
		}
		if (StringUtils::equalsIgnoreCase(format, "RGB10_A2"))
		{
			return RGB10_A2;
		}
		if (StringUtils::equalsIgnoreCase(format, "RGB10_A2UI"))
		{
			return RGB10_A2UI;
		}
		if (StringUtils::equalsIgnoreCase(format, "RGBA12"))
		{
			return RGBA12;
		}
		if (StringUtils::equalsIgnoreCase(format, "RGBA16"))
		{
			return RGBA16;
		}
		if (StringUtils::equalsIgnoreCase(format, "RGBA16_SNORM"))
		{
			return RGBA16_SNORM;
		}
		if (StringUtils::equalsIgnoreCase(format, "SRGB8"))
		{
			return SRGB8;
		}
		if (StringUtils::equalsIgnoreCase(format, "SRGB8_ALPHA8"))
		{
			return SRGB8_ALPHA8;
		}
		if (StringUtils::equalsIgnoreCase(format, "RGBA12"))
		{
			return RGBA12;
		}
		if (StringUtils::equalsIgnoreCase(format, "RGBA16"))
		{
			return RGBA16;
		}
		if (StringUtils::equalsIgnoreCase(format, "RGBA16_SNORM"))
		{
			return RGBA16_SNORM;
		}
		if (StringUtils::equalsIgnoreCase(format, "SRGB8"))
		{
			return SRGB8;
		}
		if (StringUtils::equalsIgnoreCase(format, "SRGB8_ALPHA8"))
		{
			return SRGB8_ALPHA8;
		}
		if (StringUtils::equalsIgnoreCase(format, "R16F"))
		{
			return R16F;
		}
		if (StringUtils::equalsIgnoreCase(format, "RG16F"))
		{
			return RG16F;
		}
		if (StringUtils::equalsIgnoreCase(format, "RGB16F"))
		{
			return RGB16F;
		}
		if (StringUtils::equalsIgnoreCase(format, "RGBA16F"))
		{
			return RGBA16F;
		}
		if (StringUtils::equalsIgnoreCase(format, "R32F"))
		{
			return R32F;
		}
		if (StringUtils::equalsIgnoreCase(format, "RG32F"))
		{
			return RG32F;
		}
		if (StringUtils::equalsIgnoreCase(format, "RGB32F"))
		{
			return RGB32F;
		}
		if (StringUtils::equalsIgnoreCase(format, "RGBA32F"))
		{
			return RGBA32F;
		}
		if (StringUtils::equalsIgnoreCase(format, "R11F_G11F_B10F"))
		{
			return R11F_G11F_B10F;
		}
		if (StringUtils::equalsIgnoreCase(format, "RGB9_E5"))
		{
			return RGB9_E5;
		}
		if (StringUtils::equalsIgnoreCase(format, "R8I"))
		{
			return R8I;
		}
		if (StringUtils::equalsIgnoreCase(format, "R8UI"))
		{
			return R8UI;
		}
		if (StringUtils::equalsIgnoreCase(format, "R16I"))
		{
			return R16I;
		}
		if (StringUtils::equalsIgnoreCase(format, "R16UI"))
		{
			return R16UI;
		}
		if (StringUtils::equalsIgnoreCase(format, "R32I"))
		{
			return R32I;
		}
		if (StringUtils::equalsIgnoreCase(format, "R32UI"))
		{
			return R32UI;
		}
		if (StringUtils::equalsIgnoreCase(format, "RG8I"))
		{
			return RG8I;
		}
		if (StringUtils::equalsIgnoreCase(format, "RG8UI"))
		{
			return RG8UI;
		}
		if (StringUtils::equalsIgnoreCase(format, "RG16I"))
		{
			return RG16I;
		}
		if (StringUtils::equalsIgnoreCase(format, "RG16UI"))
		{
			return RG16UI;
		}
		if (StringUtils::equalsIgnoreCase(format, "RG32I"))
		{
			return RG32I;
		}
		if (StringUtils::equalsIgnoreCase(format, "RG32UI"))
		{
			return RG32UI;
		}
		if (StringUtils::equalsIgnoreCase(format, "RGB8I"))
		{
			return RGB8I;
		}
		if (StringUtils::equalsIgnoreCase(format, "RGB8UI"))
		{
			return RGB8UI;
		}
		if (StringUtils::equalsIgnoreCase(format, "RGB16I"))
		{
			return RGB16I;
		}
		if (StringUtils::equalsIgnoreCase(format, "RGB16UI"))
		{
			return RGB16UI;
		}
		if (StringUtils::equalsIgnoreCase(format, "RG8UI"))
		{
			return RG8UI;
		}
		if (StringUtils::equalsIgnoreCase(format, "RG16I"))
		{
			return RG16I;
		}
		if (StringUtils::equalsIgnoreCase(format, "RG16UI"))
		{
			return RG16UI;
		}
		if (StringUtils::equalsIgnoreCase(format, "RG32I"))
		{
			return RG32I;
		}
		if (StringUtils::equalsIgnoreCase(format, "RG32UI"))
		{
			return RG32UI;
		}
		if (StringUtils::equalsIgnoreCase(format, "RGB8I"))
		{
			return RGB8I;
		}
		if (StringUtils::equalsIgnoreCase(format, "RGB8UI"))
		{
			return RGB8UI;
		}
		if (StringUtils::equalsIgnoreCase(format, "RGB16I"))
		{
			return RGB16I;
		}
		if (StringUtils::equalsIgnoreCase(format, "RGB16UI"))
		{
			return RGB16UI;
		}
		if (StringUtils::equalsIgnoreCase(format, "RGB32I"))
		{
			return RGB32I;
		}
		if (StringUtils::equalsIgnoreCase(format, "RGB32UI"))
		{
			return RGB32UI;
		}
		if (StringUtils::equalsIgnoreCase(format, "RGBA8I"))
		{
			return RGBA8I;
		}
		if (StringUtils::equalsIgnoreCase(format, "RGBA8UI"))
		{
			return RGBA8UI;
		}
		if (StringUtils::equalsIgnoreCase(format, "RGBA16I"))
		{
			return RGBA16I;
		}
		if (StringUtils::equalsIgnoreCase(format, "RGBA16UI"))
		{
			return RGBA16UI;
		}
		if (StringUtils::equalsIgnoreCase(format, "RGBA32I"))
		{
			return RGBA32I;
		}
		if (StringUtils::equalsIgnoreCase(format, "RGBA32UI"))
		{
			return RGBA32UI;
		}
		if (StringUtils::equalsIgnoreCase(format, "DEPTH_COMPONENT"))
		{
			return DEPTH_COMPONENT;
		}
		if (StringUtils::equalsIgnoreCase(format, "DEPTH_COMPONENT16"))
		{
			return DEPTH_COMPONENT16;
		}
		if (StringUtils::equalsIgnoreCase(format, "DEPTH_COMPONENT24"))
		{
			return DEPTH_COMPONENT24;
		}
		if (StringUtils::equalsIgnoreCase(format, "DEPTH_COMPONENT32F"))
		{
			return DEPTH_COMPONENT32F;
		}
		if (StringUtils::equalsIgnoreCase(format, "DEPTH32F_STENCIL8"))
		{
			return DEPTH32F_STENCIL8;
		}
		if (StringUtils::equalsIgnoreCase(format, "DEPTH24_STENCIL8"))
		{
			return DEPTH24_STENCIL8;
		}
		if (StringUtils::equalsIgnoreCase(format, "COMPRESSED_SIGNED_RED_RGTC1"))
		{
			return COMPRESSED_SIGNED_RED_RGTC1;
		}
		if (StringUtils::equalsIgnoreCase(format, "COMPRESSED_RG_RGTC2"))
		{
			return COMPRESSED_RG_RGTC2;
		}
		if (StringUtils::equalsIgnoreCase(format, "COMPRESSED_SIGNED_RG_RGTC2"))
		{
			return COMPRESSED_SIGNED_RG_RGTC2;
		}
		if (StringUtils::equalsIgnoreCase(format, "COMPRESSED_RGBA_BPTC_UNORM_ARB"))
		{
			return COMPRESSED_RGBA_BPTC_UNORM_ARB;
		}
		if (StringUtils::equalsIgnoreCase(format, "COMPRESSED_SRGB_ALPHA_BPTC_UNORM_ARB"))
		{
			return COMPRESSED_SRGB_ALPHA_BPTC_UNORM_ARB;
		}
		if (StringUtils::equalsIgnoreCase(format, "COMPRESSED_RGB_BPTC_SIGNED_FLOAT_ARB"))
		{
			return COMPRESSED_RGB_BPTC_SIGNED_FLOAT_ARB;
		}
		if (StringUtils::equalsIgnoreCase(format, "COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT_ARB"))
		{
			return COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT_ARB;
		}
		if (StringUtils::equalsIgnoreCase(format, "COMPRESSED_RGB_S3TC_DXT1_EXT"))
		{
			return COMPRESSED_RGB_S3TC_DXT1_EXT;
		}
		if (StringUtils::equalsIgnoreCase(format, "COMPRESSED_RGBA_S3TC_DXT1_EXT"))
		{
			return COMPRESSED_RGBA_S3TC_DXT1_EXT;
		}
		if (StringUtils::equalsIgnoreCase(format, "COMPRESSED_RGBA_S3TC_DXT3_EXT"))
		{
			return COMPRESSED_RGBA_S3TC_DXT3_EXT;
		}

		return UnknownInternalFormat;
	}

	Texture::PixelType Texture::getTexturePixelType(const std::string& type)
	{
		if (StringUtils::equalsIgnoreCase(type, "UnsignedByte"))
		{
			return UnsignedByte;
		}
		if (StringUtils::equalsIgnoreCase(type, "Byte"))
		{
			return Byte;
		}
		if (StringUtils::equalsIgnoreCase(type, "UnsignedShort"))
		{
			return UnsignedShort;
		}
		if (StringUtils::equalsIgnoreCase(type, "Short"))
		{
			return Short;
		}
		if (StringUtils::equalsIgnoreCase(type, "UnsignedInt"))
		{
			return UnsignedInt;
		}
		if (StringUtils::equalsIgnoreCase(type, "Int"))
		{
			return Int;
		}
		if (StringUtils::equalsIgnoreCase(type, "Half"))
		{
			return Half;
		}
		if (StringUtils::equalsIgnoreCase(type, "UnsignedByte_3_3_2"))
		{
			return UnsignedByte_3_3_2;
		}
		if (StringUtils::equalsIgnoreCase(type, "UnsignedByte_2_3_3_REV"))
		{
			return UnsignedByte_2_3_3_REV;
		}
		if (StringUtils::equalsIgnoreCase(type, "UnsignedShort_5_6_5"))
		{
			return UnsignedShort_5_6_5;
		}
		if (StringUtils::equalsIgnoreCase(type, "UnsignedShort_5_6_5_REV"))
		{
			return UnsignedShort_5_6_5_REV;
		}
		if (StringUtils::equalsIgnoreCase(type, "UnsignedShort_4_4_4_4"))
		{
			return UnsignedShort_4_4_4_4;
		}
		if (StringUtils::equalsIgnoreCase(type, "UnsignedShort_4_4_4_4_REV"))
		{
			return UnsignedShort_4_4_4_4_REV;
		}
		if (StringUtils::equalsIgnoreCase(type, "UnsignedShort_5_5_5_1"))
		{
			return UnsignedShort_5_5_5_1;
		}
		if (StringUtils::equalsIgnoreCase(type, "UnsignedShort_1_5_5_5_REV"))
		{
			return UnsignedShort_1_5_5_5_REV;
		}
		if (StringUtils::equalsIgnoreCase(type, "UnsignedInt_8_8_8_8"))
		{
			return UnsignedInt_8_8_8_8;
		}
		if (StringUtils::equalsIgnoreCase(type, "UnsignedInt_8_8_8_8_REV"))
		{
			return UnsignedInt_8_8_8_8_REV;
		}
		if (StringUtils::equalsIgnoreCase(type, "UnsignedInt_10_10_10_2"))
		{
			return UnsignedInt_10_10_10_2;
		}
		if (StringUtils::equalsIgnoreCase(type, "UnsignedInt_2_10_10_10_REV"))
		{
			return UnsignedInt_2_10_10_10_REV;
		}
		if (StringUtils::equalsIgnoreCase(type, "UnsignedInt_24_8"))
		{
			return UnsignedInt_24_8;
		}
		if (StringUtils::equalsIgnoreCase(type, "UnsignedInt_10F_11F_11F_REV"))
		{
			return UnsignedInt_10F_11F_11F_REV;
		}
		if (StringUtils::equalsIgnoreCase(type, "UnsignedInt_5_9_9_9_REV"))
		{
			return UnsignedInt_5_9_9_9_REV;
		}
		if (StringUtils::equalsIgnoreCase(type, "Float32UnsignedInt_24_8_REV"))
		{
			return Float32UnsignedInt_24_8_REV;
		}

		return UnknownPixelType;
	}

	Texture::CubeFace Texture::getTextureCubeFace(const std::string& face)
	{
		if (StringUtils::equalsIgnoreCase(face, "PositiveX") || StringUtils::equalsIgnoreCase(face, "posX"))
		{
			return PositiveX;
		}
		if (StringUtils::equalsIgnoreCase(face, "NegativeX") || StringUtils::equalsIgnoreCase(face, "negX"))
		{
			return NegativeX;
		}
		if (StringUtils::equalsIgnoreCase(face, "PositiveY") || StringUtils::equalsIgnoreCase(face, "posY"))
		{
			return PositiveY;
		}
		if (StringUtils::equalsIgnoreCase(face, "NegativeY") || StringUtils::equalsIgnoreCase(face, "negY"))
		{
			return NegativeY;
		}
		if (StringUtils::equalsIgnoreCase(face, "PositiveZ") || StringUtils::equalsIgnoreCase(face, "posZ"))
		{
			return PositiveZ;
		}
		if (StringUtils::equalsIgnoreCase(face, "NegativeZ") || StringUtils::equalsIgnoreCase(face, "negZ"))
		{
			return NegativeZ;
		}

		return UnknownCubeFace;
	}

	TextureWrap Texture::getTextureWrap(const std::string& wrap)
	{
		if (StringUtils::equalsIgnoreCase(wrap, "ClampToEdge"))
		{
			return TextureWrap::ClampToEdge;
		}
		if (StringUtils::equalsIgnoreCase(wrap, "ClampBorder"))
		{
			return TextureWrap::ClampBorder;
		}
		if (StringUtils::equalsIgnoreCase(wrap, "MirroredRepeat"))
		{
			return TextureWrap::MirroredRepeat;
		}

		return TextureWrap::Repeat;
	}

	TextureFilter Texture::getTextureFilter(const std::string& filter)
	{
		if (StringUtils::equalsIgnoreCase(filter, "Linear"))
		{
			return TextureFilter::Linear;
		}
		if (StringUtils::equalsIgnoreCase(filter, "NearestMipmapNearest"))
		{
			return TextureFilter::NearestMipmapNearest;
		}
		if (StringUtils::equalsIgnoreCase(filter, "LinearMipmapNearest"))
		{
			return TextureFilter::LinearMipmapNearest;
		}
		if (StringUtils::equalsIgnoreCase(filter, "NearestMipmapLinear"))
		{
			return TextureFilter::NearestMipmapLinear;
		}
		if (StringUtils::equalsIgnoreCase(filter, "LinearMipmapLinear"))
		{
			return TextureFilter::LinearMipmapLinear;
		}

		return TextureFilter::Nearest;
	}
}