#include "IndexBuffer.h"
#include "ArcanaLog.h"

namespace Arcana
{
	IndexBuffer::IndexBuffer(Format indexFormat, uint32 indexCount, bool dynamic, void* indexPointer) 
		: _indexCount(indexCount), _dynamic(dynamic), _bound(false), _format(indexFormat)
	{
		initialize(indexFormat, indexPointer);
	}
		
	IndexBuffer::~IndexBuffer()
	{
		finalize();
	}

	IndexBuffer::Format IndexBuffer::getIndexFormat() const
	{
		return _format;
	}
	
	uint32 IndexBuffer::getIndexCount() const
	{
		return _indexCount;
	}
		
	bool IndexBuffer::isDynamic() const
	{
		return _dynamic;
	}
	
	bool IndexBuffer::isBound() const
	{
		return _bound;
	}
	
	void IndexBuffer::setIndexData(const void* indexData, uint32 indexStart, uint32 indexCount)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);

		if (indexStart == 0 && indexCount == 0)
		{
			if(_indexCount == 0)
			{
				LOG(Warning, CoreEngine, "Index data set with index count of 0");
			}
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, getFormatSize(_format) * _indexCount, indexData, _dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
		}
		else
		{
			if (indexCount == 0)
			{
				if(_indexCount == 0)
				{
					LOG(Warning, CoreEngine, "Index data set with index count of 0");
				}
				indexCount = _indexCount - indexStart;
			}

			glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, indexStart * getFormatSize(_format), indexCount * getFormatSize(_format), indexData);
		}
	}
		
	bool IndexBuffer::bind() // first index as argument?
	{
		if(_ibo)
		{
			_bound = true;
			
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
						
			return true;
		}
		
		return false;
	}
		
	void IndexBuffer::unbind()
	{
		if(_bound)
		{
			_bound = false;
				
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}
	}
	
	void* IndexBuffer::map()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);

		return (void*)glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);
	}

	bool IndexBuffer::unmap()
	{
		return glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
	}
	
	int32 IndexBuffer::getFormatSize(Format format)
	{
		switch(format)
		{
		case Index8:
			return 1;
		case Index16:
			return 2;
		case Index32:
			return 4;
		};
		return 0;
	}
	
	void IndexBuffer::initialize(Format indexFormat, void* indexPointer)
	{
		glGenBuffers(1, &_ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, getFormatSize(indexFormat) * _indexCount, indexPointer, _dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	
	void IndexBuffer::finalize()
	{
		unbind();
		
		if (_ibo)
		{
			glDeleteBuffers(1, &_ibo);
			_ibo = 0;
		}
	}
}