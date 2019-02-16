#ifndef TERRAIN_TILE_H_
#define TERRAIN_TILE_H_

#include "TerrainDefines.h"

#include "ProceduralObject.h"
#include "TextureProceduralData.h"

#include "Deformation.h"

#include "TerrainQuad.h"

namespace Arcana
{
	class ARCANA_TERRAIN_API TerrainTileProceduralParameters : public ProceduralParameters
	{
	public:

		TerrainTileProceduralParameters(TerrainQuad* quad, Matrix4d transformation, double radius = 0.0);

		virtual void setUniforms(Shader& shader) const override;

	private:

		TerrainQuad* _quad;
		Matrix4d _transformation;
		double _radius;
	};

	class ARCANA_TERRAIN_API TerrainTile : public ProceduralObject
	{
	public:

		TerrainTile();

		~TerrainTile();

		Texture* getHeightTexture();

	private:

		TextureProceduralData* _heightData;
	};
}

#endif // TERRAIN_TILE_H_

