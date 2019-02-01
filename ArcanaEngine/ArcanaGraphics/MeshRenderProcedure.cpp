#include "MeshRenderProcedure.h"

#include "ResourceManager.h"

namespace Arcana
{

	MeshRenderProcedure::MeshRenderProcedure(Mesh* mesh, Material* material, const RenderState& renderState, std::string stage) 
		: _data(nullptr), _mesh(mesh), _material(material), _renderState(renderState), _stage(stage)
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
		_data->context.renderState = _renderState;
		_data->context.transform.setIdentity();
		_data->context.rendererStage = _stage;
	}

	void MeshRenderProcedure::updateRenderData(const RenderDataUpdate& data)
	{
		if (isValidProcedure())
		{
			_data->context.projectionMatrix = data.projection;
			_data->context.viewMatrix = data.view;
			_data->context.eyePosition = data.eyePosition;
			_data->context.lightProperties = data.lightProperties;
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