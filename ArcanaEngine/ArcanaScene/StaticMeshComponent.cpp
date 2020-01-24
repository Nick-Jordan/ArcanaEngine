#include "StaticMeshComponent.h"

#include "MeshRenderProcedure.h"
#include "ResourceManager.h"
#include "ResourceCreator.h"

namespace Arcana
{

	//StaticMeshComponent::StaticMeshComponent(const std::string& path, const StaticMesh::Properties& properties) : MeshComponent(), _staticMesh(new StaticMesh(path, properties))
	//{

	//}

	StaticMeshComponent::StaticMeshComponent() : _staticMesh(nullptr), _lightMap(nullptr), _materialIndex(0)
	{

	}

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

	void StaticMeshComponent::setStaticMesh(StaticMesh* staticMesh, uint32 materialIndex)
	{
		_staticMesh = staticMesh;
		_materialIndex = materialIndex;

		AE_REFERENCE(staticMesh);
	}

	void StaticMeshComponent::setStaticMesh(StaticMesh* staticMesh, Material* material)
	{
		_staticMesh = staticMesh;

		if (_staticMesh)
		{
			_staticMesh->reference();

			_materialIndex = _staticMesh->addMaterial(material);
		}
	}

	class StaticMeshComponentResource : public ResourceCreator<StaticMeshComponent>
	{
	public:

		StaticMeshComponentResource(const GlobalObjectID& id, const std::string& type, const ResourceData& data, Scheduler* dependencyScheduler)
			: ResourceCreator<StaticMeshComponent>(id, type, data, dependencyScheduler)
		{
			materialIndex = data.getUint32Parameter("materialIndex");

			meshTask = ResourceManager::instance().loadResource<StaticMesh>(data.getResourceDependency("staticMesh"), dependencyScheduler);
			if (meshTask)
			{
				meshTask->wait();
			}
			else
			{
				const ResourceData* meshData = data.getAdditionalData("staticMesh");

				if (meshData)
				{
					meshTask = ResourceManager::instance().buildResource<StaticMesh>(GlobalObjectID(id.getName() + "::staticMesh"), "staticMesh", *meshData, dependencyScheduler);
					if (meshTask)
					{
						meshTask->wait();
					}
				}
			}

			materialTask = ResourceManager::instance().loadResource<Material>(data.getResourceDependency("material"), dependencyScheduler);
			if (materialTask)
			{
				materialTask->wait();
			}
			else
			{
				const ResourceData* materialData = data.getAdditionalData("material");

				if (materialData)
				{
					materialTask = ResourceManager::instance().buildResource<Material>(GlobalObjectID(id.getName() + "::material"), "material", *materialData, dependencyScheduler);
					if (materialTask)
					{
						materialTask->wait();
					}
				}
			}
		}

		void syncInitialize() override
		{
			StaticMesh* staticMesh = nullptr;
			if (meshTask)
			{
				staticMesh = meshTask->get();
			}

			Material* material = nullptr;
			if (materialTask)
			{
				material = materialTask->get();
			}

			if (material)
			{
				setStaticMesh(staticMesh, material);
			}
			else
			{
				setStaticMesh(staticMesh, materialIndex);
			}

			initialize();
		}

	private:

		uint32 materialIndex;
		LoadResourceTask<Material>* materialTask;
		LoadResourceTask<StaticMesh>* meshTask;
	};

	Resource::Type<StaticMeshComponentResource, true> staticMeshComponentResource("staticMeshComponent");
}
