#include "Terrain.h"

#include "ArcanaLog.h"
#include "Profiler.h"
#include "MeshIndexComponent.h"

#include "ElevationProducer.h"
#include "ResidualProducer.h"
#include "TextureTileStorage.h"
#include "SphericalDeformation.h"

namespace Arcana
{
	Terrain::Terrain()
	{
		TerrainQuad* root = new TerrainQuad(nullptr, 0, 0, -6361000.0, -6361000.0, 2.0 * 6361000.0, 0.0f, 1.0);
		_terrainNode = new TerrainNode(root, new Deformation(), 2.0, 20);
		_terrainNode->reference();
		_culling = false;

		scheduler = new Scheduler();

		TextureTileStorage* storage = new TextureTileStorage(128, 1296, Texture::Red, Texture::R32F, Texture::Float, Texture::Parameters());
		TileCache* cache = new TileCache("cache", storage, scheduler);
		ElevationProducer* elevationProducer = new ElevationProducer(cache, 25);
		TileSampler* elevationSampler = new TileSampler("elevationSampler", elevationProducer);
		elevationSampler->reference();
		//_tileSamplers.push(elevationSampler);

		/*TextureTileStorage* surfaceStorage = new TextureTileStorage(128, 1296, Texture::RGBA, Texture::RGBA8, Texture::UnsignedByte, Texture::Parameters());
		TileCache* surfaceCache = new TileCache("surfaceCache", surfaceStorage, scheduler);
		ResidualProducer* surfaceProducer = new ResidualProducer(surfaceCache, "terrain_surface", "png");
		TileSampler* surfaceSampler = new TileSampler("surfaceSampler", surfaceProducer);
		surfaceSampler->reference();
		_tileSamplers.push(surfaceSampler);*/
	}


	Terrain::~Terrain()
	{
		for (auto i = _tileSamplers.createIterator(); i; i++)
		{
			AE_RELEASE(*i);
		}

		AE_RELEASE(_terrainNode);

		AE_RELEASE(scheduler);
	}

	void Terrain::getTerrainQuadVector(const MeshRenderContext& data)
	{
		for (int32 i = 0; i < _tileSamplers.size(); i++)
		{
			TileSampler* u = _tileSamplers[i];

			if (u != nullptr)
			{
				u->update(_terrainNode, data.material);
			}
		}

		drawQuad(_terrainNode->getRootQuad(), data);
	}

	void Terrain::drawQuad(TerrainQuad* quad, const MeshRenderContext& data)
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
			_terrainNode->getDeformation()->setUniforms(quad, data.material);

			for (uint32 i = 0; i < _tileSamplers.size(); ++i) 
			{
				_tileSamplers[i]->setTile(data.material, quad->getLevel(), quad->getLogicalXCoordinate(), quad->getLogicalYCoordinate(), 
					quad->getChildIndex());
			}

			data.mesh->getIndexComponent(0)->getIndexBuffer()->bind();
			glDrawElements(data.mesh->getIndexComponent(0)->getPrimitive(), data.mesh->getIndexComponent(0)->getNumIndices(), data.mesh->getIndexComponent(0)->getIndexFormat(), 0);
			data.mesh->getIndexComponent(0)->getIndexBuffer()->unbind();

			//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			//glDrawArrays(data.mesh->getPrimitive(), 0, data.mesh->getNumVertices());

		}
		else
		{
			drawQuad(quad->_children[0], data);
			drawQuad(quad->_children[1], data);
			drawQuad(quad->_children[2], data);
			drawQuad(quad->_children[3], data);
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
}
