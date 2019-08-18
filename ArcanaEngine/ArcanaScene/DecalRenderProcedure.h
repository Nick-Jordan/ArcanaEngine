#ifndef DECAL_RENDER_PROCEDURE_H_
#define DECAL_RENDER_PROCEDURE_H_

#include "GraphicsDefines.h"

#include "MeshRenderProcedure.h"
#include "DecalComponent.h"

//test
#include "Material.h"
#include "Mesh.h"
#include "RenderState.h"

namespace Arcana
{
	class ARCANA_GRAPHICS_API DecalRenderProcedure : public RenderProcedure
	{
	public:

		DecalRenderProcedure(const DecalProperties& properties);

		virtual ~DecalRenderProcedure();

		virtual bool isDirty() const override;

		virtual void markDirty(bool dirty) override;

		virtual void createRenderData() override;

		virtual void updateRenderData(const RenderDataUpdate& data) override;

		virtual RenderData* getRenderData() const override;

		virtual bool isValidProcedure() override;

	private:

		MeshRenderData* _data;
		Mesh* _mesh;
		Material* _material;
		DecalProperties _properties;
		MeshRenderProperties _renderProperties;
	};

}

#endif // !DECAL_RENDER_PROCEDURE_H_

