#include "TerrainOctreeData.h"

#include "MeshIndexComponent.h"
#include "Noise.h"

namespace Arcana
{
	TerrainOctreeDataTask::TerrainOctreeDataTask(Vector3d position, double size) : _position(position), _size(size)
	{

	}

	void TerrainOctreeDataTask::run()
	{
		Result = new MarchingCubesMeshData;

		int size = 16;
		std::vector<float> stuff(size * size * size);

		for (int x = 0; x < size; x++)
		{
			double x1 = (double)x / (double)(size - 1);
			for (int z = 0; z < size; z++)
			{
				double z1 = (double)z / (double)(size - 1);
				for (int y = 0; y < size; y++)
				{
					double y1 = (double)y / (double)(size - 1);

					Vector3d vec = Vector3d(x1 * _size + _position.x, y1 * _size + _position.y, z1 * _size + _position.z);

					if (vec.magnitude() <= 40.0)
					{
						double n = Noise::noise(4, 0.9, 0.1, vec.x, vec.y, vec.z) * 0.5 + 0.5;
						stuff[x + size * (y + size * z)] = n;
					}
				}
			}
		}

		MarchingCubes<float> cubes;
		std::vector<Vector3f> verts;
		std::vector<Quad> quads;
		cubes.build(&stuff[0], size, size, size, 0.5f, true, false, verts, quads);
		std::vector<Vector3f> normals(verts.size());

		Result->indices.resize(quads.size() * 8);

		for (int i = 0; i < quads.size(); i++)
		{
			//triangles
			Result->indices[i * 6 + 0] = quads[i].i0;
			Result->indices[i * 6 + 1] = quads[i].i1;
			Result->indices[i * 6 + 2] = quads[i].i2;
			Result->indices[i * 6 + 3] = quads[i].i2;
			Result->indices[i * 6 + 4] = quads[i].i3;
			Result->indices[i * 6 + 5] = quads[i].i0;

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

		Result->vertices.resize(verts.size() * 2);
		for (int i = 0; i < verts.size(); i++)
		{
			Result->vertices[i * 2 + 0] = verts[i] / (double)size;
			Result->vertices[i * 2 + 1] = Vector3f::normalize(normals[i]);
		}
	}

	TerrainOctreeData::TerrainOctreeData(UUID id, Vector3d position, double size) : _mesh(nullptr), _id(id), _position(position), _size(size)
	{
		//std::vector<Vector3f> vertices = {Vector3f(0, 0, 0), Vector3f(0, 1, 0), Vector3f(1, 0, 0), Vector3f(1, 1, 0)};
		//std::vector<uint32> indices = {0, 1, 2, 3};
	}

	TerrainOctreeData::~TerrainOctreeData()
	{
		AE_DELETE(_mesh);
	}

	void TerrainOctreeData::render(Shader* shader)
	{
		if (_mesh)
		{
			VertexBuffer* vbo = _mesh->getVertexBuffer();
			if (vbo)
			{
				vbo->bind();
				MeshIndexComponent* component = _mesh->getIndexComponent(0);

				component->getIndexBuffer()->bind();
				glDrawElements(component->getPrimitive(), component->getNumIndices(), component->getIndexFormat(), 0);
				component->getIndexBuffer()->unbind();

				vbo->bind();
			}
		}
	}

	void TerrainOctreeData::createMesh(MarchingCubesMeshData& data)
	{
		if (_mesh)
			return;

		VertexFormat::Attribute attribs[] =
		{
			VertexFormat::Attribute(VertexFormat::Semantic::Position, 3),
			VertexFormat::Attribute(VertexFormat::Semantic::Normal, 3),
		};
		VertexFormat format(2, attribs);
		_mesh = new Mesh(format, Mesh::TriangleStrip);

		if (!data.vertices.empty())
		{
			_mesh->setVertexBuffer(format, data.vertices.size() / 2)->setVertexData(&data.vertices[0]);
			_mesh->addIndexComponent(Mesh::Triangles)->setIndexBuffer(IndexBuffer::Index32, data.indices.size(), false, &data.indices[0]);
		}
	}
}