#include "InstanceBuffer.h"

namespace Arcana
{
	InstanceBuffer::InstanceBuffer(const VertexFormat& format, uint32 instanceCount, bool dynamic, void* dataPointer)
		: _instanceCount(instanceCount), _dynamic(dynamic), _bound(false), _format(format)
	{
		initialize(format, dataPointer);
	}

	InstanceBuffer::~InstanceBuffer()
	{
		finalize();
	}

	const VertexFormat& InstanceBuffer::getFormat() const
	{
		return _format;
	}

	uint32 InstanceBuffer::getInstanceCount() const
	{
		return _instanceCount;
	}

	bool InstanceBuffer::isDynamic() const
	{
		return _dynamic;
	}

	bool InstanceBuffer::isBound() const
	{
		return _bound;
	}

	void InstanceBuffer::setInstanceData(const void* dataPointer, uint32 start, uint32 count)
	{
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);

		if (start == 0 && count == 0)
		{
			if (_instanceCount == 0)
			{
				LOG(Warning, CoreEngine, "Instance data set with instance count of 0");
			}
			glBufferData(GL_ARRAY_BUFFER, _format.getVertexSize() * _instanceCount, dataPointer, _dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
		}
		else
		{
			if (count == 0)
			{
				if (_instanceCount == 0)
				{
					LOG(Warning, CoreEngine, "Instance data set with instance count of 0");
				}
				count = _instanceCount - start;
			}

			glBufferSubData(GL_ARRAY_BUFFER, start * _format.getVertexSize(), count * _format.getVertexSize(), dataPointer);
		}
	}

	bool InstanceBuffer::bind() // first vertex as argument?
	{
		if (_vbo)
		{
			_bound = true;

			glBindBuffer(GL_ARRAY_BUFFER, _vbo);

			bindAttributeDivisors();

			return true;
		}

		return false;
	}

	void InstanceBuffer::unbind()
	{
		if (_bound)
		{
			_bound = false;

			glBindBuffer(GL_ARRAY_BUFFER, 0);

			unbindAttributeDivisors();
		}
	}

	void InstanceBuffer::bindAttributeDivisors()
	{
		for (auto i = _attributeBindings.createConstIterator(); i; i++)
		{
			const AttributeBinding& attr = *i;

			if (attr.enabled)
			{
				glVertexAttribPointer(attr.index, attr.size, attr.type, attr.normalized, attr.stride, attr.pointer);
				glEnableVertexAttribArray(attr.index);
				glVertexAttribDivisor(attr.index, attr.divisor);
			}
		}
	}

	void InstanceBuffer::unbindAttributeDivisors()
	{
		for (auto i = _attributeBindings.createConstIterator(); i; i++)
		{
			const AttributeBinding& attr = *i;

			if (attr.enabled)
			{
				glDisableVertexAttribArray(attr.index);
			}
		}
	}

	void* InstanceBuffer::map()
	{
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);

		return (void*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	}

	bool InstanceBuffer::unmap()
	{
		return glUnmapBuffer(GL_ARRAY_BUFFER);
	}

	void InstanceBuffer::initialize(const VertexFormat& format, void* dataPointer)
	{
		glGenBuffers(1, &_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		glBufferData(GL_ARRAY_BUFFER, format.getVertexSize() * _instanceCount, dataPointer, _dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		int32 offset = 0;

		for (uint32 i = 0; i < format.getNumAttributes(); i++)
		{
			const VertexFormat::Attribute& attr = format.getAttribute(i);

			void* pointer = dataPointer ? (void*)(((uint8*)dataPointer) + offset) : (void*)offset;

			AttributeBinding binding;
			binding.divisor = 1;   ////////////////////CHANGE///////////////////////////////
			binding.enabled = true;
			binding.index = i + format.getIndexOffset();
			binding.size = (GLuint)attr.getSize();
			binding.type = GL_FLOAT;
			binding.normalized = GL_FALSE;
			binding.stride = (GLsizei)format.getVertexSize();
			binding.pointer = pointer;

			offset += attr.getSize() * sizeof(float);

			_attributeBindings.add(binding);

		}
	}

	void InstanceBuffer::finalize()
	{
		unbind();

		if (_vbo)
		{
			glDeleteBuffers(1, &_vbo);
			_vbo = 0;
		}
	}
}