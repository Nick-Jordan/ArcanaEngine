#ifndef MESH_INDEX_COMPONENT_H_
#define MESH_INDEX_COMPONENT_H_

#include "GraphicsDefines.h"

#include "Object.h"
#include "Mesh.h"
#include "IndexBuffer.h"

namespace Arcana
{
	class ARCANA_GRAPHICS_API MeshIndexComponent : public Object
	{
	public:

		MeshIndexComponent(Mesh* parent, uint32 meshIndex, Mesh::Primitive primitive);

		virtual ~MeshIndexComponent();

		Mesh::Primitive getPrimitive() const;

		IndexBuffer::Format getIndexFormat() const;

		IndexBuffer* setIndexBuffer(IndexBuffer::Format indexFormat, uint32 indexCount, bool dynamic, void* indexPointer);

		IndexBuffer* getIndexBuffer();

		int32 getNumIndices() const;

		bool isDynamic() const;

	private:

		Mesh* _parent;
		uint32 _meshIndex;
		Mesh::Primitive _primitiveType;
		IndexBuffer::Format _indexFormat;
		IndexBuffer* _indexBuffer;
	};
}

#endif // !MESH_INDEX_COMPONENT_H_

