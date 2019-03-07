#include "MeshRenderProcedure.h"

#include "ResourceManager.h"

namespace Arcana
{

	MeshRenderProcedure::MeshRenderProcedure(Mesh* mesh, Material* material, const MeshRenderProperties& properties)
		: _data(nullptr), _mesh(mesh), _material(material), _properties(properties)
	{
		if (_mesh)
		{
			_mesh->reference();
		}
		if (_material)
		{
			_material->reference();
		}
	}


	MeshRenderProcedure::~MeshRenderProcedure()
	{
		if (_data)
		{
			AE_DELETE(_data);
		}

		AE_RELEASE(_mesh);
		AE_RELEASE(_material);
	}

	bool MeshRenderProcedure::isDirty() const
	{
		return true;
	}

	void MeshRenderProcedure::markDirty(bool dirty)
	{
		//nothing
	}

	void MeshRenderProcedure::createRenderData()
	{
		if (_data)
		{
			AE_DELETE(_data);
		}

		_data = new MeshRenderData();

		_data->context.mesh = _mesh;
		_data->context.material = _material;
		_data->context.renderProperties = _properties;
		_data->context.transform.setIdentity();
	}

	void MeshRenderProcedure::updateRenderData(const RenderDataUpdate& data)
	{
		if (isValidProcedure())
		{
			_data->context.projectionMatrix = data.projection;
			_data->context.viewMatrix = data.view;
			_data->context.eyePosition = data.eyePosition;
			_data->context.transform.set(data.transform);
		}
	}

	RenderData* MeshRenderProcedure::getRenderData() const
	{
		return _data;
	}

	bool MeshRenderProcedure::isValidProcedure()
	{
		return _data != nullptr;
	}

	void MeshRenderData::render(ObjectRenderer& renderer)
	{
		renderer.addMesh(context);
	}

	const MeshRenderContext& MeshRenderData::getContext() const
	{
		return context;
	}
}