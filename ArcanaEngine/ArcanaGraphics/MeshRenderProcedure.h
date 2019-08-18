#ifndef MESH_RENDER_PROCEDURE_H_
#define MESH_RENDER_PROCEDURE_H_

#include "GraphicsDefines.h"

#include "RenderProcedure.h"


//test
#include "Material.h"
#include "Mesh.h"
#include "RenderState.h"

namespace Arcana
{
	class ARCANA_GRAPHICS_API MeshRenderProcedure : public RenderProcedure
	{
	public:

		MeshRenderProcedure(Mesh* mesh, Material* material, const RenderProcedure::RenderProperties& properties);

		virtual ~MeshRenderProcedure();
		
		virtual void render() override;

		virtual void renderWithShader(const Shader& shader, bool bindRenderState) override;

		virtual bool isValidProcedure() override;

	private:

		Mesh* _mesh;
		Material* _material;
	};

}

#endif // !MESH_RENDER_PROCEDURE_H_

