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
			_data->_meshes.clear();

			{
				PROFILE("TerrainNode update");
				_data->_terrain->_terrainNode->update(data.projection, data.view, data.eyePosition);
			}
			{
				PROFILE("TerrainNode setUniforms");
				_data->_terrain->_terrainNode->getDeformation()->setUniforms(
					data.projection, data.view, data.eyePosition,
					_data->_terrain->_terrainNode, _terrainMaterial);
			}

			std::vector<TerrainQuadRenderData> terrainQuadVector;
			{
				PROFILE("Terrain getTerrainQuadVector");
				_data->_terrain->getTerrainQuadVector(terrainQuadVector);
			}

			std::vector<TerrainQuadRenderData>::iterator i;
			{
				//PROFILE("Push Mesh Contexts");
				for (i = terrainQuadVector.begin(); i != terrainQuadVector.end(); i++)
				{
					{
						PROFILE("Create Mesh Context");
						MeshRenderContext context;
						context.material = _terrainMaterial;
						context.mesh = _mesh;
						context.renderState = _terrainRenderState;
						context.transform.setIdentity();


						context.uniforms = (*i).uniforms;

						context.projectionMatrix = data.projection;
						context.viewMatrix = data.view;
						context.eyePosition = data.eyePosition;

						_data->_meshes.push_back(context);
					}
				}
			}
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
		std::vector<MeshRenderContext>::iterator i;
		for (i = _meshes.begin(); i != _meshes.end(); i++)
		{
			MeshRenderContext& mesh = *i;

			renderer.queueMesh(mesh);
		}
	}
}
