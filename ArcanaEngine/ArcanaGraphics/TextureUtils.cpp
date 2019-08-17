#include "TextureUtils.h"

namespace Arcana
{
	Texture* TextureUtils::createImageCubeTexture(std::string files[6], bool fixAlignment)
	{
		Image<uint8> posX;
		posX.init(files[0]);
		Image<uint8> negX;
		negX.init(files[1]);
		Image<uint8> posY;
		posY.init(files[2]);
		Image<uint8> negY;
		negY.init(files[3]);
		Image<uint8> posZ;
		posZ.init(files[4]);
		Image<uint8> negZ;
		negZ.init(files[5]);

		if (fixAlignment)
		{
			posX.flip(Image<uint8>::FlipAxis::Vertical);
			negX.flip(Image<uint8>::FlipAxis::Vertical);
			posY.flip(Image<uint8>::FlipAxis::Horizontal);
			negY.flip(Image<uint8>::FlipAxis::Horizontal);
			posZ.flip(Image<uint8>::FlipAxis::Vertical);
			negZ.flip(Image<uint8>::FlipAxis::Horizontal);
		}

		const void* pixels[6] =
		{
			posX.getPixelsPtr(),
			negX.getPixelsPtr(),
			posY.getPixelsPtr(),
			negY.getPixelsPtr(),
			posZ.getPixelsPtr(),
			negZ.getPixelsPtr()
		};

		bool rgba = posX.getFormat() == ImageFormat::RGBA;

		Texture::Parameters params;
		params.setMinFilter(TextureFilter::Linear);
		params.setMagFilter(TextureFilter::Linear);
		Texture* cube = Texture::createCube(rgba ? Texture::RGBA : Texture::RGB, 2048, 2048, 
			rgba ? Texture::RGBA8 : Texture::RGB8, Texture::UnsignedByte, pixels, params);
		
		return cube;
	}
}