#include "MeshComponent.h"

#include "MeshRenderProcedure.h"

namespace Arcana
{

	MeshComponent::MeshComponent() : _mesh(nullptr), _material(nullptr)
	{
	}

	MeshComponent::MeshComponent(Mesh* mesh, Material* material, const RenderProcedure::RenderProperties& properties) : _mesh(nullptr), _material(nullptr)
	{
		initialize(mesh, material, properties);
	}

	MeshComponent::~MeshComponent()
	{
		AE_RELEASE(_mesh);
		AE_RELEASE(_material);
	}

	void MeshComponent::initialize(Mesh* mesh, Material* material, const RenderProcedure::RenderProperties& properties)
	{
		_mesh = mesh;
		_material = material;
		_properties = properties;

		AE_REFERENCE(_mesh);
		AE_REFERENCE(_material);

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

		return true;
	}
}