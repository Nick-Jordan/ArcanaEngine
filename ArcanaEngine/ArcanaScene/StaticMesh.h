#ifndef STATIC_MESH_H_
#define STATIC_MESH_H_

#include "SceneDefines.h"

#include "RenderProcedure.h"
#include "GlobalObjectID.h"
#include "MaterialMap.h"
#include "Callback.h"
#include "Mesh.h"
#include <string>

namespace Arcana
{
	REGISTER_CALLBACK(MeshChangedCallback);

	class ARCANA_SCENE_API StaticMesh : public Object
	{
	public:

		struct ARCANA_SCENE_API Properties
		{
			int32 LightMapResolution;
			Shader RenderingShader;
			RenderState RenderState;
			LightProperties LightProperties;
			bool isEnvironmentMesh;
			bool isTransparent;
			bool isBackgroundSkybox;
			std::string VertexShader;
			std::string FragmentShader;

			Properties() : isEnvironmentMesh(false), isTransparent(false), isBackgroundSkybox(false) {}
		};

		StaticMesh(const std::string& path, const Properties& properties);

		~StaticMesh();

		MeshChangedCallback& getMeshChangedCallback();

		Mesh* getMesh() const;

		Material* getMaterial(uint32 index) const;

		int32 addMaterial(Material* material);

		const GlobalObjectID& getGUID() const;

		const GlobalObjectID& getLightingGUID() const;

		int32 getLightMapResolution() const;

		const RenderProcedure::RenderProperties& getMeshRenderProperties() const;

		bool isTransparent() const;

		bool isEnvironmentMesh() const;
		
		bool isBackgroundSkybox() const;

	private:

		bool initialize(const std::string& path, const Properties& properties, const bool reinitialize);

	private:

		std::string _path;

		bool _initialized;

		Mesh* _mesh;

		GlobalObjectID _guid;

		GlobalObjectID _lightingGUID;

		int32 _lightMapResolution;

		RenderProcedure::RenderProperties _meshRenderProperties;

		bool _isTransparent;

		bool _isEnvironmentMesh;

		bool _isBackgroundSkybox;

		MeshChangedCallback _meshChangedCallback;

		std::vector<Material*> _materials;
	};

}

#endif // !STATIC_MESH_H_

