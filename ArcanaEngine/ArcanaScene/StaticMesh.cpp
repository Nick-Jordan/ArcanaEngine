#include "StaticMesh.h"

#include "MeshLoader.h"

namespace Arcana
{
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
}
