#ifndef DEPTH_STENCIL_ATTACHMENT_H_
#define DEPTH_STENCIL_ATTACHMENT_H_

#include "GraphicsDefines.h"

#include "RenderbufferObjectAttachment.h"

namespace Arcana
{

	class ARCANA_GRAPHICS_API DepthStencilAttachment : public RenderbufferObjectAttachment
	{
	public:

		enum Format
		{
			Depth,
			DepthStencil
		};

		DepthStencilAttachment(const std::string& id, Format target, uint32 width, uint32 height);

		virtual ~DepthStencilAttachment();

		virtual uint32 getWidth() const override;

		virtual uint32 getHeight() const override;

		bool isPacked() const;

		virtual void initialize(Framebuffer* framebuffer, uint32 index) override;

	private:

		Format _format;
		GLuint _stencilHandle;
		bool _packed;
	};

}

#endif // !DEPTH_STENCIL_ATTACHMENT_H_

