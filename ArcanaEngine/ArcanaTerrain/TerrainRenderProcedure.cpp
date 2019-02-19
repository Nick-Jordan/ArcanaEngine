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
		//defines.addDefine("DEBUG_QUADTREE");
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
		createGrid(vertices, indices);
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
			_data->_context.material = _terrainMaterial;
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
				_data->_terrain->_terrainNode, _data->_context.material->getCurrentTechnique()->getPass(0));
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

		Technique* technique = _context.material->getCurrentTechnique();
		if (technique)
		{
			for (uint32 i = 0; i < technique->getPassCount(); i++)
			{
				Shader* pass = technique->getPass(i);
				if (pass)
				{
					{
						PROFILE("Binding Shader and Setting Uniforms");
						pass->bind();

						pass->getUniform("u_ProjectionMatrix").setValue(_context.projectionMatrix.cast<float>());
						pass->getUniform("u_ViewMatrix").setValue(_context.viewMatrix.cast<float>());
						pass->getUniform("u_ModelMatrix").setValue(_context.transform.getMatrix().cast<float>());
						pass->getUniform("u_CameraPosition").setValue(_context.eyePosition.cast<float>());

						int32 unit = _terrainSurface->bind(_context.material);
						pass->getUniform("u_TerrainSurface").setValue(unit);
						unit = _terrainColor->bind(_context.material);
						pass->getUniform("u_TerrainColor").setValue(unit);

						unit = Terrain::_inscatter->bind(_context.material);
						pass->getUniform("inscatterSampler").setValue(unit);
						unit = Terrain::_irradiance->bind(_context.material);
						pass->getUniform("skyIrradianceSampler").setValue(unit);
						unit = Terrain::_transmittance->bind(_context.material);
						pass->getUniform("transmittanceSampler").setValue(unit);

						pass->getUniform("u_Time").setValue(t);
						t += 0.016;
						for (uint32 j = 0; j < _context.uniforms.size(); j++)
						{
							pass->getUniform(_context.uniforms[i].name).setValue(_context.uniforms[i].value);
						}
					}

					//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
					//glDrawArrays(_context.mesh->getPrimitive(), 0, _context.mesh->getNumVertices());


					{
						PROFILE("Render Terrain Quads");
						_terrain->getTerrainQuadVector(_context);
					}

					pass->unbind();
				}
			}
		}

		_context.mesh->getVertexBuffer()->unbind();
		_context.renderState.unbind();
	}

	void TerrainRenderProcedure::createGrid(std::vector<Vector3f>& vertices, std::vector<unsigned int>& indices)
	{
		//size 24

		int32 size = 25;

		vertices.resize(size*size);
		int i = 0;

		for (int32 row = 0; row < size; row++) 
		{
			for (int32 col = 0; col < size; col++) 
			{
				vertices[i++] = Vector3f(row, col, 0.0f) / (size - 1);
			}
		}

		indices.resize((size*size) + (size - 1)*(size - 2));
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
