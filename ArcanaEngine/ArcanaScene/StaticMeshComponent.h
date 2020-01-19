#ifndef STATIC_MESH_COMPONENT_H_
#define STATIC_MESH_COMPONENT_H_

#include "SceneDefines.h"

#include "MeshComponent.h"
#include "StaticMesh.h"
#include "FTLGlobalIlluminationTexture.h"

namespace Arcana
{

	class ARCANA_SCENE_API StaticMeshComponent : public MeshComponent
	{
	public:

		//StaticMeshComponent(const std::string& path, const StaticMesh::Properties& properties);

		StaticMeshComponent();

		StaticMeshComponent(StaticMesh* staticMesh, uint32 materialIndex);

		StaticMeshComponent(StaticMesh* staticMesh, Material* material);

		virtual ~StaticMeshComponent();

		virtual void initialize() override;

		virtual bool createRenderProcedure() override;

		virtual void updateRenderProcedure() override;


		const StaticMesh* getStaticMesh() const;

		void setFTLGlobalIlluminationTexture(Texture* data, AxisAlignedBoundingBoxf boundingBox);

		const FTLGlobalIlluminationTexture& getFTLGlobalIlluminationTexture() const;

		void setLightMap(Texture* texture);

		Texture* getLightMap() const;

		const uint32& getMaterialIndex() const;

		Material* getStaticMeshMaterial() const;

	protected:

		void setStaticMesh(StaticMesh* staticMesh, uint32 materialIndex);

		void setStaticMesh(StaticMesh* staticMesh, Material* material);

	private:

		StaticMesh* _staticMesh;

		uint32 _materialIndex;

		FTLGlobalIlluminationTexture _indirectLightingTexture;

		Texture* _lightMap;
	};

}

#endif // !STATIC_MESH_COMPONENT_H_

