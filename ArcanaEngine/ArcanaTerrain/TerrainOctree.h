#ifndef TERRAIN_OCTREE_QUAD_H_
#define TERRAIN_OCTREE_QUAD_H_

#include "TerrainDefines.h"

#include "Object.h"
#include "AxisAlignedBoundingBox.h"

#include "Mesh.h"
#include "GlobalObjectID.h"
#include "TerrainOctreeData.h"

namespace Arcana
{
	class ARCANA_TERRAIN_API VoxelTerrain;

	class ARCANA_TERRAIN_API TerrainOctree : public Object
	{
	public:

		enum Visibility
		{
			PartiallyVisible,
			FullyVisible,
			Invisible,
		};


		TerrainOctree(VoxelTerrain* owner, int32 tx, int32 ty, int32 tz, double ox, double oy, double oz, double l, int32 maxLevel, int32 childIndex = -1, const TerrainOctree* parent = nullptr);

		virtual ~TerrainOctree();

		VoxelTerrain* getOwner();

		bool isLeaf() const;

		int32 getSize() const;

		int32 getDepth() const;

		void update();

		void subdivide();

		void merge();

		const TerrainOctree* getParent() const;

		TerrainOctreeData* getData() const;

		TerrainOctree* getChild(int32 i) const;

		const int32 getLevel() const;

		const int32 getLogicalXCoordinate() const;

		const int32 getLogicalYCoordinate() const;

		const int32 getLogicalZCoordinate() const;

		const double getPhysicalXCoordinate() const;

		const double getPhysicalYCoordinate() const;

		const double getPhysicalZCoordinate() const;

		const double getPhysicalSize() const;

		const int32 getChildIndex() const;

		const UUID getId() const;

	private:

		UUID _id;

		const TerrainOctree* _parent;
		const int32 _level;
		const int32 _tx;
		const int32 _ty;
		const int32 _tz;
		const double _ox;
		const double _oy;
		const double _oz;
		const double _l;
		const int32 _maxLevel;

		int32 _childIndex;
		TerrainOctree* _children[8];
		Visibility _visible; // visibility enum
		bool _occluded;
		bool _drawable;
		VoxelTerrain* _owner;

		TerrainOctreeData* _data;
	};

}

#endif // !TERRAIN_QUAD_H_

