#ifndef TERRAIN_RENDER_PROCEDURE_H_
#define TERRAIN_RENDER_PROCEDURE_H_

#include "TerrainDefines.h"

#include "RenderProcedure.h"
#include "Terrain.h"
#include "Transform.h"
#include "Material.h"

namespace Arcana
{
	class ARCANA_TERRAIN_API TerrainRenderProcedure : public RenderProcedure
	{
	public:

		TerrainRenderProcedure(Terrain* terrain, std::string vertex, std::string fragment);

		virtual ~TerrainRenderProcedure();

		virtual void render() override;

		virtual bool isValidProcedure()  override;

		void updateTerrain();

		void renderTerrain();

	private:

		Terrain* _terrain;
		Mesh* _mesh;
		Material* _terrainMaterial;
		std::string _vertex;
		std::string _fragment;
		VertexFormat _instanceFormat;

		Texture* _terrainSurface;
		Texture* _terrainColor;
	};

}

#endif // !TERRAIN_RENDER_PROCEDURE_H_

