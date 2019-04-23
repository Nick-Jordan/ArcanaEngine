#include "VertexBuffer.h"

namespace Arcana
{
	VertexBuffer::VertexBuffer(const VertexFormat& vertexFormat, uint32 vertexCount, bool dynamic, void* vertexPointer) 
		: _vertexCount(vertexCount), _dynamic(dynamic), _bound(false), _format(vertexFormat)
	{
		initialize(vertexFormat, vertexPointer);
	}
		
	VertexBuffer::~VertexBuffer()
	{
		finalize();
	}
	
	const VertexFormat& VertexBuffer::getVertexFormat() const
	{
		return _format;
	}

	uint32 VertexBuffer::getVertexCount() const
	{
		return _vertexCount;
	}
		
	bool VertexBuffer::isDynamic() const
	{
		return _dynamic;
	}
	
	bool VertexBuffer::isBound() const
	{
		return _bound;
	}
	
	void VertexBuffer::setVertexData(const void* vertexData, uint32 vertexStart, uint32 vertexCount)
	{
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);

		if (vertexStart == 0 && vertexCount == 0)
		{
			if(_vertexCount == 0)
			{
				LOG(Warning, CoreEngine, "Vertex data set with vertex count of 0");
			}
			glBufferData(GL_ARRAY_BUFFER, _format.getVertexSize() * _vertexCount, vertexData, _dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
		}
		else
		{
			if (vertexCount == 0)
			{
				if(_vertexCount == 0)
				{
					LOG(Warning, CoreEngine, "Vertex data set with vertex count of 0");
				}
				vertexCount = _vertexCount - vertexStart;
			}

			glBufferSubData(GL_ARRAY_BUFFER, vertexStart * _format.getVertexSize(), vertexCount * _format.getVertexSize(), vertexData);
		}
	}
		
	bool VertexBuffer::bind() // first vertex as argument?
	{
		if(_vbo)
		{
			_bound = true;
			
			glBindBuffer(GL_ARRAY_BUFFER, _vbo);
			
			bindVertexAttributes();
			
			return true;
		}
		
		return false;
	}
		
	void VertexBuffer::unbind()
	{
		if(_bound)
		{
			_bound = false;
				
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			
			unbindVertexAttributes();
		}
	}
	
	void VertexBuffer::bindVertexAttributes()
	{
		for(auto i = _attributeBindings.createConstIterator(); i; i++)
		{
			const AttributeBinding& attr = *i;
			
			if(attr.enabled)
			{
				glVertexAttribPointer(attr.index, attr.size, attr.type, attr.normalized, attr.stride, attr.pointer);
				glEnableVertexAttribArray(attr.index);
			}
		}
	}
	
	void VertexBuffer::unbindVertexAttributes()
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
	
	void* VertexBuffer::map()
	{
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);

		return (void*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	}

	bool VertexBuffer::unmap()
	{
		return glUnmapBuffer(GL_ARRAY_BUFFER);
	}
	
	void VertexBuffer::initialize(const VertexFormat& vertexFormat, void* vertexPointer)
	{
		glGenBuffers(1, &_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		glBufferData(GL_ARRAY_BUFFER, vertexFormat.getVertexSize() * _vertexCount, vertexPointer, _dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);	
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		
		int32 offset = 0;
		
		for(uint32 i = 0; i < vertexFormat.getNumAttributes(); i++)
		{
			const VertexFormat::Attribute& attr = vertexFormat.getAttribute(i);
		
			void* pointer = vertexPointer ? (void*)(((uint8*)vertexPointer) + offset) : (void*)offset;
			
			
			AttributeBinding binding;
			binding.enabled = true;
			binding.index = i;
			binding.size = (GLuint)attr.getSize();
			binding.type = GL_FLOAT;
			binding.normalized = GL_FALSE;
			binding.stride = (GLsizei)vertexFormat.getVertexSize();
			binding.pointer = pointer;

			offset += attr.getSize() * sizeof(float);
			
			_attributeBindings.add(binding);
		
		}
	}
	
	void VertexBuffer::finalize()
	{
		unbind();
		
		if (_vbo)
		{
			glDeleteBuffers(1, &_vbo);
			_vbo = 0;
		}
	}
}