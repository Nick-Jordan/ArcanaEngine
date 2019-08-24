#ifndef DECAL_RENDER_PROCEDURE_H_
#define DECAL_RENDER_PROCEDURE_H_

#include "SceneDefines.h"

#include "MeshRenderProcedure.h"
#include "DecalComponent.h"

//test
#include "Material.h"
#include "Mesh.h"
#include "RenderState.h"

namespace Arcana
{
	struct DecalVertex
	{
		Vector4f color;
		Vector4f texCoords;

		Vector4f transform0;
		Vector4f transform1;
		Vector4f transform2;
		Vector4f transform3;
	};

	class ARCANA_SCENE_API DecalRenderProcedure : public RenderProcedure
	{
	public:

		Decal* Decals;
		uint32 NumDecals;

		DecalRenderProcedure(const DecalProperties& properties);

		virtual ~DecalRenderProcedure();
		
		virtual void render() override;

		virtual bool isValidProcedure() override;

	private:

		Material* selectMaterial(bool zTest);

		void passDecalAttributes(Shader* shader);

	private:

		Mesh* _mesh;
		Material* _decalMaterial;
		Material* _zTestedDecalMaterial;
		DecalProperties _properties;
		VertexFormat _instanceFormat;
	};

}

#endif // !DECAL_RENDER_PROCEDURE_H_

