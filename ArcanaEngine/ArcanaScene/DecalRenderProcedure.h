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
	class ARCANA_SCENE_API DecalRenderProcedure : public RenderProcedure
	{
	public:

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
	};

}

#endif // !DECAL_RENDER_PROCEDURE_H_

