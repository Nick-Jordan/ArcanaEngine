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
	class ARCANA_GRAPHICS_API MeshRenderData : public RenderData
	{
	public:
		 
		virtual void render(ObjectRenderer& renderer) override;

	public:

		MeshRenderContext context;
	};

	class ARCANA_GRAPHICS_API MeshRenderProcedure : public RenderProcedure
	{
	public:

		MeshRenderProcedure();

		virtual ~MeshRenderProcedure();

		virtual bool isDirty() const override;

		virtual void markDirty(bool dirty) override;

		virtual void createRenderData() override;

		virtual void updateRenderData(const RenderDataUpdate& data) override;

		virtual RenderData* getRenderData() const override;

		virtual bool isValidProcedure() override;



		//test

		Material* test;
		Mesh* mesh;
		RenderState testRenderState;

	private:

		MeshRenderData* _data;
	};

}

#endif // !MESH_RENDER_PROCEDURE_H_

