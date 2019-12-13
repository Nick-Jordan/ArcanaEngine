#include "TerrainTileGenerator.h"

#include "Random.h"
#include "Noise.h"

namespace Arcana
{
	void TerrainTileGenerator::generateObjectAsync(const TileParameters& params, HeightField<float>** object)
	{
		*object = new HeightField<float>(101, 101);

		Noise::FunctionProperties tinyDetail;
		tinyDetail.octaves = 9;
		tinyDetail.persistence = 0.8;
		tinyDetail.frequency = 0.15;
		tinyDetail.low = 0.0;
		tinyDetail.high = 50;

		Noise::FunctionProperties smallDetail;
		smallDetail.octaves = 6;
		smallDetail.persistence = 0.8;
		smallDetail.frequency = 0.05;
		smallDetail.low = 0.0;
		smallDetail.high = 100.0;

		Noise::FunctionProperties largeDetail;
		largeDetail.octaves = 8;
		largeDetail.persistence = 0.8;
		largeDetail.frequency = 0.003;
		largeDetail.low = -2000.0;
		largeDetail.high = 3000.0;

		Noise::FunctionProperties mountainsCellSquared;
		mountainsCellSquared.function = Noise::CellularSquaredNoise;
		mountainsCellSquared.octaves = 3;
		mountainsCellSquared.persistence = 0.6;
		mountainsCellSquared.frequency = 0.05;
		mountainsCellSquared.low = -25000.0;
		mountainsCellSquared.high = 25000.0;

		Noise::FunctionProperties mountainsRidged;
		mountainsRidged.function = Noise::RidgedNoise;
		mountainsRidged.octaves = 11;
		mountainsRidged.persistence = 0.5;
		mountainsRidged.frequency = 0.03;
		mountainsRidged.low = 0.0;
		mountainsRidged.high = 7500.0;

		Noise::FunctionProperties mountains;
		mountains.function = Noise::CubedNoise;
		mountains.operation = Noise::Multiply;
		mountains.persistence = 0.7;
		mountains.frequency = 0.002;
		mountains.low = -13.0;
		mountains.high = 13.0;
		mountains.clamp = Vector2d(0.0, 1.0);
		mountains.children.add(mountainsCellSquared);
		mountains.children.add(mountainsRidged);

		Noise::FunctionProperties oceanNoise;
		oceanNoise.operation = Noise::Subtract;
		oceanNoise.octaves = 6;
		oceanNoise.persistence = 0.9;
		oceanNoise.frequency = 0.002;
		oceanNoise.low = 10000.0;
		oceanNoise.high = 20000.0;

		Noise::FunctionProperties oceans;
		oceans.operation = Noise::Multiply;
		oceans.octaves = 6;
		oceans.persistence = 0.75;
		oceans.frequency = 0.00015;
		oceans.low = -5.0;
		oceans.high = 7.0;
		oceans.clamp = Vector2d(0.0, 1.0);
		oceans.children.add(oceanNoise);

		Noise::Base base;
		base.base = 10.0;
		base.functions.add(tinyDetail);
		base.functions.add(smallDetail);
		base.functions.add(largeDetail);
		base.functions.add(mountains);
		base.functions.add(oceans);

		Vector3d offset = Vector3d::zero();// (-1.2, 0.5, 0.2);

		for (uint32 i = 0; i < 101; i++)
		{
			for (uint32 j = 0; j < 101; j++)
			{
				float x = (float)i / 100.0f;
				float y = (float)j / 100.0f;

				Vector3d position = Vector3d(x * params.tileSize + params.tileOffset.x, y * params.tileSize + params.tileOffset.y, params.radius);
				position = params.transform * (Vector3d::normalize(position) * params.radius);

				double h;//= Noise::simplexNoise(position.x * 0.0000003, position.y * 0.0000003, position.z * 0.0000003) * 10000.0;
				Noise::evaluateNoise(position/100000.0 + offset, base, h);

				(*object)->setHeight(i, j, (float)h);
			}
		}
	}
}