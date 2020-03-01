#include "VoxelTerrainRenderProcedure.h"

#include "MeshIndexComponent.h"
#include "MarchingCubes.h"

#include "Noise.h"

namespace Arcana
{
	VoxelTerrainRenderProcedure::VoxelTerrainRenderProcedure(const class Transform& transform)
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
		int size = 32;
		std::vector<float> stuff(size * size * size);

		for (int x = 0; x < size; x++)
		{
			int x1 = x - size / 2;
			for (int z = 0; z < size; z++)
			{
				int z1 = z - size / 2;
				for (int y = 0; y < size; y++)
				{
					int y1 = y - size / 2;
					Vector3d vec = transform.getMatrix() * Vector3d(x1, y1, z1);
					double n = Noise::noise(4, 0.6, 0.1, vec.x, vec.y, vec.z) * 0.5 + 0.5;

					if(vec.magnitude() <= 48.0)
						stuff[x + size * (y + size * z)] = n;
				}
			}
		}

		MarchingCubes<float> cubes;
		std::vector<Vector3f> verts;
		std::vector<Quad> quads;
		cubes.build(&stuff[0], size, size, size, 0.5f, true, false, verts, quads);
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

		if (!vertices.empty())
		{
			_mesh->setVertexBuffer(format, verts.size())->setVertexData(&vertices[0]);
			_mesh->addIndexComponent(Mesh::Triangles)->setIndexBuffer(IndexBuffer::Index32, indices.size(), false, &indices[0]);
		}

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
		VertexBuffer* buffer = _mesh->getVertexBuffer();

		if (!buffer)
			return;

		buffer->bind();

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
