#include "TextureAttachment.h"

namespace Arcana
{

	TextureAttachment::TextureAttachment(const std::string& id, Texture* texture) : Framebuffer::Attachment(id), _texture(texture)
	{
	}

	TextureAttachment::TextureAttachment(const std::string& id, Texture* texture, Texture::CubeFace face) : Framebuffer::Attachment(id), _texture(texture), _face(face)
	{
		AE_REFERENCE(_texture);
	}

	TextureAttachment::~TextureAttachment()
	{
	}

	Texture* TextureAttachment::getTexture() const
	{
		return _texture;
	}

	uint32 TextureAttachment::getWidth() const
	{
		if (_texture)
		{
			return _texture->getWidth();
		}

		return 0;
	}

	uint32 TextureAttachment::getHeight() const
	{
		if (_texture)
		{
			return _texture->getHeight();
		}

		return 0;
	}

	void TextureAttachment::initialize(Framebuffer* framebuffer, uint32 index)
	{
		if (_texture)
		{

			Framebuffer* prev = Framebuffer::bind(framebuffer);

			if (_texture->getType() == Texture::Texture2D)
			{
				glBindTexture(_texture->getType(), _texture->getId());
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, _texture->getType(), _texture->getId(), 0);
			}
			else if (_texture->getType() == Texture::TextureCube && _face >= Texture::PositiveX && _face <= Texture::NegativeZ)
			{
				glBindTexture(_texture->getType(), _texture->getId());
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, _face, _texture->getId(), 0);
			}

			GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
			if (status != GL_FRAMEBUFFER_COMPLETE)
			{
				LOGF(Error, CoreEngine, "Framebuffer status incomplete after creating texture attachment, %s: 0x%x", getId().c_str(), status);
			}

			Framebuffer::bind(prev);
		}
	}
}