#include "Terrain.h"

#include "ArcanaLog.h"

namespace Arcana
{
	Terrain::Terrain()
	{
		TerrainQuad* root = new TerrainQuad(nullptr, 0, 0, -100.0, -100.0, 2.0 * 100.0, 0.0f, 1.0, nullptr);
		_terrainNode = new TerrainNode(root, new Deformation(), 2.0, 10);
		_terrainNode->reference();
		/*root->subdivide();
		root->_children[0]->subdivide();
		root->_children[0]->_children[0]->subdivide();
		root->_children[0]->_children[1]->subdivide();
		root->_children[0]->_children[2]->subdivide();
		root->_children[1]->subdivide();
		root->_children[2]->subdivide();*/
		_culling = true;
	}


	Terrain::~Terrain()
	{
		AE_RELEASE(_terrainNode);
	}

	void Terrain::getTerrainQuadVector(std::vector<TerrainQuadRenderData>& terrainQuadVector)
	{
		drawQuad(_terrainNode->getRootQuad(), terrainQuadVector);
	}

	void Terrain::drawQuad(TerrainQuad* quad, std::vector<TerrainQuadRenderData>& terrainQuadVector)
	{

		if (_culling && !quad->_visible) 
		{
			return;
		}
		/*if (async && !q->drawable) {
			return;
		}*/

		if (quad->isLeaf()) 
		{			
			TerrainQuadRenderData quadData;
			_terrainNode->getDeformation()->setUniforms(quad, quadData);
			terrainQuadVector.push_back(quadData);
		}
		else 
		{
			drawQuad(quad->_children[0], terrainQuadVector);
			drawQuad(quad->_children[1], terrainQuadVector);
			drawQuad(quad->_children[2], terrainQuadVector);
			drawQuad(quad->_children[3], terrainQuadVector);
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
