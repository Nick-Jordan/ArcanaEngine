#ifndef ATMOSPHERE_RENDER_PROCEDURE_H_
#define ATMOSPHERE_RENDER_PROCEDURE_H_

#include "TerrainDefines.h"

#include "RenderProcedure.h"
#include "Terrain.h"

namespace Arcana
{
	class ARCANA_TERRAIN_API AtmosphereRenderData : public RenderData
	{
	public:

		AtmosphereRenderData();

		~AtmosphereRenderData();

		virtual void render(ObjectRenderer& renderer) override;

	public:

		MeshRenderContext _context;
	};

	class ARCANA_TERRAIN_API AtmosphereRenderProcedure : public RenderProcedure
	{
	public:

		AtmosphereRenderProcedure(const Transform& transform);

		virtual ~AtmosphereRenderProcedure();

		virtual bool isDirty() const override;

		virtual void markDirty(bool dirty) override;

		virtual void createRenderData()  override;

		virtual void updateRenderData(const RenderDataUpdate& data)  override;

		virtual RenderData* getRenderData() const  override;

		virtual bool isValidProcedure()  override;

	private:

		AtmosphereRenderData* _data;
		Mesh* _mesh;
		Material* _material;
		RenderState _renderState;
		Transform _transform;
		Matrix4d _dummyProjection;
	};

}

#endif // !ATMOSPHERE_RENDER_PROCEDURE_H_

