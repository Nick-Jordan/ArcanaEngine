#include "TerrainRenderProcedure.h"

#include "Profiler.h"
#include "MeshIndexComponent.h"

namespace Arcana
{
	TerrainRenderProcedure::TerrainRenderProcedure(Terrain* terrain, const Transform& transform, std::string vertex, std::string fragment)
		: _tempTerrain(terrain), _data(nullptr), _mesh(nullptr), _terrainMaterial(nullptr), _transform(transform), _vertex(vertex), _fragment(fragment)
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
		shader.createProgram(Shader::Vertex, _vertex);
		Shader::Defines defines;
		defines.addDefine("DEBUG_QUADTREE");
		shader.createProgram(Shader::Fragment, _fragment, defines);

		Technique* technique = new Technique(shader);
		_terrainMaterial->addTechnique(technique);

		_terrainRenderState.setCullEnabled(true);
		_terrainRenderState.setCullFaceSide(RenderState::Back);
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

		/*float vertices[] =
		{
			0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f,
			1.0f, 1.0f, 0.0f,
			1.0f, 0.0f, 0.0f
		};*/


		std::vector<Vector3f> vertices;
		std::vector<uint32> indices;
		Terrain::createGrid(25, vertices, indices);
		_mesh->setVertexBuffer(format, vertices.size())->setVertexData(&vertices[0]);
		_mesh->addIndexComponent(Mesh::TriangleStrip)->setIndexBuffer(IndexBuffer::Index32, indices.size(), false, &indices[0]);

		Image<uint8> image0;
		image0.init("resources/terrain/terrain_texture.png");
		Texture::Parameters params;
		params.setWrapS(TextureWrap::Repeat);
		params.setWrapT(TextureWrap::Repeat);
		_data->_terrainSurface = Texture::create2D(Texture::RGBA, 1024, 1024, Texture::RGBA8, Texture::UnsignedByte, image0.getPixelsPtr(), params);

		Image<uint8> image1;
		image1.init("resources/terrain/terrain_color.png");
		_data->_terrainColor = Texture::create2D(Texture::RGBA, 256, 256, Texture::RGBA8, Texture::UnsignedByte, image1.getPixelsPtr(), params);

		AE_RELEASE(_tempTerrain);
	}

	void TerrainRenderProcedure::updateRenderData(const RenderDataUpdate& data)
	{
		PROFILE("Update Terrain Render Data");

		if (isValidProcedure())
		{
			_data->_context.mesh = _mesh;
			_data->_context.eyePosition = data.eyePosition;
			_data->_context.viewMatrix = data.view;
			_data->_context.projectionMatrix = data.projection;
			_data->_context.renderState = _terrainRenderState;

			_data->_context.transform = _transform;

			_data->_context.rendererStage = "TransparentObjectStage";

			if (!_data->_context.callback.isBound())
			{
				_data->_context.callback.bind(_data, &TerrainRenderData::renderTerrain);
			}

			_data->_terrainMaterial = _terrainMaterial;

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
				_data->_context.transform.getMatrix(),
				_data->_context.projectionMatrix,
				_data->_context.viewMatrix,
				_data->_context.eyePosition);
		}
		{
			PROFILE("TerrainNode setUniforms");
			_data->_terrain->_terrainNode->getDeformation()->setUniforms(
				_data->_context.transform.getMatrix(),
				_data->_context.projectionMatrix, _data->_context.viewMatrix, _data->_context.eyePosition,
				_data->_terrain->_terrainNode, _terrainMaterial->getCurrentTechnique()->getPass(0));
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
		renderer.addMesh(_context);
	}

	void TerrainRenderData::renderTerrain()
	{
		PROFILE("Render Terrain");

		_context.renderState.bind();
		_context.mesh->getVertexBuffer()->bind();

		uint32 componentCount = 1;

		for (uint32 c = 0; c < componentCount; c++)
		{
			Technique* technique = _terrainMaterial->getTechnique(c);
			if (!technique)
			{
				technique = _terrainMaterial->getCurrentTechnique();
			}

			if (technique)
			{
				MeshIndexComponent* component = _context.mesh->getIndexComponent(c);
				for (uint32 i = 0; i < technique->getPassCount(); i++)
				{
					Shader* pass = technique->getPass(i);
					if (pass)
					{
						pass->bind();

						//Default Uniforms
						pass->getUniform("u_ModelMatrix").setValue(_context.transform.getMatrix().cast<float>());
						pass->getUniform("u_CameraPosition").setValue(_context.eyePosition.cast<float>());
						//pass->getUniform("u_ProjectionMatrix").setValue(_context.projectionMatrix.cast<float>());
						//pass->getUniform("u_ViewMatrix").setValue(_context.viewMatrix.cast<float>());

						_terrain->getTerrainQuadVector(_context, _terrainMaterial);

						pass->unbind();
					}
				}
			}
		}

		_context.mesh->getVertexBuffer()->unbind();
		_context.renderState.unbind();
	}
}
