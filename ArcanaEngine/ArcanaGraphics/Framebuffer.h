#ifndef FRAMEBUFFER_H_
#define FRAMEBUFFER_H_

#include "GraphicsDefines.h"

#include "Object.h"
#include "ArcanaLog.h"
#include "Array.h"
#include "../Dependencies/include/opengl/include.h"

namespace Arcana
{

	class ARCANA_GRAPHICS_API Framebuffer : public Object
	{
	public:

		enum Target
		{
			TargetDrawFramebuffer = GL_DRAW_FRAMEBUFFER,
			TargetReadFramebuffer = GL_READ_FRAMEBUFFER,
			TargetFramebuffer = GL_FRAMEBUFFER
		};

		enum Buffer
		{
			Unknown = -1,

			None = GL_NONE,
			Color0 = GL_COLOR_ATTACHMENT0,
			Color1 = GL_COLOR_ATTACHMENT1,
			Color2 = GL_COLOR_ATTACHMENT2,
			Color3 = GL_COLOR_ATTACHMENT3,
			Color4 = GL_COLOR_ATTACHMENT4,
			Color5 = GL_COLOR_ATTACHMENT5,
			Color6 = GL_COLOR_ATTACHMENT6,
			Color7 = GL_COLOR_ATTACHMENT7,
			Color8 = GL_COLOR_ATTACHMENT8,
			Color9 = GL_COLOR_ATTACHMENT9,
			Color10 = GL_COLOR_ATTACHMENT10,
			Color11 = GL_COLOR_ATTACHMENT11,
			Color12 = GL_COLOR_ATTACHMENT12,
			Color13 = GL_COLOR_ATTACHMENT13,
			Color14 = GL_COLOR_ATTACHMENT14,
			Color15 = GL_COLOR_ATTACHMENT15,
			Depth = GL_DEPTH_ATTACHMENT,
			Stencil = GL_STENCIL_ATTACHMENT
		};

		class ARCANA_GRAPHICS_API Attachment : public Object
		{
		public:

			Attachment(const std::string& id);

			~Attachment();

			const std::string& getId() const;

			GLuint getHandle() const;

			virtual uint32 getWidth() const = 0;

			virtual uint32 getHeight() const = 0;

			virtual void initialize(Framebuffer* framebuffer, uint32 index) = 0;

		protected:

			GLuint _handle;

		private:

			std::string _id;
		};

		Framebuffer(const std::string& id);

		virtual ~Framebuffer();

		const std::string& getId() const;

		uint32 getWidth() const;

		uint32 getHeight() const;

		GLuint getHandle() const;

		void addAttachment(Attachment* attachment);

		void removeAttachment(const std::string& id);

		void setDrawBuffers(const GLenum* buffers, uint32 numBuffers = 0);

		void setDrawBuffer(Buffer buffer);

		Framebuffer* setReadBuffer(Buffer buffer);

		Framebuffer* bind(Target target = TargetFramebuffer);

		void readPixels(Buffer readBuffer, int32 x, int32 y, int64 width, int64 height, GLenum format, GLenum type, void *pixels);

		static Framebuffer* bind(Framebuffer* framebuffer, Target target = TargetFramebuffer);

		static Framebuffer* get(const std::string& id);

		static Framebuffer* getCurrentFramebuffer();

		static uint32 getMaxColorAttachments();

	private:

		Array<Attachment*> _attachments;
		std::string _id;
		GLuint _handle;
		uint32 _width;
		uint32 _height;

		static Array<Framebuffer*> __framebufferCache;
		static Framebuffer* __currentFrameBuffer;
	};

}

#endif // !FRAMEBUFFER_H_

