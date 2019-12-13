#include "Terrain.h"

#include "ArcanaLog.h"
#include "Profiler.h"
#include "MeshIndexComponent.h"

#include "TextureTileStorage.h"
#include "SphericalDeformation.h"
#include "Material.h"

namespace Arcana
{
	Texture* Terrain::_inscatter = nullptr;
	Texture* Terrain::_irradiance = nullptr;
	Texture* Terrain::_transmittance = nullptr;
	Texture* Terrain::_sunglare = nullptr;

	Terrain::Terrain(const Parameters& params)
	{
		TerrainQuad* root = new TerrainQuad(nullptr, 0, 0, -params.radius, -params.radius, 2.0 * params.radius, params.zmin, params.zmax);

		Deformation* deform = nullptr;

		if (params.deformation == "sphere")
		{
			deform = new SphericalDeformation(params.radius);
		}
		else
		{
			deform = new Deformation();
		}

		_terrainNode = new TerrainNode(root, deform, params.splitFactor, params.maxLevel);
		_terrainNode->reference();
		_culling = true;

		_elevationDataGenerator = new ElevationDataGenerator();

		/*scheduler = new Scheduler();

		TextureTileStorage* storage = new TextureTileStorage(128, 1296, Texture::Red, Texture::R32F, Texture::Float, Texture::Parameters());
		TileCache* cache = new TileCache("cache", storage, scheduler);
		ElevationProducer* elevationProducer = new ElevationProducer(cache, 25);
		TileSampler* elevationSampler = new TileSampler("elevationSampler", elevationProducer);
		elevationSampler->reference();
		_tileSamplers.push(elevationSampler);*/

		/*TextureTileStorage* surfaceStorage = new TextureTileStorage(128, 1296, Texture::RGBA, Texture::RGBA8, Texture::UnsignedByte, Texture::Parameters());
		TileCache* surfaceCache = new TileCache("surfaceCache", surfaceStorage, scheduler);
		ResidualProducer* surfaceProducer = new ResidualProducer(surfaceCache, "terrain_surface", "png");
		TileSampler* surfaceSampler = new TileSampler("surfaceSampler", surfaceProducer);
		surfaceSampler->reference();
		_tileSamplers.push(surfaceSampler);*/

		createAtmosphereTextures();
	}


	Terrain::~Terrain()
	{
		/*for (auto i = _tileSamplers.createIterator(); i; i++)
		{
			AE_RELEASE(*i);
		}*/

		AE_RELEASE(_terrainNode);

		//AE_RELEASE(scheduler);
	}

	void Terrain::drawTerrain(Material* material, Array<Vector4f>& data, int32& instanceCount)
	{
		/*for (int32 i = 0; i < _tileSamplers.size(); i++)
		{
			TileSampler* u = _tileSamplers[i];

			if (u != nullptr)
			{
				u->update(_terrainNode, material);
			}
		}*/

		_elevationDataGenerator->update(material);
		
		drawQuad(_terrainNode->getRootQuad(), data, instanceCount);
	}

	void Terrain::drawQuad(TerrainQuad* quad, Array<Vector4f>& data, int32& instanceCount)
	{
		if (_culling && quad->_visible == TerrainQuad::Invisible)
		{
			return;
		}
		/*if (async && !q->drawable) {
			return;
		}*/

		if (quad->isLeaf())
		{
			instanceCount = instanceCount + 1;
			_terrainNode->getDeformation()->setUniforms(quad, data);

			uint32 level = _elevationDataGenerator->getTile(quad, _transform);

			data.add(Vector4f((float)level, 0.0f, 0.0f, 0.0f));

			/*_terrainNode->getDeformation()->setUniforms(quad, material->getCurrentTechnique()->getPass(0));

			for (uint32 i = 0; i < _tileSamplers.size(); ++i) 
			{
				_tileSamplers[i]->setTile(material, quad->getLevel(), quad->getLogicalXCoordinate(), quad->getLogicalYCoordinate(), 
					quad->getChildIndex());
			}

			mesh->getIndexComponent(0)->getIndexBuffer()->bind();
			glDrawElements(mesh->getIndexComponent(0)->getPrimitive(), mesh->getIndexComponent(0)->getNumIndices(), mesh->getIndexComponent(0)->getIndexFormat(), 0);
			mesh->getIndexComponent(0)->getIndexBuffer()->unbind();*/
		}
		else
		{
			drawQuad(quad->_children[0], data, instanceCount);
			drawQuad(quad->_children[1], data, instanceCount);
			drawQuad(quad->_children[2], data, instanceCount);
			drawQuad(quad->_children[3], data, instanceCount);
			/*int order[4];
			double ox = _terrainNode->getLocalCamera().x;
			double oy = _terrainNode->getLocalCamera().y;

			double cx = quad->ox + q->l / 2.0;
			double cy = q->oy + q->l / 2.0;
			if (oy < cy) {
				if (ox < cx) {
					order[0] = 0;
					order[1] = 1;
					order[2] = 2;
					order[3] = 3;
				}
				else {
					order[0] = 1;
					order[1] = 0;
					order[2] = 3;
					order[3] = 2;
				}
			}
			else {
				if (ox < cx) {
					order[0] = 2;
					order[1] = 0;
					order[2] = 3;
					order[3] = 1;
				}
				else {
					order[0] = 3;
					order[1] = 1;
					order[2] = 2;
					order[3] = 0;
				}
			}

			int done = 0;

			for (int i = 0; i < 4; ++i) {
				if (q->children[order[i]].get())
				{
					if (culling && q->children[order[i]]->visible == SceneManager::INVISIBLE) {
						done |= (1 << order[i]);
					}
					else if (!async || q->children[order[i]]->drawable) {
						drawQuad(q->children[order[i]], uniforms);
						done |= (1 << order[i]);
					}
				}
			}

			if (done < 15) {
				int sizes[16] = { 0, 4, 7, 10, 12, 15, 17, 19, 20, 23, 25, 27, 28, 30, 31, 32 };
				for (unsigned int i = 0; i < uniforms.size(); ++i) {
					uniforms[i]->setTile(q->level, q->tx, q->ty);
				}
				_terrainNode->deform->setUniforms(m_sceneNode, q, _terrainMaterial);
				//SceneManager::getCurrentFrameBuffer()->draw(p, *m, m->mode, gridSize * sizes[done], gridSize * (sizes[done + 1] - sizes[done]));
			}*/
		}
	}

	void Terrain::createAtmosphereTextures()
	{
		if (!_inscatter)
		{
			FileInputStream input;

			Texture::Parameters params;
			params.setMinFilter(TextureFilter::Linear);
			params.setMagFilter(TextureFilter::Linear);
			params.setWrapS(TextureWrap::ClampToEdge);
			params.setWrapT(TextureWrap::ClampToEdge);
			params.setWrapR(TextureWrap::ClampToEdge);

			if (input.open("resources/terrain/atmosphere/inscatter.raw"))
			{
				float* inscatter = new float[32 * 8 * 128 * 32 * 4];
				input.read(inscatter, 32 * 8 * 128 * 32 * 4 * sizeof(float));
				_inscatter = Texture::create3D(Texture::RGBA, 32 * 8, 128, 32, Texture::RGBA16F, Texture::Float, inscatter, params);
				AE_DELETE_ARRAY(inscatter);
			}

			if (input.open("resources/terrain/atmosphere/irradiance.raw"))
			{
				float* irradiance = new float[64 * 16 * 3];
				input.read(irradiance, 64 * 16 * 3 * sizeof(float));
				_irradiance = Texture::create2D(Texture::RGB, 64, 16, Texture::RGB16F, Texture::Float, irradiance, params);
				AE_DELETE_ARRAY(irradiance);
			}

			if (input.open("resources/terrain/atmosphere/transmittance.raw"))
			{
				float* transmittance = new float[256 * 64 * 3];
				input.read(transmittance, 256 * 64 * 3 * sizeof(float));
				_transmittance = Texture::create2D(Texture::RGB, 256, 64, Texture::RGB16F, Texture::Float, transmittance, params);
				AE_DELETE_ARRAY(transmittance);
			}

			Image<uint8> sunglare;
			sunglare.init("resources/terrain/atmosphere/sunglare.png");
			_sunglare = Texture::create2D(Texture::RGBA, sunglare.getWidth(), sunglare.getHeight(), Texture::RGBA8, Texture::UnsignedByte, sunglare.getPixelsPtr(), params);
		}
	}

	void Terrain::createGrid(int32 size, std::vector<Vector3f>& vertices, std::vector<uint32>& indices)
	{
		//size 24

		vertices.resize(size * size);
		int32 i = 0;

		for (int32 row = 0; row < size; row++)
		{
			for (int32 col = 0; col < size; col++)
			{
				vertices[i++] = Vector3f(row, col, 0.0f) / (float)(size - 1);
			}
		}

		indices.resize((size * size) + (size - 1) * (size - 2));
		i = 0;

		for (int32 row = 0; row < size - 1; row++)
		{
			if ((row & 1) == 0)
			{
				for (int32 col = 0; col < size; col++)
				{
					indices[i++] = col + row * size;
					indices[i++] = col + (row + 1) * size;
				}
			}
			else
			{ // odd rows
				for (int32 col = size - 1; col > 0; col--)
				{
					indices[i++] = col + (row + 1) * size;
					indices[i++] = col - 1 + +row * size;
				}
			}
		}

		if ((size & 1) && size > 2)
		{
			indices[i++] = (size - 1) * size;
		}
	}
}
