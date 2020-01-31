#include "ElevationDataGenerator.h"

#include "HeightField.h"
#include "Random.h"

namespace Arcana
{
	ElevationDataGenerator::ElevationDataGenerator()
	{
		Texture::Parameters params;
		params.setMinFilter(TextureFilter::Linear);
		params.setMagFilter(TextureFilter::Linear);
		_terrainDataTexture = Texture::create2DArray(Texture::Red, 101, 101, 1024, Texture::R32F, Texture::Float, nullptr, params);

		_layers.resize(1024);
		for (int i = 0; i < 1024; i++)
		{
			_layers[i].free = true;
			_layers[i].loaded = false;
			_layers[i].id = -1;
		}

		_generator = new TerrainTileGenerator();
	}

	ElevationDataGenerator::~ElevationDataGenerator()
	{

	}

	void ElevationDataGenerator::update(Material* material)
	{
		const int32 updateLimit = 10;


		int32 unit = _terrainDataTexture->bind(material);
		material->getTechnique(0)->getPass(0)->getUniform("u_ElevationSampler").setValue(unit);
	}

	uint32 ElevationDataGenerator::getTile(TerrainQuad* quad, Matrix4d transform)
	{
		int64 tileId = quad->getId();

		auto iter = _idToLayer.find(tileId);

		if (iter != _idToLayer.end())
		{
			GenerationTask<HeightField<float>, TileParameters, int64>* task = _tasks[tileId];

			uint32 layer = (*iter).second;

			if (_layers[layer].loaded)
				return layer;

			if (task && task->isDone())
			{
				HeightField<float>* heightField = task->get();

				_terrainDataTexture->update2DArray(0, 0, 0, layer, 101, 101, 1, Texture::Red, Texture::Float, heightField->getData());

				delete heightField;

				_layers[layer].loaded = true;

				return layer;
			}
			
			//return parent id;
			return layer;
		}

		int32 freeLayer = -1;
		for (int i = 0; i < 1024; i++)
		{
			if (_layers[i].free)
			{
				freeLayer = i;
				break;
			}
		}

		if (freeLayer != -1)
		{
			TileParameters params;
			params.radius = 6367000.0;
			params.tileSize = quad->getPhysicalLevel();
			params.tileOffset = Vector2d(quad->getPhysicalXCoordinate(), quad->getPhysicalYCoordinate());
			params.transform = transform;
			_generator->generate(params, tileId);

			_layers[freeLayer].free = false;
			_layers[freeLayer].loaded = false;
			_layers[freeLayer].id = tileId;

			_idToLayer[tileId] = freeLayer;
			_tasks[tileId] = _generator->getTask(tileId);

			return (uint32)freeLayer;
		}

  		return 0;
	}
}