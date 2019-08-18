#include "StaticMeshComponent.h"

#include "MeshRenderProcedure.h"

namespace Arcana
{

	//StaticMeshComponent::StaticMeshComponent(const std::string& path, const StaticMesh::Properties& properties) : MeshComponent(), _staticMesh(new StaticMesh(path, properties))
	//{

	//}

	StaticMeshComponent::StaticMeshComponent(StaticMesh* staticMesh, uint32 materialIndex) : _staticMesh(staticMesh), _materialIndex(materialIndex), _lightMap(nullptr)
	{
		AE_REFERENCE(_staticMesh);

		initialize();
	}

	StaticMeshComponent::StaticMeshComponent(StaticMesh* staticMesh, Material* material) : _staticMesh(staticMesh), _lightMap(nullptr)
	{
		if (_staticMesh)
		{
			_staticMesh->reference();

			_materialIndex = _staticMesh->addMaterial(material);
		}

		initialize();
	}

	StaticMeshComponent::~StaticMeshComponent()
	{
		AE_RELEASE(_staticMesh);
	}

	void StaticMeshComponent::initialize()
	{
		MeshComponent::initialize();
	}

	bool StaticMeshComponent::createRenderProcedure()
	{
		if (_staticMesh)
		{
			_renderProcedure = new MeshRenderProcedure(_staticMesh->getMesh(), _staticMesh->getMaterial(_materialIndex), _staticMesh->getMeshRenderProperties());
			_renderProcedure->reference();

			return true;
		}

		return false;
	}

	void StaticMeshComponent::updateRenderProcedure()
	{
		_renderProcedure->FTLResult.LightMap = _lightMap;
		_renderProcedure->FTLResult.IndirectLightData = _indirectLightingTexture;
	}

	const StaticMesh* StaticMeshComponent::getStaticMesh() const
	{
		return _staticMesh;
	}

	void StaticMeshComponent::setFTLGlobalIlluminationTexture(Texture* data, AxisAlignedBoundingBoxf boundingBox)
	{
		_indirectLightingTexture.set(data, boundingBox);
	}

	const FTLGlobalIlluminationTexture& StaticMeshComponent::getFTLGlobalIlluminationTexture() const
	{
		return _indirectLightingTexture;
	}

	void StaticMeshComponent::setLightMap(Texture* texture)
	{
		AE_RELEASE(_lightMap);
		_lightMap = texture;
		AE_REFERENCE(_lightMap);
	}

	Texture* StaticMeshComponent::getLightMap() const
	{
		return _lightMap;
	}

	const uint32& StaticMeshComponent::getMaterialIndex() const
	{
		return _materialIndex;
	}

	Material* StaticMeshComponent::getStaticMeshMaterial() const
	{
		if (_staticMesh)
		{
			return _staticMesh->getMaterial(_materialIndex);
		}

		return nullptr;
	}
}
