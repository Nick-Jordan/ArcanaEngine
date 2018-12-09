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

		enum Visibility
		{
			PartiallyVisible,
			FullyVisible,
			Invisible,
		};


		TerrainQuad(TerrainNode *owner, int32 tx, int32 ty, double ox, double oy, double l, float zmin, float zmax, int32 childIndex = -1, const TerrainQuad* parent = nullptr);

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

		const int32 getChildIndex() const;

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

		int32 _childIndex;

		TerrainQuad* _children[4];

		Visibility _visible; // visibility enum

		bool _occluded;

		bool _drawable;

		TerrainNode* _owner;
	};

}

#endif // !TERRAIN_QUAD_H_

