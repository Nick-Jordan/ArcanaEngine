#ifndef ELEVATION_DATA_GENERATOR_H_
#define ELEVATION_DATA_GENERATOR_H_

#include "TerrainDefines.h"

#include "Material.h"
#include "Object.h"

#include "TerrainQuad.h"
#include "TerrainTileGenerator.h"

namespace Arcana
{
	class ARCANA_TERRAIN_API ElevationDataGenerator : public Object
	{
	public:

		ElevationDataGenerator();

		~ElevationDataGenerator();

		void update(Material* material);

		uint32 getTile(TerrainQuad* quad, Matrix4d transform);

	private:

		struct Layer
		{
			int64 id;
			bool free;
			bool loaded;
		};

		Texture* _terrainDataTexture;

		TerrainTileGenerator* _generator;

		std::map<int64, uint32> _idToLayer;
		std::map<int64, GenerationTask<HeightField<float>, TileParameters, int64>*> _tasks;

		std::vector<Layer> _layers;
	};
}

#endif // !ELEVATION_DATA_GENERATOR_H_