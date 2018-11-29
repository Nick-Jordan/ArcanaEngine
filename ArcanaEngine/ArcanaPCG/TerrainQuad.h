#ifndef TERRAIN_QUAD_H_
#define TERRAIN_QUAD_H_

#include "PCGDefines.h"

#include "Object.h"
#include "AxisAlignedBoundingBox.h"

namespace Arcana
{

	class ARCANA_PCG_API TerrainQuad : public Object
	{
		friend class TerrainNode;
		friend class Terrain;

	public:

		TerrainQuad(TerrainNode *owner, int32 tx, int32 ty, double ox, double oy, double l, float zmin, float zmax, const TerrainQuad* parent = nullptr);

		virtual ~TerrainQuad();

		TerrainNode* getOwner();

		bool isLeaf() const;

		int32 getSize() const;

		int32 getDepth() const;

		void update();

		const TerrainQuad* getParent() const;

		const int32 getLevel() const;

		const int32 getLogicalXCoordinate() const;

		const int32 getLogicalYCoordinate() const;

		const double getPhysicalXCoordinate() const;

		const double getPhysicalYCoordinate() const;

		const double getPhysicalLevel() const;

		float getMinZ() const;

		float getMaxZ() const;

	private:

		void subdivide();

	private:

		const TerrainQuad* _parent;

		const int32 _level;

		const int32 _tx;

		const int32 _ty;

		const double _ox;

		const double _oy;

		const double _l;

		float _zmin;

		float _zmax;

		TerrainQuad* _children[4];

		bool _visible; // visibility enum

		bool _occluded;

		bool _drawable;

		TerrainNode* _owner;
	};

}

#endif // !TERRAIN_QUAD_H_

