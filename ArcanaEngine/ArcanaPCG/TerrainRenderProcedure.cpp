#include "TerrainRenderProcedure.h"

#include "Profiler.h"

namespace Arcana
{
	TerrainRenderProcedure::TerrainRenderProcedure(Terrain* terrain)
		: _tempTerrain(terrain), _data(nullptr), _mesh(nullptr), _terrainMaterial(nullptr)
	{
		if (_tempTerrain)
		{
			_tempTerrain->reference();
		}
	}

	TerrainRenderProcedure::~TerrainRenderProcedure()
	{
		if (_data)
		{
			AE_DELETE(_data);
		}

		if (_mesh)
		{
			AE_DELETE(_mesh);
		}

		if (_terrainMaterial)
		{
			AE_RELEASE(_terrainMaterial);
		}
	}

	bool TerrainRenderProcedure::isDirty() const
	{
		return true;
	}

	void TerrainRenderProcedure::markDirty(bool dirty)
	{
		//todo
	}

	void TerrainRenderProcedure::createRenderData()
	{
		if (_data)
		{
			AE_DELETE(_data);
		}
		if (_mesh)
		{
			AE_DELETE(_mesh);
		}
		if (_terrainMaterial)
		{
			AE_RELEASE(_terrainMaterial);
		}

		_data = new TerrainRenderData(_tempTerrain);

		_terrainMaterial = new Material("terrain");
		Shader shader;
		shader.createProgram(Shader::Vertex, "resources/terrain_vert.glsl");
		shader.createProgram(Shader::Fragment, "resources/terrain_frag.glsl");

		Technique* technique = new Technique(shader);
		_terrainMaterial->addTechnique(technique);

		_terrainRenderState.setCullEnabled(true);
		_terrainRenderState.setDepthTestEnabled(true);
		_terrainRenderState.setBlendEnabled(true);
		_terrainRenderState.setBlendSrc(RenderState::Blend::SrcAlpha);
		_terrainRenderState.setBlendDst(RenderState::Blend::OneMinusSrcAlpha);

		VertexFormat::Attribute attribs[] =
		{
			VertexFormat::Attribute(VertexFormat::Semantic::Position, 3),
		};
		VertexFormat format(1, attribs);
		_mesh = new Mesh(format, Mesh::TriangleStrip);

		float vertices[] =
		{
			0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f,
			1.0f, 1.0f, 0.0f,
			1.0f, 0.0f, 0.0f
		};
		_mesh->setVertexBuffer(format, 4)->setVertexData(vertices);

		AE_RELEASE(_tempTerrain);
	}

	void TerrainRenderProcedure::updateRenderData(const RenderDataUpdate& data)
	{
		if (isValidProcedure())
		{
			_data->_context.material = _terrainMaterial;
			_data->_context.mesh = _mesh;
			_data->_context.eyePosition = data.eyePosition;
			_data->_context.viewMatrix = data.view;
			_data->_context.projectionMatrix = data.projection;

			updateTerrain();
		}
	}

	RenderData* TerrainRenderProcedure::getRenderData() const
	{
		return _data;
	}

	bool TerrainRenderProcedure::isValidProcedure()
	{
		return _data != nullptr && _data->_terrain != nullptr;
	}

	void TerrainRenderProcedure::updateTerrain()
	{
		{
			PROFILE("TerrainNode update");
			_data->_terrain->_terrainNode->update(
				_data->_context.projectionMatrix, 
				_data->_context.viewMatrix, 
				_data->_context.eyePosition);
		}
		{
			PROFILE("TerrainNode setUniforms");
			_data->_terrain->_terrainNode->getDeformation()->setUniforms(
				_data->_context.projectionMatrix, _data->_context.viewMatrix, _data->_context.eyePosition,
				_data->_terrain->_terrainNode, _data->_context.material);
		}

		/*{
			PROFILE("Terrain getTerrainQuadVector");
			_data->_meshQueueMutex.lock();
			_data->_terrain->getTerrainQuadVector(_data->_meshes, _data->_context);
			_data->_meshQueueMutex.unlock();
		}*/
	}


	TerrainRenderData::TerrainRenderData(Terrain* terrain) : _terrain(terrain)
	{
		_terrain->reference();
	}

	TerrainRenderData::~TerrainRenderData()
	{
		AE_RELEASE(_terrain);
	}

	void TerrainRenderData::render(ObjectRenderer& renderer)
	{
		/*PROFILE("Queuing Meshes");

		renderer._queuedMeshes.clear();

		_meshQueueMutex.lock();
		while(!_meshes.empty())
		{
			MeshRenderContext& mesh = _meshes.back();
			_meshes.pop();
			renderer.queueMesh(mesh);
		}
		_meshQueueMutex.unlock();*/

		//{
			//PROFILE("Terrain getTerrainQuadVector");
			//_terrain->getTerrainQuadVector(_data->_meshes, _data->_context);
		//}


		_context.renderState.bind();
		_context.mesh->getVertexBuffer()->bind();

		Technique* technique = _context.material->getCurrentTechnique();
		if (technique)
		{
			for (uint32 i = 0; i < technique->getPassCount(); i++)
			{
				Shader* pass = technique->getPass(i);
				if (pass)
				{
					pass->bind();

					pass->getUniform("u_ProjectionMatrix")->setValue(_context.projectionMatrix);
					pass->getUniform("u_ViewMatrix")->setValue(_context.viewMatrix);
					pass->getUniform("u_ModelMatrix")->setValue(_context.transform.getMatrix().cast<float>());
					pass->getUniform("u_CameraPosition")->setValue(_context.eyePosition.cast<float>());

					for (uint32 j = 0; j < _context.uniforms.size(); j++)
					{
						pass->getUniform(_context.uniforms[i].name)->setValue(_context.uniforms[i].value);
					}

					//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
					//glDrawArrays(_context.mesh->getPrimitive(), 0, _context.mesh->getNumVertices());

					_terrain->getTerrainQuadVector(_context);

					pass->unbind();
				}
			}
		}

		_context.mesh->getVertexBuffer()->unbind();
		_context.renderState.unbind();
	}
}
