#include "MeshRenderProcedure.h"

#include "ResourceManager.h"

namespace Arcana
{

	MeshRenderProcedure::MeshRenderProcedure(Mesh* mesh, Material* material, const MeshRenderProperties& properties)
		: _data(nullptr), _mesh(mesh), _material(material), _properties(properties)
	{
		AE_REFERENCE(_mesh);
		AE_REFERENCE(_material);
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

			_data->context.uniforms.clear();

			if (data.ftlResult.LightMap)
			{
				MeshRenderContext::UniformParameter lightMap;
				lightMap.name = "u_LightMap";
				lightMap.value.type = Uniform::Value::Int32;
				lightMap.value.i = data.ftlResult.LightMap->bind();
				_data->context.uniforms.push_back(lightMap);
			}

			if (data.ftlResult.IndirectLightData.getData() != nullptr)
			{
				MeshRenderContext::UniformParameter indirectLightData;
				indirectLightData.name = "u_IndirectLightData.data";
				indirectLightData.value.type = Uniform::Value::Int32;
				indirectLightData.value.i = data.ftlResult.IndirectLightData.getData()->bind();

				MeshRenderContext::UniformParameter indirectLightBoundsMin;
				indirectLightBoundsMin.name = "u_IndirectLightData.boundsMin";
				indirectLightBoundsMin.value.type = Uniform::Value::Vec3f;
				indirectLightBoundsMin.value.vec3 = data.ftlResult.IndirectLightData.getBoundingBox().getMin();

				MeshRenderContext::UniformParameter indirectLightBoundsMax;
				indirectLightBoundsMax.name = "u_IndirectLightData.boundsMax";
				indirectLightBoundsMax.value.type = Uniform::Value::Vec3f;
				indirectLightBoundsMax.value.vec3 = data.ftlResult.IndirectLightData.getBoundingBox().getMax();

				_data->context.uniforms.push_back(indirectLightData);
				_data->context.uniforms.push_back(indirectLightBoundsMin);
				_data->context.uniforms.push_back(indirectLightBoundsMax);
			}
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