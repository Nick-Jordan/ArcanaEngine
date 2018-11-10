#include "RenderbufferObjectAttachment.h"

namespace Arcana
{

	RenderbufferObjectAttachment::RenderbufferObjectAttachment(const std::string& id, GLenum internalFormat, uint32 width, uint32 height)
		: Framebuffer::Attachment(id), _format(internalFormat), _width(width), _height(height)
	{

	}

	RenderbufferObjectAttachment::~RenderbufferObjectAttachment()
	{
		if (_handle)
		{
			glDeleteRenderbuffers(1, &_handle);
		}
	}

	GLenum RenderbufferObjectAttachment::getFormat() const
	{
		return _format;
	}

	uint32 RenderbufferObjectAttachment::getWidth() const
	{
		return _width;
	}

	uint32 RenderbufferObjectAttachment::getHeight() const
	{
		return _height;
	}

	void RenderbufferObjectAttachment::initialize(Framebuffer* framebuffer, uint32 index)
	{
		glGenRenderbuffers(1, &_handle);

		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer->getHandle());

		glBindRenderbuffer(GL_RENDERBUFFER, _handle);
		glRenderbufferStorage(GL_RENDERBUFFER, _format, _width, _height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, GL_RENDERBUFFER, _handle);

		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (status != GL_FRAMEBUFFER_COMPLETE)
		{
			LOGF(Error, CoreEngine, "Framebuffer status incomplete after creating renderbuffer object attachment: 0x%x", status);
		}

		Framebuffer::bind(Framebuffer::getCurrentFramebuffer());
	}
}
