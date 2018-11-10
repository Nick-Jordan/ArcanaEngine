#ifndef RENDERBUFFER_OBJECT_ATTACHMENT_H_
#define RENDERBUFFER_OBJECT_ATTACHMENT_H_

#include "GraphicsDefines.h"

#include "Framebuffer.h"

namespace Arcana
{

	class ARCANA_GRAPHICS_API RenderbufferObjectAttachment : public Framebuffer::Attachment
	{
	public:

		RenderbufferObjectAttachment(const std::string& id, GLenum internalFormat, uint32 width, uint32 height);

		virtual ~RenderbufferObjectAttachment();
	
		virtual GLenum getFormat() const;

		virtual uint32 getWidth() const override;

		virtual uint32 getHeight() const override;

		virtual void initialize(Framebuffer* framebuffer, uint32 index) override;

	private:

		GLenum _format;

		uint32 _width;
		uint32 _height;
	};

}

#endif // !RENDERBUFFER_OBJECT_ATTACHMENT_H_

