#include "VoxelTerrain.h"

#include "Input.h"
#include "Noise.h"
#include "MeshIndexComponent.h"

namespace Arcana
{
	VoxelTerrain::VoxelTerrain()
	{
		_scheduler = new Scheduler();

		_octree = new TerrainOctree(this, 0, 0, 0, -50.0, -50.0, -50.0, 100.0, 1);
		//test
		//_octree->subdivide();
	}

	VoxelTerrain::~VoxelTerrain()
	{
		AE_DELETE(_octree);
		AE_DELETE(_scheduler);
	}

	bool updating = true;

	void VoxelTerrain::update(Matrix4d transform, Matrix4d projection, Matrix4d view, Vector3d eyePosition)
	{
		if (updating)
		{
			checkSubdivisions(_octree, eyePosition);
		}

		updating = !Input::isKeyPressed(Keys::K);
	}

	void VoxelTerrain::checkNodeData()
	{
		const int Limit = 1;

		int32 count = 0;
		for (auto it = _terrainDataTasks.begin(); it !=_terrainDataTasks.end();)
		{
			if (count > Limit)
				return;

			auto pair = *it;
			if (pair.second && pair.second->isDone())
			{
				it = _terrainDataTasks.erase(it);

				pair.first->createMesh(*pair.second->get());
				count++;
			}
			else
			{
				it++;
			}
		}
	}

	void VoxelTerrain::createData(TerrainOctreeData* data)
	{
		if (!data)
			return;

		TerrainOctreeDataTask* task = new TerrainOctreeDataTask(data->_position, data->_size);
		_scheduler->schedule(task);
		_terrainDataTasks.emplace(data, task);
	}

	void VoxelTerrain::checkSubdivisions(TerrainOctree* octree, Vector3d position)
	{
		if (octree)
		{
			Vector3d pos = Vector3d(octree->getPhysicalXCoordinate(), octree->getPhysicalYCoordinate(), octree->getPhysicalZCoordinate());

			if (Vector3d::distance(pos, position) < 1.5 * octree->getPhysicalSize())
			{
				octree->subdivide();
				for (int32 i = 0; i < 8; i++)
					checkSubdivisions(octree->getChild(i), position);
			}
			else
			{
				octree->merge();
			}
		}
	}
}
