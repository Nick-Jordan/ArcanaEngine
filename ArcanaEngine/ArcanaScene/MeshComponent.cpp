#include "MeshComponent.h"

#include "MeshRenderProcedure.h"

namespace Arcana
{

	MeshComponent::MeshComponent() : _mesh(nullptr), _material(nullptr)
	{
	}

	MeshComponent::MeshComponent(Mesh* mesh, Material* material, const MeshRenderProperties& properties) : _mesh(nullptr), _material(nullptr)
	{
		initialize(mesh, material, properties);
	}

	MeshComponent::~MeshComponent()
	{
	}

	void MeshComponent::initialize(Mesh* mesh, Material* material, const MeshRenderProperties& properties)
	{
		_mesh = mesh;
		_material = material;
		_properties = properties;

		initialize();
	}

	void MeshComponent::initialize()
	{
		GeometryComponent::initialize();
	}

	bool MeshComponent::createRenderProcedure()
	{
		_renderProcedure = new MeshRenderProcedure(_mesh, _material, _properties);
		_renderProcedure->reference();

		_renderProcedure->createRenderData();

		return true;
	}

	void MeshComponent::updateRenderData(Matrix4d view, Matrix4d projection, Vector3d eyePosition)
	{
		RenderDataUpdate update;
		update.view = view;
		update.projection = projection;
		update.eyePosition = eyePosition;
		update.ftlResult = _ftlResult;
		//copy light properties;
		//update.lightProperties = _lightProperties;
		update.transform.set(getWorldTransform());

		_renderProcedure->updateRenderData(update);
	}
}