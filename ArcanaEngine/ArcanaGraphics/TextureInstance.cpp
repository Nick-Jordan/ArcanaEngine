#include "TextureInstance.h"

#include "Texture.h"
#include "RenderState.h"

namespace Arcana
{

	TextureInstance::TextureInstance() : _id(0)
	{
	}


	TextureInstance::~TextureInstance()
	{
		if (_id)
		{
			glDeleteTextures(1, &_id);
		}
	}

	GLuint TextureInstance::getId() const
	{
		return _id;
	}

	uint32 TextureInstance::getWidth() const
	{
		return 0;
	}

	uint32 TextureInstance::getHeight() const
	{
		return 0;
	}

	uint32 TextureInstance::getDepth() const
	{
		return 0;
	}

	uint32 TextureInstance::getLayers() const
	{
		return 0;
	}

	void TextureInstance::initialize(Texture::Type type, Texture::InternalFormat iformat, const Texture::Parameters &params)
	{
		glGenTextures(1, &_id);
		AE_ASSERT(_id > 0);

		/*glBindTexture(type, _id);

		if (type == Texture::TextureBuffer) {
			return;
		}

		glTexParameteri(type, GL_TEXTURE_WRAP_S, params.wrapS());
		glTexParameteri(type, GL_TEXTURE_WRAP_T, params.wrapT());
		glTexParameteri(type, GL_TEXTURE_WRAP_R, params.wrapR());
		glTexParameteri(type, GL_TEXTURE_MIN_FILTER, params.minFilter());
		glTexParameteri(type, GL_TEXTURE_MAG_FILTER, params.magFilter());
		switch (params.borderType()) {
		case 0:
			glTexParameteriv(type, GL_TEXTURE_BORDER_COLOR, params.borderi());
			break;
		case 1:
			glTexParameterfv(type, GL_TEXTURE_BORDER_COLOR, params.borderf());
			break;
		case 2:
			glTexParameterIiv(type, GL_TEXTURE_BORDER_COLOR, params.borderIi());
			break;
		case 3:
			glTexParameterIuiv(type, GL_TEXTURE_BORDER_COLOR, params.borderIui());
			break;
		default:
			assert(false);
		}
		if (type != GL_TEXTURE_RECTANGLE) {
			glTexParameterf(type, GL_TEXTURE_MIN_LOD, params.lodMin());
			glTexParameterf(type, GL_TEXTURE_MAX_LOD, params.lodMax());
		}

		glTexParameterf(type, GL_TEXTURE_LOD_BIAS, params.lodBias());
		if (params.compareFunc() != RenderState::DepthFunction::Always) {
			glTexParameteri(type, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
			glTexParameteri(type, GL_TEXTURE_COMPARE_FUNC, params.compareFunc());
		}
		glTexParameterf(type, GL_TEXTURE_MAX_ANISOTROPY_EXT, params.maxAnisotropyEXT());
		glTexParameteri(type, GL_TEXTURE_SWIZZLE_R, getTextureSwizzle(params.swizzle()[0]));
		glTexParameteri(type, GL_TEXTURE_SWIZZLE_G, getTextureSwizzle(params.swizzle()[1]));
		glTexParameteri(type, GL_TEXTURE_SWIZZLE_B, getTextureSwizzle(params.swizzle()[2]));
		glTexParameteri(type, GL_TEXTURE_SWIZZLE_A, getTextureSwizzle(params.swizzle()[3]));
		if (type != GL_TEXTURE_RECTANGLE) {
			glTexParameteri(type, GL_TEXTURE_BASE_LEVEL, params.minLevel());
			glTexParameteri(type, GL_TEXTURE_MAX_LEVEL, params.maxLevel());
		}*/
	}
}
