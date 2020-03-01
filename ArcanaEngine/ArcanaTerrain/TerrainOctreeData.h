#ifndef TERRAIN_OCTREE_DATA_H_
#define TERRAIN_OCTREE_DATA_H_

#include "TerrainDefines.h"

#include "Object.h"
#include "MarchingCubes.h"
#include "ProceduralGenerator.h"
#include "Mesh.h"
#include "UUID.h"
#include "Shader.h"

namespace Arcana
{
	struct TerrainOctreeDataParameters
	{
		Vector3d position;
		double size;
	};

	struct MarchingCubesMeshData
	{
		std::vector<Vector3f> vertices;
		std::vector<uint32> indices;
	};

	class ARCANA_TERRAIN_API TerrainOctreeDataTask : public Task<MarchingCubesMeshData>
	{
	public:

		TerrainOctreeDataTask(Vector3d position, double size);

		virtual void run() override;

		virtual void done() override {};

	private:

		Vector3d _position;
		double _size;
	};

	class ARCANA_TERRAIN_API TerrainOctreeData : public Object
	{
		friend class VoxelTerrain;

	public:

		TerrainOctreeData(UUID id, Vector3d position, double size);

		~TerrainOctreeData();

		void render(Shader* shader);

		void createMesh(MarchingCubesMeshData& data);

	private:

		Mesh* _mesh;
		Vector3d _position;
		double _size;
		UUID _id;
	};
}

#endif // !TERRAIN_OCTREE_DATA_H_