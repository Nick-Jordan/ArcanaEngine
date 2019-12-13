#ifndef TERRAIN_TILE_GENERATOR_H_
#define TERRAIN_TILE_GENERATOR_H_

#include "TerrainDefines.h"

#include "ProceduralGenerator.h"
#include "HeightField.h"
#include "Texture.h"

#include "Matrix4.h"

namespace Arcana
{
	struct ARCANA_TERRAIN_API TileParameters
	{
		double tileSize;
		Vector2d tileOffset;
		double radius;
		Matrix4d transform;
	};

	class ARCANA_TERRAIN_API TerrainTileGenerator : public ProceduralGenerator<HeightField<float>, TileParameters, int64>
	{
	public:

		virtual void generateObjectAsync(const TileParameters& params, HeightField<float>** object) override;
	};
}

#endif // !TERRAIN_TILE_GENERATOR_H_

