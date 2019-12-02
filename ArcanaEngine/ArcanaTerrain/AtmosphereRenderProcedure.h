#ifndef ATMOSPHERE_RENDER_PROCEDURE_H_
#define ATMOSPHERE_RENDER_PROCEDURE_H_

#include "TerrainDefines.h"

#include "RenderProcedure.h"
#include "Terrain.h"
#include "Transform.h"
#include "Material.h"

namespace Arcana
{
	class ARCANA_TERRAIN_API AtmosphereRenderProcedure : public RenderProcedure
	{
	public:

		AtmosphereRenderProcedure();

		virtual ~AtmosphereRenderProcedure();

		virtual void render() override;

		virtual bool isValidProcedure()  override;

		void renderAtmosphere();

	private:

		Mesh* _mesh;
		Material* _material;
	};
}

#endif // !ATMOSPHERE_RENDER_PROCEDURE_H_