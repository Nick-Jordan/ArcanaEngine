#ifndef IMMEDIATE_RENDERER_H_
#define IMMEDIATE_RENDERER_H_

#include "GraphicsDefines.h"

#include "Object.h"
#include "Mesh.h"

namespace Arcana
{

	class ARCANA_GRAPHICS_API ImmediateRenderer
	{
	public:

		ImmediateRenderer(const VertexFormat& vertexFormat, Mesh::Primitive primitiveType, bool indexed, uint32 initialCapacity = 1024, uint32 growSize = 1024);

		ImmediateRenderer(const ImmediateRenderer& copy);

		~ImmediateRenderer();

		uint32 getCapacity() const;

		void setCapacity(uint32 capacity);

		template <class T>
		void add(const T* vertices, uint32 vertexCount, const uint16* indices = nullptr, uint32 indexCount = 0);

		void add(const float* vertices, uint32 vertexCount, const uint16* indices = nullptr, uint32 indexCount = 0);

		void start();

		bool isStarted() const;

		void finish();

		Mesh* draw();

	private:

		void add(const void* vertices, size_t size, uint32 vertexCount, const uint16* indices, uint32 indexCount);

		bool resize(uint32 capacity);

		const VertexFormat _vertexFormat;
		Mesh::Primitive _primitiveType;
		bool _indexed;
		uint32 _capacity;
		uint32 _growSize;
		uint32 _vertexCapacity;
		uint32 _indexCapacity;
		uint32 _vertexCount;
		uint32 _indexCount;
		uint8* _vertices;
		uint8* _verticesPtr;
		uint16* _indices;
		uint16* _indicesPtr;
		bool _started;

		Mesh* _mesh;
	};

	template <class T>
	void ImmediateRenderer::add(const T* vertices, uint32 vertexCount, const uint16* indices, uint32 indexCount)
	{
		AE_ASSERT(sizeof(T) == _vertexFormat.getVertexSize());
		add(vertices, sizeof(T), vertexCount, indices, indexCount);
	}
}

#endif // !IMMEDIATE_RENDERER_H_

