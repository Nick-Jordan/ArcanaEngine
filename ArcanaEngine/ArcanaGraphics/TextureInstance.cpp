#include "TextureInstance.h"

#include "Texture.h"
#include "RenderState.h"
#include "ArcanaLog.h"

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

	void TextureInstance::initialize(Texture::Type type, Texture::InternalFormat iformat, const Texture::Parameters& params)
	{
		glGenTextures(1, &_id);
		AE_ASSERT(_id > 0);

		glBindTexture(type, _id);

		params.set(type);
	}
}
