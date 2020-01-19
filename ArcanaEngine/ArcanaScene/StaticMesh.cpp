#include "StaticMesh.h"

#include "MeshLoader.h"
#include "ResourceManager.h"
#include "ResourceCreator.h"

namespace Arcana
{
	StaticMesh::StaticMesh() : Object("StaticMesh"),
		_initialized(false), _path(""), _mesh(nullptr), _lightMapResolution(0)
	{

	}

	StaticMesh::StaticMesh(const std::string& path, const Properties& properties) : Object("StaticMesh"),
		_initialized(false), _path(path), _mesh(nullptr), _lightMapResolution(0)
	{
		if (!initialize(path, properties, true))
		{
			LOGF(Error, CoreEngine, "Failed to properly initialize static mesh, %d!", _guid.getId());
		}
	}

	StaticMesh::~StaticMesh()
	{
		AE_RELEASE(_mesh);
		for (auto iter = _materials.begin(); iter != _materials.end(); iter++)
		{
			AE_RELEASE(*iter);
		}
		_materials.clear();
	}

	bool StaticMesh::initialize(const std::string& path, const Properties& properties, const bool reinitialize)
	{
		if (_initialized && !reinitialize)
		{
			LOGF(Warning, CoreEngine, "Static mesh, %d, has already been initialized! Reinitialization not allowed...", _guid.getId());
			return true;
		}

		if (_initialized && reinitialize)
		{
			LOGF(Warning, CoreEngine, "Static mesh, %d, has already been initialized! Reinitializing...", _guid.getId());
		}
		else if (!_initialized)
		{
			_guid = GlobalObjectID(path);
		}

		_initialized = true;

		_lightMapResolution = properties.LightMapResolution;
		_meshRenderProperties.RenderState = properties.RenderState;
		_meshRenderProperties.LightProperties = properties.LightProperties;
		_isTransparent = properties.isTransparent;
		_isEnvironmentMesh = properties.isEnvironmentMesh;
		_isBackgroundSkybox = properties.isBackgroundSkybox;

		if (properties.isBackgroundSkybox)
		{
			_meshRenderProperties.RendererStage = "BackgroundSkyboxStage";
		}
		else if (properties.isEnvironmentMesh && properties.isTransparent)
		{
			_meshRenderProperties.RendererStage = "TransparentEnvironmentStage";
		}
		else if (properties.isEnvironmentMesh && !properties.isTransparent)
		{
			_meshRenderProperties.RendererStage = "OpaqueEnvironmentStage";
		}
		else if (!properties.isEnvironmentMesh && properties.isTransparent)
		{
			_meshRenderProperties.RendererStage = "TransparentObjectStage";
		}
		else if (!properties.isEnvironmentMesh && !properties.isTransparent)
		{
			_meshRenderProperties.RendererStage = "OpaqueObjectStage";
		}

		MeshStruct m = MeshLoader::instance().createMesh(path, properties.VertexShader, properties.FragmentShader);

		_mesh = m.mesh;
		if (m.materialMap)
		{
			m.materialMap->reference();
			_materials.push_back(m.materialMap);
		}

		if (!_mesh)
		{
			LOGF(Error, CoreEngine, "Mesh from path, \"%s,\" not loaded properly...", path.c_str());
			return false;
		}
		else
		{
			_mesh->reference();
		}

		if (!m.materialMap)
		{
			LOGF(Warning, CoreEngine, "Static mesh, %d, missing material map...", _guid.getId());
		}

		return true;
	}

	bool StaticMesh::asyncInitialize(const std::string& path, const bool reinitialize, 
		uint32& numMaterials, FileInputStream& file,
		Shader::Defines& vertexDefines, bool& hasNormal, bool& hasColor, bool& hasTexCoords0, VertexFormat& format,
		std::vector<std::vector<uint32>>& totalIndexData, std::vector<float>& vertexData, MaterialMap** materialMap)
	{
		if (_initialized && !reinitialize)
		{
			LOGF(Warning, CoreEngine, "Static mesh, %d, has already been initialized! Reinitialization not allowed...", _guid.getId());
			return true;
		}

		if (_initialized && reinitialize)
		{
			LOGF(Warning, CoreEngine, "Static mesh, %d, has already been initialized! Reinitializing...", _guid.getId());
		}
		else if (!_initialized)
		{
			_guid = GlobalObjectID(path);
		}

		_initialized = true;

		MeshLoader::instance().createMeshLoadData(path, numMaterials, file, vertexDefines, hasNormal, hasColor, hasTexCoords0, format, totalIndexData, vertexData, materialMap);
	}

	bool StaticMesh::syncInitialize(const std::string& path, const Properties& properties, uint32 numMaterials, FileInputStream& file,
		const Shader::Defines& vertexDefines, bool hasNormal, bool hasColor, bool hasTexCoords0, const VertexFormat& format,
		std::vector<std::vector<uint32>>& totalIndexData, const std::vector<float>& vertexData, MaterialMap** materialMap)
	{
		_lightMapResolution = properties.LightMapResolution;
		_meshRenderProperties.RenderState = properties.RenderState;
		_meshRenderProperties.LightProperties = properties.LightProperties;
		_isTransparent = properties.isTransparent;
		_isEnvironmentMesh = properties.isEnvironmentMesh;
		_isBackgroundSkybox = properties.isBackgroundSkybox;

		if (properties.isBackgroundSkybox)
		{
			_meshRenderProperties.RendererStage = "BackgroundSkyboxStage";
		}
		else if (properties.isEnvironmentMesh && properties.isTransparent)
		{
			_meshRenderProperties.RendererStage = "TransparentEnvironmentStage";
		}
		else if (properties.isEnvironmentMesh && !properties.isTransparent)
		{
			_meshRenderProperties.RendererStage = "OpaqueEnvironmentStage";
		}
		else if (!properties.isEnvironmentMesh && properties.isTransparent)
		{
			_meshRenderProperties.RendererStage = "TransparentObjectStage";
		}
		else if (!properties.isEnvironmentMesh && !properties.isTransparent)
		{
			_meshRenderProperties.RendererStage = "OpaqueObjectStage";
		}

		MeshStruct m = MeshLoader::instance().createMeshFinal(numMaterials, file, vertexDefines, hasNormal, hasColor, hasTexCoords0, format, 
			totalIndexData, vertexData, materialMap, properties.VertexShader, properties.FragmentShader);

		_mesh = m.mesh;
		if (m.materialMap)
		{
			m.materialMap->reference();
			_materials.push_back(m.materialMap);
		}

		if (!_mesh)
		{
			LOGF(Error, CoreEngine, "Mesh from path, \"%s,\" not loaded properly...", path.c_str());
			return false;
		}
		else
		{
			_mesh->reference();
		}

		if (!m.materialMap)
		{
			LOGF(Warning, CoreEngine, "Static mesh, %d, missing material map...", _guid.getId());
		}

		return true;
	}

	MeshChangedCallback& StaticMesh::getMeshChangedCallback()
	{
		return _meshChangedCallback;
	}

	Mesh* StaticMesh::getMesh() const
	{
		return _mesh;
	}

	Material* StaticMesh::getMaterial(uint32 index) const
	{
		if (index < _materials.size())
		{
			return _materials[index];
		}

		return nullptr;
	}

	int32 StaticMesh::addMaterial(Material* material)
	{
		if (material)
		{
			std::vector<Material*>::iterator iter = std::find(_materials.begin(), _materials.end(), material);

			if (iter != _materials.end())
			{
				return std::distance(_materials.begin(), iter);
			}
			else
			{
				material->reference();

				_materials.push_back(material);

				return _materials.size() - 1;
			}
		}

		return -1;
	}

	const GlobalObjectID& StaticMesh::getGUID() const
	{
		return _guid;
	}

	const GlobalObjectID& StaticMesh::getLightingGUID() const
	{
		return _lightingGUID;
	}

	int32 StaticMesh::getLightMapResolution() const
	{
		return _lightMapResolution;
	}

	const RenderProcedure::RenderProperties& StaticMesh::getMeshRenderProperties() const
	{
		return _meshRenderProperties;
	}

	bool StaticMesh::isTransparent() const
	{
		return _isTransparent;
	}

	bool StaticMesh::isEnvironmentMesh() const
	{
		return _isEnvironmentMesh;
	}

	bool StaticMesh::isBackgroundSkybox() const
	{
		return _isBackgroundSkybox;
	}

	class StaticMeshPropertiesResource : public ResourceCreator<StaticMesh::Properties>
	{
	public:

		StaticMeshPropertiesResource(const GlobalObjectID& id, const std::string& type, const ResourceData& data)
			: ResourceCreator<StaticMesh::Properties>(id, type, data)
		{
			shaderBuildTask = nullptr;

			LightMapResolution = data.getInt32Parameter("lightMapResolution");
			isEnvironmentMesh = data.getBoolParameter("isEnvironmentMesh") || data.getBoolParameter("environment");
			isTransparent = data.getBoolParameter("isTransparent") || data.getBoolParameter("transparent");
			isBackgroundSkybox = data.getBoolParameter("isBackgroundSkybox") || data.getBoolParameter("background") || data.getBoolParameter("skybox");
			VertexShader = data.getStringParameter("vertexShader");
			FragmentShader = data.getStringParameter("fragmentShader");

			const ResourceData* renderState = data.getAdditionalData("renderState");
			if (renderState)
			{
				//change to RenderState
				LoadResourceTask<OpenGLState>* buildTask = ResourceManager::instance().buildResource<OpenGLState>(GlobalObjectID(id.getName() + "::renderState"), "renderState", *renderState);
				buildTask->wait();
				this->RenderState.set(*buildTask->get());
			}

			const ResourceData* lightProperties = data.getAdditionalData("lightProperties");
			if (lightProperties)
			{
				LoadResourceTask<struct LightProperties>* buildTask = ResourceManager::instance().buildResource<struct LightProperties>(GlobalObjectID(id.getName() + "::lightProperties"), "lightProperties", *lightProperties);
				buildTask->wait();
				this->LightProperties = *buildTask->get();
			}

			const ResourceData* shader = data.getAdditionalData("shader");
			if (shader)
			{
				LoadResourceTask<Shader>* buildTask = ResourceManager::instance().buildResource<Shader>(GlobalObjectID(id.getName() + "::shader"), "shader", *shader);
				buildTask->wait();
				shaderBuildTask = buildTask;
			}
		}

		virtual void syncInitialize() override
		{
			if (shaderBuildTask)
			{
				RenderingShader = *shaderBuildTask->get();
			}

		}

	private:

		LoadResourceTask<Shader>* shaderBuildTask;
	};

	class StaticMeshResource : public ResourceCreator<StaticMesh>
	{
	public:

		StaticMeshResource(const GlobalObjectID& id, const std::string& type, const ResourceData& data)
			: ResourceCreator<StaticMesh>(id, type, data)
		{
			propertiesBuildTask = nullptr;

			path = data.getStringParameter("path");
			bool reinitialize = false;

			const ResourceData* params = data.getAdditionalData("properties");

			if (params)
			{
				LoadResourceTask<StaticMesh::Properties>* buildTask = ResourceManager::instance().buildResource<StaticMesh::Properties>(GlobalObjectID(id.getName() + "::properties"), "staticMeshProperties", *params);
				buildTask->wait();
				propertiesBuildTask = buildTask;
			}

			asyncInitialize(path, reinitialize, numMaterials, file, vertexDefines, hasNormal, hasColor, hasTexCoords0, format, totalIndexData, vertexData, &materialMap);
		}

		virtual void syncInitialize() override
		{
			if (propertiesBuildTask)
			{
				properties = *propertiesBuildTask->get();
			}

			StaticMesh::syncInitialize(path, properties, numMaterials, file, vertexDefines, hasNormal, hasColor, hasTexCoords0, format, totalIndexData, vertexData, &materialMap);
		}

	private:

		LoadResourceTask<StaticMesh::Properties>* propertiesBuildTask;

		std::string path;
		Properties properties;
		uint32 numMaterials;
		FileInputStream file;
		Shader::Defines vertexDefines;
		bool hasNormal;
		bool hasColor;
		bool hasTexCoords0;
		VertexFormat format;
		std::vector<std::vector<uint32>> totalIndexData;
		std::vector<float> vertexData;
		MaterialMap* materialMap;
	};

	Resource::Type<StaticMeshResource, true> staticMeshResource("staticMesh");
	Resource::Type<StaticMeshPropertiesResource, true> staticMeshPropertiesResource("staticMeshProperties");
}
