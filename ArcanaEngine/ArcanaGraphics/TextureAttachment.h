#ifndef TEXTURE_ATTACHMENT_H_
#define TEXTURE_ATTACHMENT_H_

#include "GraphicsDefines.h"

#include "Framebuffer.h"
#include "Texture.h"

namespace Arcana
{

	class ARCANA_GRAPHICS_API TextureAttachment : public Framebuffer::Attachment
	{
	public:

		TextureAttachment(const std::string& id, Texture* texture);

		TextureAttachment(const std::string& id, Texture* texture, Texture::CubeFace face);

		virtual ~TextureAttachment();

		Texture* getTexture() const;

		virtual uint32 getWidth() const override;

		virtual uint32 getHeight() const override;

		virtual void initialize(Framebuffer* framebuffer, uint32 index) override;

	private:

		Texture* _texture;
		Texture::CubeFace _face;
	};

}

#endif // !TEXTURE_ATTACHMENT_H_
