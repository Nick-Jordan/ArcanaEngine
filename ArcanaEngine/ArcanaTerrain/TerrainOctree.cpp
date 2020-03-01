#include "TerrainOctree.h"

#include "VoxelTerrain.h"
#include <algorithm>

namespace Arcana
{
	TerrainOctree::TerrainOctree(VoxelTerrain* owner, int32 tx, int32 ty, int32 tz, double ox, double oy, double oz, double l, int32 maxLevel, int32 childIndex, const TerrainOctree* parent)
		: _owner(owner), _tx(tx), _ty(ty), _tz(tz), _ox(ox), _oy(oy), _oz(oz), _l(l), _parent(parent), _childIndex(childIndex),
		_level(parent == nullptr ? 0 : parent->_level + 1), _maxLevel(maxLevel),_occluded(false), _drawable(true), _visible(PartiallyVisible)
	{
		for(int32 i = 0; i < 8; i++)
			_children[i] = nullptr;

		_id = GlobalObjectID(std::to_string(_level) + std::to_string(tx) + std::to_string(ty) + std::to_string(tz)).getId();

		_data = new TerrainOctreeData(_id, Vector3d(ox, oy, oz), l); //owner->getTransform()
		owner->createData(_data);
	}

	TerrainOctree::~TerrainOctree()
	{
		if (!isLeaf())
		{
			for (int32 i = 0; i < 8; i++)
				AE_DELETE(_children[i])
		}
	}

	VoxelTerrain* TerrainOctree::getOwner()
	{
		return _owner;
	}

	bool TerrainOctree::isLeaf() const
	{
		return _children[0] == nullptr;
	}

	int32 TerrainOctree::getSize() const
	{
		if (isLeaf())
		{
			return 1;
		}

		int32 size = 0;
		for (int32 i = 0; i < 8; i++)
			size += _children[i]->getSize();

		return 1 + size;
	}

	int32 TerrainOctree::getDepth() const
	{
		if (isLeaf())
		{
			return _level;
		}

		int32 depth = _children[0]->getDepth();
		for (int32 i = 1; i < 8; i++)
			depth = Math::max(depth, _children[i]->getDepth());

		return depth;
	}

	void TerrainOctree::update()
	{
		/*Visibility v = _parent == nullptr ? PartiallyVisible : _parent->_visible;
		if (v == PartiallyVisible)
		{
			AxisAlignedBoundingBoxd localBox;
			localBox.set(_ox, _oy, _oz, _ox + _l, _oy + _l, _oz + _l);
			//_visible = _owner->getVisibility(localBox);
		}
		else
		{
			_visible = v;
		}

		if (_visible != Invisible && _occluded)
		{
			AxisAlignedBoundingBoxd localBox;
			localBox.set(_ox, _oy, _oz, _ox + _l, _oy + _l, _oz + _l);
			//_occluded = _owner->isOccluded(localBox);
			if (_occluded)
			{
				_visible = Invisible;
			}
		}

		double ground = 0.0;//TerrainNode::groundHeightAtCamera;
		AxisAlignedBoundingBoxd box;
		box.set(_ox, _oy, _oz, _ox + _l, _oy + _l, _oz + _l);

		double dist = _owner->getCameraDist(box);

		if ((_owner->_splitInvisibleQuads || _visible != Invisible) && dist < _l * _owner->getSplitDistance() && _level < _owner->_maxLevel)
		{
			if (isLeaf())
			{
				subdivide();
			}

			int32 order[4];
			double ox = _owner->getLocalCamera().x;
			double oy = _owner->getLocalCamera().y;
			double cx = this->_ox + _l / 2.0;
			double cy = this->_oy + _l / 2.0;
			if (oy < cy)
			{
				if (ox < cx)
				{
					order[0] = 0;
					order[1] = 1;
					order[2] = 2;
					order[3] = 3;
				}
				else
				{
					order[0] = 1;
					order[1] = 0;
					order[2] = 3;
					order[3] = 2;
				}
			}
			else
			{
				if (ox < cx)
				{
					order[0] = 2;
					order[1] = 0;
					order[2] = 3;
					order[3] = 1;
				}
				else
				{
					order[0] = 3;
					order[1] = 1;
					order[2] = 2;
					order[3] = 0;
				}
			}

			_children[order[0]]->update();
			_children[order[1]]->update();
			_children[order[2]]->update();
			_children[order[3]]->update();

			_occluded = _children[0]->_occluded && _children[1]->_occluded && _children[2]->_occluded && _children[3]->_occluded;
		}
		else
		{
			if (_visible != Invisible)
			{
				AxisAlignedBoundingBoxd box;
				box.set(_ox, _oy, _zmin, _ox + _l, _oy + _l, _zmax);
				_occluded = _owner->addOccluder(box);
				if (_occluded)
				{
					_visible = Invisible;
				}
			}
			merge();
		}*/

		
	}

	const TerrainOctree* TerrainOctree::getParent() const
	{
		return _parent;
	}

	TerrainOctreeData* TerrainOctree::getData() const
	{
		return _data;
	}

	TerrainOctree* TerrainOctree::getChild(int32 i) const
	{
		return _children[i];
	}

	const int32 TerrainOctree::getLevel() const
	{
		return _level;
	}

	const int32 TerrainOctree::getLogicalXCoordinate() const
	{
		return _tx;
	}

	const int32 TerrainOctree::getLogicalYCoordinate() const
	{
		return _ty;
	}

	const int32 TerrainOctree::getLogicalZCoordinate() const
	{
		return _tz;
	}

	const double TerrainOctree::getPhysicalXCoordinate() const
	{
		return _ox;
	}

	const double TerrainOctree::getPhysicalYCoordinate() const
	{
		return _oy;
	}

	const double TerrainOctree::getPhysicalZCoordinate() const
	{
		return _oz;
	}

	const double TerrainOctree::getPhysicalSize() const
	{
		return _l;
	}

	const int32 TerrainOctree::getChildIndex() const
	{
		return _childIndex;
	}

	const UUID TerrainOctree::getId() const
	{
		return _id;
	}

	void TerrainOctree::subdivide()
	{
		if (_level + 1 > _maxLevel)
		{
			return;
		}

		if (isLeaf())
		{
			double hl = _l / 2.0;
			_children[0] = new TerrainOctree(_owner, 2 * _tx, 2 * _ty, 2 * _tz, _ox, _oy, _oz, hl, _maxLevel, 0, this);
			_children[1] = new TerrainOctree(_owner, 2 * _tx + 1, 2 * _ty, 2 * _tz, _ox + hl, _oy, _oz, hl, _maxLevel, 1, this);
			_children[2] = new TerrainOctree(_owner, 2 * _tx, 2 * _ty + 1, 2 * _tz, _ox, _oy + hl, _oz, hl, _maxLevel, 2, this);
			_children[3] = new TerrainOctree(_owner, 2 * _tx, 2 * _ty, 2 * _tz + 1, _ox, _oy, _oz + hl, hl, _maxLevel, 3, this);
			_children[4] = new TerrainOctree(_owner, 2 * _tx + 1, 2 * _ty + 1, 2 * _tz, _ox + hl, _oy + hl, _oz, hl, _maxLevel, 4, this);
			_children[5] = new TerrainOctree(_owner, 2 * _tx + 1, 2 * _ty, 2 * _tz + 1, _ox + hl, _oy, _oz + hl, hl, _maxLevel, 5, this);
			_children[6] = new TerrainOctree(_owner, 2 * _tx, 2 * _ty + 1, 2 * _tz + 1, _ox, _oy + hl, _oz + hl, hl, _maxLevel, 6, this);
			_children[7] = new TerrainOctree(_owner, 2 * _tx + 1, 2 * _ty + 1, 2 * _tz + 1, _ox + hl, _oy + hl, _oz + hl, hl, _maxLevel, 7, this);
		}
	}

	void TerrainOctree::merge()
	{
		if (!isLeaf())
		{
			for (int32 i = 0; i < 8; i++)
				AE_DELETE(_children[i]);
		}
	}
}
