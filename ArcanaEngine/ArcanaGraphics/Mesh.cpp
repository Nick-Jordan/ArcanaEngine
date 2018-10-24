#include "Mesh.h"

namespace Arcana
{
	Mesh::Mesh(const VertexFormat& vertexFormat, Primitive primitive) : _vertexFormat(vertexFormat), _primitive(primitive), _vertexBuffer(nullptr)
	{
	}
		
	Mesh::~Mesh()
	{
		AE_DELETE(_vertexBuffer);
	}
		
	const VertexFormat& Mesh::getVertexFormat() const
	{
		return _vertexFormat;
	}
	
	void Mesh::setPrimitive(Primitive primitive)
	{
		_primitive = primitive;
	}
		
	Mesh::Primitive Mesh::getPrimitive() const
	{
		return _primitive;
	}
	
	uint32 Mesh::getVertexSize() const
	{
		return _vertexFormat.getVertexSize();
	}
	
	void Mesh::setVertexBuffer(VertexBuffer* vertexBuffer)
	{
		_vertexBuffer = vertexBuffer;
	}
		
	VertexBuffer* Mesh::getVertexBuffer()
	{
		return _vertexBuffer;
	}
	
	int32 Mesh::getNumVertices() const
	{
		if(_vertexBuffer)
		{
			return _vertexBuffer->getVertexCount();
		}
		return 0;
	}
		
	bool Mesh::isDynamic() const
	{
		if(_vertexBuffer)
		{
			return _vertexBuffer->isDynamic();
		}
		return false;
	}

	uint32 Mesh::getNumIndexComponents() const
	{
		return 0; //temp
	}
}