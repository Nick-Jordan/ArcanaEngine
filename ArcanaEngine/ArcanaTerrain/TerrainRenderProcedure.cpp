#include "TerrainRenderProcedure.h"

#include "Profiler.h"
#include "MeshIndexComponent.h"

namespace Arcana
{
	TerrainRenderProcedure::TerrainRenderProcedure(Terrain* terrain, std::string vertex, std::string fragment)
		: _terrain(terrain), _mesh(nullptr), _terrainMaterial(nullptr), _vertex(vertex), _fragment(fragment)
	{
		AE_REFERENCE(_terrain);

		_terrainMaterial = new Material("terrain");
		Shader shader;
		shader.createProgram(Shader::Vertex, _vertex);
		Shader::Defines defines;
		//defines.addDefine("DEBUG_QUADTREE");
		shader.createProgram(Shader::Fragment, _fragment, defines);

		Technique* technique = new Technique(shader);
		_terrainMaterial->addTechnique(technique);

		VertexFormat::Attribute attribs[] =
		{
			VertexFormat::Attribute(VertexFormat::Semantic::Position, 3),
		};
		VertexFormat format(1, attribs);
		_mesh = new Mesh(format, Mesh::TriangleStrip);

		std::vector<Vector3f> vertices;
		std::vector<uint32> indices;
		Terrain::createGrid(25, vertices, indices);
		_mesh->setVertexBuffer(format, vertices.size())->setVertexData(&vertices[0]);
		_mesh->addIndexComponent(Mesh::TriangleStrip)->setIndexBuffer(IndexBuffer::Index32, indices.size(), false, &indices[0]);

		VertexFormat::Attribute instanceAttribs[] =
		{
			VertexFormat::Attribute(VertexFormat::Semantic::Position, 4), //offset
			VertexFormat::Attribute(VertexFormat::Semantic::Position, 4), //camera
			VertexFormat::Attribute(VertexFormat::Semantic::Position, 4), //sceenQuadCorners
			VertexFormat::Attribute(VertexFormat::Semantic::Position, 4),
			VertexFormat::Attribute(VertexFormat::Semantic::Position, 4),
			VertexFormat::Attribute(VertexFormat::Semantic::Position, 4),
			VertexFormat::Attribute(VertexFormat::Semantic::Position, 4), //sceenQuadVerticals
			VertexFormat::Attribute(VertexFormat::Semantic::Position, 4),
			VertexFormat::Attribute(VertexFormat::Semantic::Position, 4),
			VertexFormat::Attribute(VertexFormat::Semantic::Position, 4),
			VertexFormat::Attribute(VertexFormat::Semantic::Position, 4), //screenQuadCornerNorms
			/*VertexFormat::Attribute(VertexFormat::Semantic::Position, 4), //tangentFrameToWorld
			VertexFormat::Attribute(VertexFormat::Semantic::Position, 4),
			VertexFormat::Attribute(VertexFormat::Semantic::Position, 4),
			VertexFormat::Attribute(VertexFormat::Semantic::Position, 4),*/
			VertexFormat::Attribute(VertexFormat::Semantic::Position, 4) //Tile ids
			///add id and parent id (to access texture array)
		};
		_instanceFormat = VertexFormat(12, instanceAttribs, 1);
		_mesh->setInstanceBuffer(_instanceFormat, 0, true);

		Image<uint8> image0;
		image0.init("resources/terrain/terrain_texture.png");
		Texture::Parameters params;
		params.setWrapS(TextureWrap::Repeat);
		params.setWrapT(TextureWrap::Repeat);
		_terrainSurface = Texture::create2D(Texture::RGBA, 1024, 1024, Texture::RGBA8, Texture::UnsignedByte, image0.getPixelsPtr(), params);

		Image<uint8> image1;
		image1.init("resources/terrain/terrain_color.png");
		_terrainColor = Texture::create2D(Texture::RGBA, 256, 256, Texture::RGBA8, Texture::UnsignedByte, image1.getPixelsPtr(), params);

		Properties.LightProperties.CastsDynamicShadow = false;
		Properties.RendererStage = "TransparentObjectStage";//environment
		Properties.RenderState.setCullEnabled(true);
		Properties.RenderState.setCullFaceSide(RenderState::Back);
		Properties.RenderState.setDepthTestEnabled(true);
		Properties.RenderState.setBlendEnabled(true);
		Properties.RenderState.setBlendSrc(RenderState::Blend::SrcAlpha);
		Properties.RenderState.setBlendDst(RenderState::Blend::OneMinusSrcAlpha);
	}

	TerrainRenderProcedure::~TerrainRenderProcedure()
	{
		if (_mesh)
		{
			AE_DELETE(_mesh);
		}

		if (_terrainMaterial)
		{
			AE_RELEASE(_terrainMaterial);
		}
	}

	void TerrainRenderProcedure::render()
	{
		updateTerrain();
		renderTerrain();
	}

	bool TerrainRenderProcedure::isValidProcedure()
	{
		return _mesh != nullptr && _terrain != nullptr && _terrainMaterial != nullptr;
	}

	void TerrainRenderProcedure::updateTerrain()
	{
		_terrain->_transform = Transform.getMatrix();

		{
			PROFILE("TerrainNode update");
			_terrain->_terrainNode->update(
				Transform.getMatrix(),
				Projection,
				View,
				EyePosition);
		}
		{
			PROFILE("TerrainNode setUniforms");
			_terrain->_terrainNode->getDeformation()->setUniforms(
				Transform.getMatrix(),
				Projection, View, EyePosition,
				_terrain->_terrainNode, _terrainMaterial->getCurrentTechnique()->getPass(0));
		}

		/*{
			PROFILE("Terrain getTerrainQuadVector");
			_data->_meshQueueMutex.lock();
			_data->_terrain->getTerrainQuadVector(_data->_meshes, _data->_context);
			_data->_meshQueueMutex.unlock();
		}*/
	}

	void TerrainRenderProcedure::renderTerrain()
	{
		Array<Vector4f> data;
		int32 instanceCount = 0;

		_terrain->drawTerrain(_terrainMaterial, data, instanceCount);

		_mesh->getInstanceProperties()._numInstances = instanceCount;

		_mesh->getInstanceBuffer()->bind();
		if (instanceCount)
		{
			glBufferData(GL_ARRAY_BUFFER, _instanceFormat.getVertexSize() * instanceCount, &data[0].x, GL_DYNAMIC_DRAW);
		}
		else
		{
			//glMapBuffer(GL_ARRAY_BUFFER, GL_MAP_INVALIDATE_BUFFER_BIT);
		}
		_mesh->getInstanceBuffer()->unbind();


		Properties.RenderState.bind();
		_mesh->getVertexBuffer()->bind();

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
				MeshIndexComponent* component = _mesh->getIndexComponent(c);
				for (uint32 i = 0; i < technique->getPassCount(); i++)
				{
					Shader* pass = technique->getPass(i);
					if (pass)
					{
						pass->bind();

						//Default Uniforms
						pass->getUniform("u_ModelMatrix").setValue(Transform.getMatrix().cast<float>());
						pass->getUniform("u_CameraPosition").setValue(EyePosition.cast<float>());
						pass->getUniform("u_ProjectionMatrix").setValue(Projection.cast<float>());
						pass->getUniform("u_ViewMatrix").setValue(View.cast<float>());

						pass->getUniform("u_WorldSunDir").setValue(Vector3f(0, 0, 1));

						int32 unit = Terrain::_inscatter->bind(_terrainMaterial);
						pass->getUniform("inscatterSampler").setValue(unit);
						unit = Terrain::_irradiance->bind(_terrainMaterial);
						pass->getUniform("skyIrradianceSampler").setValue(unit);
						unit = Terrain::_transmittance->bind(_terrainMaterial);
						pass->getUniform("transmittanceSampler").setValue(unit);

						unit = _terrainColor->bind(_terrainMaterial);
						pass->getUniform("u_TerrainColor").setValue(unit);
						unit = _terrainSurface->bind(_terrainMaterial);
						pass->getUniform("u_TerrainSurface").setValue(unit);

						component->getIndexBuffer()->bind();
						if (instanceCount > 0)
						{
							_mesh->getInstanceBuffer()->bind();
							glDrawElementsInstanced(component->getPrimitive(), component->getNumIndices(), component->getIndexFormat(), 0, instanceCount);
							_mesh->getInstanceBuffer()->unbind();
						}
						component->getIndexBuffer()->unbind();

						pass->unbind();
					}
				}
			}
		}

		_mesh->getVertexBuffer()->unbind();
		Properties.RenderState.unbind();
	}
}
