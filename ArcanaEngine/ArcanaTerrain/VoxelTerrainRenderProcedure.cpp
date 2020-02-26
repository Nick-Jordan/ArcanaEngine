#include "VoxelTerrainRenderProcedure.h"

#include "MeshIndexComponent.h"
#include "MarchingCubes.h"

#include "Noise.h"

namespace Arcana
{
	VoxelTerrainRenderProcedure::VoxelTerrainRenderProcedure()
		: _terrainMaterial(nullptr), _mesh(nullptr)
	{
		_terrainMaterial = new Material("terrain");
		Shader shader;
		shader.createProgram(Shader::Vertex, "resources/terrain/voxel_terrain_vert.glsl");
		Shader::Defines defines;
		//defines.addDefine("DEBUG_QUADTREE");
		shader.createProgram(Shader::Fragment, "resources/terrain/voxel_terrain_frag.glsl", defines);

		Technique* technique = new Technique(shader);
		_terrainMaterial->addTechnique(technique);

		
		VertexFormat::Attribute attribs[] =
		{
			VertexFormat::Attribute(VertexFormat::Semantic::Position, 3),
			VertexFormat::Attribute(VertexFormat::Semantic::Normal, 3),
		};
		VertexFormat format(2, attribs);
		_mesh = new Mesh(format, Mesh::TriangleStrip);

		//std::vector<Vector3f> vertices = {Vector3f(0, 0, 0), Vector3f(0, 1, 0), Vector3f(1, 0, 0), Vector3f(1, 1, 0)};
		//std::vector<uint32> indices = {0, 1, 2, 3};
		int size = 128;
		float* stuff = new float[size * size * size];
		for (int i = 0; i < size * size * size; i++)
			stuff[i] = 0.0;
		/*for (int x = 1; x < size - 1; x++)
		{
			for (int y = 1; y < size - 1; y++)
			{
				for (int z = 1; z < size - 1; z++)
				{
					int x1 = x - size / 2;
					int y1 = y - size / 2;
					int z1 = z - size / 2;
					if (Noise::noise(4, 0.6, 0.01, x1, y1, z1) >= 0.0)
					{
						stuff[x + size * (y + size * z)] = 1.0f;
					}
				}
			}
		}*/

		for (int x = 0; x < size; x++)
		{
			for (int z = 0; z < size; z++)
			{
				int x1 = x - size / 2;
				int z1 = z - size / 2;
				int h = (int)((Noise::noise(4, 0.6, 0.01, x1, z1) * 0.5 + 0.5) * (double)(size/2-1));

				for (int y = h; y >= 0; y--)
				{
					int y1 = y - size / 2;

					if (Noise::noise(4, 0.6, 0.01, x1, y1, z1) < 0.0 || y > h-4)
					{
						stuff[x + size * (y + size * z)] = 1.0f;
					}
					else
					{
						stuff[x + size * (y + size * z)] = 0.0f;
					}
				}
			}
		}

		MarchingCubes<float> cubes;
		std::vector<Vector3f> verts;
		std::vector<Quad> quads;
		cubes.build(stuff, size, size, size, 0.5f, true, false, verts, quads);
		delete[] stuff;

		std::vector<Vector3f> normals(verts.size());

		std::vector<uint32> indices(quads.size() * 8);

		for (int i = 0; i < quads.size(); i++)
		{
			//triangles
			indices[i * 6 + 0] = quads[i].i0;
			indices[i * 6 + 1] = quads[i].i1;
			indices[i * 6 + 2] = quads[i].i2;
			indices[i * 6 + 3] = quads[i].i2;
			indices[i * 6 + 4] = quads[i].i3;
			indices[i * 6 + 5] = quads[i].i0;

			//lines
			/*indices[i * 8 + 0] = quads[i].i0;
			indices[i * 8 + 1] = quads[i].i1;
			indices[i * 8 + 2] = quads[i].i1;
			indices[i * 8 + 3] = quads[i].i2;

			indices[i * 8 + 4] = quads[i].i2;
			indices[i * 8 + 5] = quads[i].i3;
			indices[i * 8 + 6] = quads[i].i3;
			indices[i * 8 + 7] = quads[i].i0;*/

			Vector3f v0 = verts[quads[i].i0];
			Vector3f v1 = verts[quads[i].i1];
			Vector3f v2 = verts[quads[i].i2];
			Vector3f v3 = verts[quads[i].i3];
			Vector3f n1 = Vector3f::cross(v1 - v0, v2 - v0);
			Vector3f n2 = Vector3f::cross(v2 - v0, v3 - v0);

			normals[quads[i].i0] += n1;
			normals[quads[i].i1] += n1;
			normals[quads[i].i2] += n1;
			normals[quads[i].i2] += n2;
			normals[quads[i].i3] += n2;
			normals[quads[i].i0] += n2;
		}

		std::vector<Vector3f> vertices(verts.size() * 2);
		for (int i = 0; i < verts.size(); i++)
		{
			vertices[i * 2 + 0] = verts[i];
			vertices[i * 2 + 1] = Vector3f::normalize(normals[i]);
		}

		_mesh->setVertexBuffer(format, verts.size())->setVertexData(&vertices[0]);
		_mesh->addIndexComponent(Mesh::Triangles)->setIndexBuffer(IndexBuffer::Index32, indices.size(), false, &indices[0]);

		Properties.LightProperties.CastsDynamicShadow = false;
		Properties.RendererStage = "TransparentObjectStage";//environment
		Properties.RenderState.setCullEnabled(true);
		Properties.RenderState.setCullFaceSide(RenderState::Back);
		Properties.RenderState.setDepthTestEnabled(true);
		Properties.RenderState.setBlendEnabled(true);
		Properties.RenderState.setBlendSrc(RenderState::Blend::SrcAlpha);
		Properties.RenderState.setBlendDst(RenderState::Blend::OneMinusSrcAlpha);
	}

		VoxelTerrainRenderProcedure::~VoxelTerrainRenderProcedure()
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

	void VoxelTerrainRenderProcedure::render()
	{
		updateTerrain();
		renderTerrain();
	}

	bool VoxelTerrainRenderProcedure::isValidProcedure()
	{
		return _mesh != nullptr && _terrainMaterial != nullptr;
	}

	void VoxelTerrainRenderProcedure::updateTerrain()
	{
		
	}

	void VoxelTerrainRenderProcedure::renderTerrain()
	{
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

						component->getIndexBuffer()->bind();
						glDrawElements(component->getPrimitive(), component->getNumIndices(), component->getIndexFormat(), 0);
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
