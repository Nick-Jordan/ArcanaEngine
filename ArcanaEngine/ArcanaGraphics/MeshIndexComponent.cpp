#include "MeshIndexComponent.h"

#include "CoreDefines.h"

namespace Arcana
{

	MeshIndexComponent::MeshIndexComponent(Mesh* parent, uint32 meshIndex, Mesh::Primitive primitive) : Object("MeshIndexComponent"),
		_parent(parent), _meshIndex(meshIndex), _primitiveType(primitive), _indexBuffer(nullptr)
	{
	}


	MeshIndexComponent::~MeshIndexComponent()
	{
		AE_DELETE(_indexBuffer);
	}

	Mesh::Primitive MeshIndexComponent::getPrimitive() const
	{
		return _primitiveType;
	}

	IndexBuffer::Format MeshIndexComponent::getIndexFormat() const
	{
		return _indexFormat;
	}

	IndexBuffer* MeshIndexComponent::setIndexBuffer(IndexBuffer::Format indexFormat, uint32 indexCount, bool dynamic, void* indexPointer)
	{
		_indexBuffer = new IndexBuffer(indexFormat, indexCount, dynamic, indexPointer);

		_indexFormat = indexFormat;

		return _indexBuffer;
	}

	IndexBuffer* MeshIndexComponent::getIndexBuffer()
	{
		return _indexBuffer;
	}

	int32 MeshIndexComponent::getNumIndices() const
	{
		if (_indexBuffer)
		{
			return _indexBuffer->getIndexCount();
		}
		return 0;
	}

	bool MeshIndexComponent::isDynamic() const
	{
		if (_indexBuffer)
		{
			return _indexBuffer->isDynamic();
		}
		return false;
	}
}
