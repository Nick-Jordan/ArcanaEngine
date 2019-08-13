#include "Mesh.h"

#include "MeshIndexComponent.h"

namespace Arcana
{
	Mesh::Mesh(const VertexFormat& vertexFormat, Primitive primitive) : Object("Mesh"), _vertexFormat(vertexFormat), _primitive(primitive), _vertexBuffer(nullptr), _instanceBuffer(nullptr)
	{
	}
		
	Mesh::~Mesh()
	{
		AE_DELETE(_vertexBuffer);
		AE_DELETE(_instanceBuffer);

		for (auto i = _indexComponents.createIterator(); i; i++)
		{
			AE_RELEASE(*i);
		}
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
	
	VertexBuffer* Mesh::setVertexBuffer(const VertexFormat& vertexFormat, uint32 vertexCount, bool dynamic, void* vertexPointer)
	{
		_vertexBuffer = new VertexBuffer(vertexFormat, vertexCount, dynamic, vertexPointer);
		
		return _vertexBuffer;
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
		return (uint32)_indexComponents.size(); //temp
	}

	MeshIndexComponent* Mesh::addIndexComponent(Mesh::Primitive primitive)
	{
		MeshIndexComponent* component = new MeshIndexComponent(this, getNumIndexComponents(), primitive);
		component->reference();
		_indexComponents.add(component);

		return component;
	}

	void Mesh::addIndexComponent(MeshIndexComponent* component)
	{
		if (component)
		{
			component->reference();
			_indexComponents.add(component);
		}
	}

	MeshIndexComponent* Mesh::getIndexComponent(uint32 index)
	{
		if (index < getNumIndexComponents())
		{
			return _indexComponents[index];
		}
		return nullptr;
	}

	InstanceBuffer* Mesh::setInstanceBuffer(const VertexFormat& format, uint32 instanceCount, bool dynamic, void* dataPointer)
	{
		_instanceBuffer = new InstanceBuffer(format, instanceCount, dynamic, dataPointer);

		_instanceProperties._instanced = true;
		_instanceProperties._numInstances = instanceCount;

		return _instanceBuffer;
	}

	InstanceBuffer* Mesh::getInstanceBuffer()
	{
		return _instanceBuffer;
	}

	Mesh::InstanceProperties& Mesh::getInstanceProperties()
	{
		return _instanceProperties;
	}


	Mesh::InstanceProperties::InstanceProperties()
		: _instanced(false), _numInstances(0)
	{

	}

	bool Mesh::InstanceProperties::isInstanced() const
	{
		return _instanced;
	}

	int32 Mesh::InstanceProperties::getNumInstances() const
	{
		return _numInstances;
	}
}