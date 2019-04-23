#include "ImmediateRenderer.h"

#include "ArcanaMath.h"
#include "MeshIndexComponent.h"
#include "Vector4.h"

namespace Arcana
{

	ImmediateRenderer::ImmediateRenderer(const VertexFormat& vertexFormat, Mesh::Primitive primitiveType, bool indexed, uint32 initialCapacity, uint32 growSize)
		: _vertexFormat(vertexFormat), _primitiveType(primitiveType), _indexed(indexed), _capacity(0), _growSize(growSize),
		_vertexCapacity(0), _indexCapacity(0), _vertexCount(0), _indexCount(0), _vertices(NULL), _verticesPtr(NULL), _indices(NULL), _indicesPtr(NULL), _started(false),
		_mesh(nullptr)
	{
		resize(initialCapacity);

		//std::vector<Vector4f> verts = { Vector4f(0.0, 0.0, 0.0, 1.0), Vector4f(1.0, 0.0, 0.0, 1.0), Vector4f(1.0, 1.0, 0.0, 1.0) };
	}

	ImmediateRenderer::~ImmediateRenderer()
	{
		AE_DELETE(_mesh);

		AE_DELETE_ARRAY(_vertices);
		AE_DELETE_ARRAY(_indices);
	}

	void ImmediateRenderer::add(const void* vertices, size_t size, uint32 vertexCount, const uint16* indices, uint32 indexCount)
	{
		AE_ASSERT(vertices);

		uint32 newVertexCount = _vertexCount + vertexCount;
		uint32 newIndexCount = _indexCount + indexCount;
		if (_primitiveType == Mesh::TriangleStrip && _vertexCount > 0)
		{
			newIndexCount += 2;
		}

		while (newVertexCount > _vertexCapacity || (_indexed && newIndexCount > _indexCapacity))
		{
			if (_growSize == 0)
				return;
			if (!resize(_capacity + _growSize))
				return;
		}

		AE_ASSERT(_verticesPtr);
		uint32 vBytes = vertexCount * _vertexFormat.getVertexSize();
		memcpy(_verticesPtr, vertices, vBytes);

		if (_indexed)
		{
			AE_ASSERT(indices);
			AE_ASSERT(_indicesPtr);

			if (_vertexCount == 0)
			{
				memcpy(_indicesPtr, indices, indexCount * sizeof(uint16));
			}
			else
			{
				if (_primitiveType == Mesh::TriangleStrip)
				{
					_indicesPtr[0] = *(_indicesPtr - 1);
					_indicesPtr[1] = _vertexCount;
					_indicesPtr += 2;
				}

				for (uint32 i = 0; i < indexCount; ++i)
				{
					_indicesPtr[i] = indices[i] + _vertexCount;
				}
			}
			_indicesPtr += indexCount;
			_indexCount = newIndexCount;
		}

		_verticesPtr += vBytes;
		_vertexCount = newVertexCount;
	}

	uint32 ImmediateRenderer::getCapacity() const
	{
		return _capacity;
	}

	void ImmediateRenderer::setCapacity(uint32 capacity)
	{
		resize(capacity);
	}

	bool ImmediateRenderer::resize(uint32 capacity)
	{
		if (capacity == 0)
		{
			LOG(Warning, CoreEngine, "Invalid resize capacity: 0.");
			return false;
		}

		if (capacity == _capacity)
			return true;

		uint8* oldVertices = _vertices;
		uint16* oldIndices = _indices;

		uint32 vertexCapacity = 0;
		switch (_primitiveType)
		{
		case Mesh::Lines:
			vertexCapacity = capacity * 2;
			break;
		case Mesh::LineStrip:
			vertexCapacity = capacity + 1;
			break;
		case Mesh::Points:
			vertexCapacity = capacity;
			break;
		case Mesh::Triangles:
			vertexCapacity = capacity * 3;
			break;
		case Mesh::TriangleStrip:
			vertexCapacity = capacity + 2;
			break;
		default:
			LOGF(Warning, CoreEngine, "Unsupported primitive type, %d, for mesh batch.", _primitiveType);
			return false;
		}

		uint32 indexCapacity = vertexCapacity;
		if (_indexed && indexCapacity > USHRT_MAX)
		{
			LOGF(Warning, CoreEngine, "Index capacity is greater than the maximum uint16 value (%d > %d).", indexCapacity, USHRT_MAX);
			return false;
		}

		AE_DELETE(_mesh);

		_mesh = new Mesh(_vertexFormat, _primitiveType);
		_mesh->setVertexBuffer(_vertexFormat, capacity);// ->setVertexData(&verts[0], 0, verts.size());

		uint32 voffset = _verticesPtr - _vertices;
		uint32 vBytes = vertexCapacity * _vertexFormat.getVertexSize();
		_vertices = new uint8[vBytes];
		if (voffset >= vBytes)
		{
			voffset = vBytes - 1;
		}
		_verticesPtr = _vertices + voffset;

		if (_indexed)
		{
			uint32 ioffset = _indicesPtr - _indices;
			_indices = new uint16[indexCapacity];
			if (ioffset >= indexCapacity)
			{
				ioffset = indexCapacity - 1;
			}
			_indicesPtr = _indices + ioffset;

			MeshIndexComponent* comp = _mesh->addIndexComponent(_primitiveType);
			comp->setIndexBuffer(IndexBuffer::Index16, indexCapacity, true, nullptr);
		}

		if (oldVertices)
		{
			memcpy(_vertices, oldVertices, Math::min(_vertexCapacity, vertexCapacity) * _vertexFormat.getVertexSize());
		}

		AE_DELETE_ARRAY(oldVertices);

		if (oldIndices)
		{
			memcpy(_indices, oldIndices, Math::min(_indexCapacity, indexCapacity) * sizeof(uint16));
		}

		AE_DELETE_ARRAY(oldIndices);

		_capacity = capacity;
		_vertexCapacity = vertexCapacity;
		_indexCapacity = indexCapacity;

		return true;
	}

	void ImmediateRenderer::add(const float* vertices, uint32 vertexCount, const uint16* indices, uint32 indexCount)
	{
		add(vertices, sizeof(float), vertexCount, indices, indexCount);
	}

	void ImmediateRenderer::start()
	{
		_vertexCount = 0;
		_indexCount = 0;
		_verticesPtr = _vertices;
		_indicesPtr = _indices;
		_started = true;
	}

	bool ImmediateRenderer::isStarted() const
	{
		return _started;
	}

	void ImmediateRenderer::finish()
	{
		_started = false;
	}

	Mesh* ImmediateRenderer::draw()
	{
		_mesh->getVertexBuffer()->setVertexData(_vertices, 0, _vertexCount);

		if (_indexed)
		{
			MeshIndexComponent* comp = _mesh->getIndexComponent(0);

			comp->getIndexBuffer()->setIndexData(_indices, 0, _indexCount);
		}

		return _mesh;
	}

}