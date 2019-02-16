#include "TerrainQuad.h"

#include "TerrainNode.h"
#include "Tile.h"
#include <algorithm>

#include "TerrainTile.h"

namespace Arcana
{
	std::map<Tile::Id, TerrainTile*> tilemap;

	TerrainQuad::TerrainQuad(TerrainNode* owner, int32 tx, int32 ty, double ox, double oy, double l, float zmin, float zmax, int32 childIndex, const TerrainQuad* parent)
		: _owner(owner), _tx(tx), _ty(ty), _ox(ox), _oy(oy), _l(l), _zmin(zmin), _zmax(zmax), _parent(parent), _childIndex(childIndex),
		_level(parent == nullptr ? 0 : parent->_level + 1), _occluded(false), _drawable(true), _visible(PartiallyVisible)
	{
		_children[0] = nullptr;
		_children[1] = nullptr;
		_children[2] = nullptr;
		_children[3] = nullptr;

		/*Tile::Id id = Tile::Id(0, _level, tx, ty);
		std::map<Tile::Id, TerrainTile*>::iterator iter = tilemap.find(id);
		if (iter == tilemap.end())
		{
			_tile = new TerrainTile();
			_tile->generate(TerrainTileProceduralParameters(this, Matrix4d::IDENTITY, 6361000.0), Seed());
			tilemap.emplace(id, _tile);
		}
		else
		{
			_tile = iter->second;
		}*/
	}

	TerrainQuad::~TerrainQuad()
	{
		if (!isLeaf())
		{
			AE_DELETE(_children[0]);
			AE_DELETE(_children[1]);
			AE_DELETE(_children[2]);
			AE_DELETE(_children[3]);
		}

		//AE_DELETE(_tile);
	}

	TerrainNode* TerrainQuad::getOwner()
	{
		return _owner;
	}

	bool TerrainQuad::isLeaf() const
	{
		return _children[0] == nullptr;
	}

	int32 TerrainQuad::getSize() const
	{
		int32 s = 1;

		if (isLeaf())
		{
			return s;
		}

		return s + _children[0]->getSize() + _children[1]->getSize() +
			_children[2]->getSize() + _children[3]->getSize();
	}

	int32 TerrainQuad::getDepth() const
	{
		if (isLeaf()) 
		{
			return _level;
		}

		return (std::max)((std::max)(_children[0]->getDepth(), _children[1]->getDepth()),
			(std::max)(_children[2]->getDepth(), _children[3]->getDepth()));
	}

	void TerrainQuad::update()
	{
		Visibility v = _parent == nullptr ? PartiallyVisible : _parent->_visible;
		if (v == PartiallyVisible)
		{
			AxisAlignedBoundingBoxd localBox;
			localBox.set(_ox, _oy, _zmin, _ox + _l, _oy + _l, _zmax);
			_visible = _owner->getVisibility(localBox);
		}
		else 
		{
			_visible = v;
		}

		if (_visible != Invisible && _occluded) 
		{
			AxisAlignedBoundingBoxd localBox;
			localBox.set(_ox, _oy, _zmin, _ox + _l, _oy + _l, _zmax);
			_occluded = _owner->isOccluded(localBox);
			if (_occluded) 
			{
				_visible = Invisible;
			}
		}

		double ground = 0.0;//TerrainNode::groundHeightAtCamera;
		AxisAlignedBoundingBoxd box;
		box.set(_ox, _oy, (std::min)(0.0, ground), _ox + _l, _oy + _l, (std::max)(0.0, ground));

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
			if (!isLeaf()) 
			{
				AE_DELETE(_children[0]);
				AE_DELETE(_children[1]);
				AE_DELETE(_children[2]);
				AE_DELETE(_children[3]);
			}
		}
	}

	const TerrainQuad* TerrainQuad::getParent() const
	{
		return _parent;
	}

	const int32 TerrainQuad::getLevel() const
	{
		return _level;
	}

	const int32 TerrainQuad::getLogicalXCoordinate() const
	{
		return _tx;
	}

	const int32 TerrainQuad::getLogicalYCoordinate() const
	{
		return _ty;
	}

	const double TerrainQuad::getPhysicalXCoordinate() const
	{
		return _ox;
	}

	const double TerrainQuad::getPhysicalYCoordinate() const
	{
		return _oy;
	}
	const double TerrainQuad::getPhysicalLevel() const
	{
		return _l;
	}

	const int32 TerrainQuad::getChildIndex() const
	{
		return _childIndex;
	}

	float TerrainQuad::getMinZ() const
	{
		return _zmin;
	}

	float TerrainQuad::getMaxZ() const
	{
		return _zmax;
	}

	void TerrainQuad::subdivide()
	{
		float hl = (float)_l / 2.0f;
		_children[0] = new TerrainQuad(_owner, 2 * _tx, 2 * _ty, _ox, _oy, hl, _zmin, _zmax, 0, this);
		_children[1] = new TerrainQuad(_owner, 2 * _tx + 1, 2 * _ty, _ox + hl, _oy, hl, _zmin, _zmax, 1, this);
		_children[2] = new TerrainQuad(_owner, 2 * _tx, 2 * _ty + 1, _ox, _oy + hl, hl, _zmin, _zmax, 2, this);
		_children[3] = new TerrainQuad(_owner, 2 * _tx + 1, 2 * _ty + 1, _ox + hl, _oy + hl, hl, _zmin, _zmax, 3, this);
	}
}
