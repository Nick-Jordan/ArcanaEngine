#include "TextureCubeInstance.h"

namespace Arcana
{
	TextureCubeInstance::TextureCubeInstance(Texture::Format format, uint32 width, uint32 height, Texture::InternalFormat iformat, Texture::PixelType pixelType,
		void* pixels[6], const Texture::Parameters& parameters)
	{
		initialize(format, width, height, iformat, pixelType, pixels, parameters);
	}

	TextureCubeInstance::~TextureCubeInstance()
	{

	}

	uint32 TextureCubeInstance::getWidth() const
	{
		return _width;
	}

	uint32 TextureCubeInstance::getHeight() const
	{
		return _height;
	}

	void TextureCubeInstance::initialize(Texture::Format format, uint32 width, uint32 height, Texture::InternalFormat iformat, Texture::PixelType pixelType,
		void* pixels[6], const Texture::Parameters& parameters)
	{
		_width = width;
		_height = height;

		TextureInstance::initialize(Texture::Type::TextureCube, iformat, parameters);

		glBindTexture(Texture::Type::TextureCube, _id);

		const Texture::CubeFace faces[6] = {
			Texture::CubeFace::PositiveX,
			Texture::CubeFace::NegativeX,
			Texture::CubeFace::PositiveY,
			Texture::CubeFace::NegativeY,
			Texture::CubeFace::PositiveZ,
			Texture::CubeFace::NegativeZ
		};
		/*if (texture->isCompressed()) {
			for (int i = 0; i < 6; ++i) {
				pixels[i]->bind(BufferType::PixelUnpackBuffer());
				if (s[i].compressedSize() > 0) {
					glCompressedTexImage2D(FACES[i], 0, iformat, width, height, 0, s[i].compressedSize(), pixels[i]->data(0));
				}
				else {
					s[i].set();
					glTexImage2D(FACES[i], 0, iformat, width, height, 0, format, pixelType, pixels[i]->data(0));
					s[i].unset();
				}
				pixels[i]->unbind(BufferType::PixelUnpackBuffer());
			}
		}*/
		//else 
		{
			for (int i = 0; i < 6; ++i) {
				//pixels[i]->bind(BufferType::PixelUnpackBuffer());
				//s[i].set();
				glTexImage2D(faces[i], 0, iformat, width, height, 0, format, pixelType, pixels ? pixels[i] : nullptr);
				//s[i].unset();
				//pixels[i]->unbind(BufferType::PixelUnpackBuffer());
			}
		}

		glBindTexture(Texture::Type::TextureCube, 0);
	}
}
