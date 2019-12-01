#include "Framebuffer.h"

#include <algorithm>

namespace Arcana
{
	Array<Framebuffer*> Framebuffer::__framebufferCache = Array<Framebuffer*>();
	Framebuffer* Framebuffer::__currentFrameBuffer = nullptr;

	Framebuffer::Framebuffer(const std::string& id) : _id(id)
	{
		glGenFramebuffers(1, &_handle);

		__framebufferCache.add(this);
	}

	Framebuffer::~Framebuffer()
	{
		for (auto iter = _attachments.createIterator(); iter; iter++)
		{
			AE_RELEASE(*iter);
		}

		_attachments.clear();

		if (_handle)
		{
			glDeleteFramebuffers(1, &_handle);
		}

		__framebufferCache.remove(this);
	}

	const std::string& Framebuffer::getId() const
	{
		return _id;
	}

	uint32 Framebuffer::getWidth() const
	{
		return _width;
	}

	uint32 Framebuffer::getHeight() const
	{
		return _height;
	}

	GLuint Framebuffer::getHandle() const
	{
		return _handle;
	}

	void Framebuffer::addAttachment(Attachment* attachment)
	{
		if (attachment)
		{
			attachment->reference();
			attachment->initialize(this, _attachments.size());
			_attachments.add(attachment);
		}
	}

	void Framebuffer::removeAttachment(const std::string& id)
	{
		int32 index = _attachments.indexOfByPredicate([&](Attachment* attachment) {return attachment->getId() == id; });

		if (index != -1)
		{
			_attachments.removeAt(index);
		}
	}

	void Framebuffer::setDrawBuffers(const GLenum* buffers, uint32 numBuffers)
	{
		Framebuffer* prev = bind();

		if (!numBuffers)
		{
			numBuffers = _attachments.size();
		}

		glDrawBuffers(numBuffers, buffers);

		Framebuffer::bind(prev);
	}

	void Framebuffer::setDrawBuffer(Buffer buffer)
	{
		Framebuffer* prev = bind();

		glDrawBuffer(buffer);

		Framebuffer::bind(prev);
	}

	Framebuffer* Framebuffer::setReadBuffer(Buffer buffer)
	{
		Framebuffer* prev = bind();

		glReadBuffer(buffer);

		return prev;
	}

	Framebuffer* Framebuffer::bind(Target target)
	{
		if (__currentFrameBuffer == this)
			return this;

		glBindFramebuffer(target, _handle);

		Framebuffer* previous = __currentFrameBuffer;
		__currentFrameBuffer = this;
		return previous;
	}

	void Framebuffer::readPixels(Buffer readBuffer, int32 x, int32 y, int64 width, int64 height, GLenum format, GLenum type, void *pixels)
	{
		Framebuffer* prev = setReadBuffer(readBuffer);

		glReadPixels(x, y, width, height, format, type, pixels);

		Framebuffer::bind(prev);
	}

	Framebuffer* Framebuffer::bind(Framebuffer* framebuffer, Target target)
	{
		if (framebuffer == __currentFrameBuffer)
			return framebuffer;

		if (framebuffer)
		{
			glBindFramebuffer(target, framebuffer->_handle);
		}
		else
		{
			glBindFramebuffer(target, 0);
		}

		Framebuffer* previous = __currentFrameBuffer;
		__currentFrameBuffer = framebuffer;
		return previous;
	}

	Framebuffer* Framebuffer::get(const std::string& id)
	{
		return *__framebufferCache.findByPredicate([&](Framebuffer* buffer) { return buffer->getId() == id; });
	}

	Framebuffer* Framebuffer::getCurrentFramebuffer()
	{
		return __currentFrameBuffer;
	}

	uint32 Framebuffer::getMaxColorAttachments()
	{
		static uint32 __maxColorAttachments = 0;

		if (!__maxColorAttachments)
		{
			GLint value;
			glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &value);
			__maxColorAttachments = (uint32)(std::max)(1, value);
		}

		return __maxColorAttachments;
	}


	Framebuffer::Attachment::Attachment(const std::string& id) : _id(id)
	{

	}

	Framebuffer::Attachment::~Attachment()
	{

	}

	const std::string& Framebuffer::Attachment::getId() const
	{
		return _id;
	}

	GLuint Framebuffer::Attachment::getHandle() const
	{
		return _handle;
	}
}
