#include "VertexFormat.h"

namespace Arcana
{
	VertexFormat::VertexFormat() : _vertexSize(0), _indexOffset(0)
	{
	}
		
	VertexFormat::VertexFormat(uint32 numAttributes, const Attribute* attributes, uint32 indexOffset) : _vertexSize(0), _indexOffset(indexOffset)
	{
		if(attributes)
		{
			for(uint32 i = 0; i < numAttributes; i++)
			{
				Attribute attribute;
				memcpy(&attribute, &attributes[i], sizeof(Attribute));
				addAttribute(attribute);
			}
		}
	}
		
	VertexFormat::~VertexFormat()
	{
	}
		
	const VertexFormat::Attribute& VertexFormat::getAttribute(uint32 index) const
	{
		if(index < _attributes.size())
		{
			return _attributes[index];
		}
		return Attribute();
	}
		
	void VertexFormat::addAttribute(const Attribute& attribute)
	{
		_attributes.push_back(attribute);
		_vertexSize += attribute.getSize() * sizeof(float);
	}
		
	uint32 VertexFormat::getNumAttributes() const
	{
		return _attributes.size();
	}
		
	uint32 VertexFormat::getVertexSize() const
	{
		return _vertexSize;
	}

	uint32 VertexFormat::getIndexOffset() const
	{
		return _indexOffset;
	}
		
		
	bool VertexFormat::operator == (const VertexFormat& f) const
	{
		if (getNumAttributes() != f.getNumAttributes())
		{
			return false;
		}

		for (size_t i = 0; i < getNumAttributes(); i++)
		{
			if (_attributes[i] != f._attributes[i])
			{
				return false;
			}
		}

		return true;
	}

	bool VertexFormat::operator != (const VertexFormat& f) const
	{
		return !(*this == f);
	}
	
	
	
	VertexFormat::Attribute::Attribute() : _type(Position), _size(0)
	{
	}
	
	VertexFormat::Attribute::Attribute(Semantic type, uint32 size) : _type(type), _size(size)
	{
	}
			
	VertexFormat::Semantic VertexFormat::Attribute::getType() const
	{
		return _type;
	}
			
	uint32 VertexFormat::Attribute::getSize() const
	{
		return _size;
	}
			
	bool VertexFormat::Attribute::operator==(const Attribute& c) const
	{
		return _type == c._type && _size == c._size;
	}
			
	bool VertexFormat::Attribute::operator!=(const Attribute& c) const
	{
		return !(*this == c);
	}
}