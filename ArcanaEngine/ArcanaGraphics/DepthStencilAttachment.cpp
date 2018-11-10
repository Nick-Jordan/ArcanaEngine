#include "DepthStencilAttachment.h"

namespace Arcana
{
	DepthStencilAttachment::DepthStencilAttachment(const std::string& id, Format format, uint32 width, uint32 height)
		: RenderbufferObjectAttachment(id, GL_DEPTH24_STENCIL8, width, height), _packed(false)
	{
	}

	DepthStencilAttachment::~DepthStencilAttachment()
	{
		if (_stencilHandle)
		{
			glDeleteRenderbuffers(1, &_stencilHandle);
		}
	}

	uint32 DepthStencilAttachment::getWidth() const
	{
		return RenderbufferObjectAttachment::getWidth();
	}

	uint32 DepthStencilAttachment::getHeight() const
	{
		return RenderbufferObjectAttachment::getHeight();
	}

	bool DepthStencilAttachment::isPacked() const
	{
		return _packed;
	}

	void DepthStencilAttachment::initialize(Framebuffer* framebuffer, uint32 index)
	{
		glGenRenderbuffers(1, &_handle);
		glBindRenderbuffer(GL_RENDERBUFFER, _handle);

		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, getWidth(), getHeight());

		GLenum error = glGetError();
		if (error != GL_NO_ERROR)
		{
			LOGF(Error, CoreEngine, "Error %d", error);

			const char* extString = (const char*)glGetString(GL_EXTENSIONS);

			if (strstr(extString, "GL_OES_packed_depth_stencil") != 0)
			{
				glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, getWidth(), getHeight());
				_packed = true;
			}
			else
			{
				if (strstr(extString, "GL_OES_depth24") != 0)
				{
					glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, getWidth(), getHeight());
				}
				else
				{
					glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, getWidth(), getHeight());
				}
				if (_format == DepthStencil)
				{
					glGenRenderbuffers(1, &_stencilHandle);
					glBindRenderbuffer(GL_RENDERBUFFER, _stencilHandle);
					glRenderbufferStorage(GL_RENDERBUFFER, GL_STENCIL_INDEX8, getWidth(), getHeight());
				}
			}
		}
		else
		{
			_packed = true;
		}

		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer->getHandle());

		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _handle);
		if (isPacked())
		{
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _handle);
		}
		else if (_format == DepthStencil)
		{
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _stencilHandle);
		}

		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (status != GL_FRAMEBUFFER_COMPLETE)
		{
			LOGF(Error, CoreEngine, "Framebuffer status incomplete after creating depth/stencil attachment: 0x%x", status);
		}

		Framebuffer::bind(Framebuffer::getCurrentFramebuffer());
	}
}
