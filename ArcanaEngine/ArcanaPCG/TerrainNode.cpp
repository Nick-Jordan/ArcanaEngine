#include "TerrainNode.h"

#define HORIZON_SIZE 256

namespace Arcana
{
	TerrainNode::TerrainNode(TerrainQuad* root, Deformation* deformation, float splitFactor, int32 maxLevel)
		: _root(root), _deformation(deformation), _splitFactor(splitFactor), _maxLevel(maxLevel)
	{
		_root->reference();
		_deformation->reference();

		_splitInvisibleQuads = false;
		_horizonCulling = true;
		_splitDist = 1.1f;
		root->_owner = this;
		_horizon = new double[HORIZON_SIZE];
	}

	TerrainNode::~TerrainNode()
	{
		AE_RELEASE(_root);
		AE_RELEASE(_deformation);
		AE_DELETE_ARRAY(_horizon);
	}


	Deformation* TerrainNode::getDeformation() const
	{
		return _deformation;
	}

	TerrainQuad* TerrainNode::getRootQuad()
	{
		return _root;
	}

	float TerrainNode::getSplitFactor() const
	{
		return _splitFactor;
	}

	bool TerrainNode::isCullingHorizon() const
	{
		return _horizonCulling;
	}

	int32 TerrainNode::getMaxLevel() const
	{
		return _maxLevel;
	}

	Vector3d TerrainNode::getDeformedCamera() const
	{
		return _deformedCameraPos;
	}

	const Planef* TerrainNode::getDeformedFrustumPlanes() const
	{
		return _deformedFrustumPlanes;
	}

	Vector3d TerrainNode::getLocalCamera() const
	{
		return _localCameraPos;
	}

	double TerrainNode::getCameraDist(const AxisAlignedBoundingBoxd &localBox) const
	{
		double dist = (double)(std::max)(abs(_localCameraPos.z - localBox.getMax().z) / _distFactor,
			(std::max)((std::min)(abs(_localCameraPos.x - localBox.getMin().x), abs(_localCameraPos.x - localBox.getMax().x)),
			(std::min)(abs(_localCameraPos.y - localBox.getMin().y), abs(_localCameraPos.y - localBox.getMax().y))));

		return dist;
	}

	float TerrainNode::getSplitDistance() const
	{
		return _splitDist;
	}

	float TerrainNode::getDistFactor() const
	{
		return _distFactor;
	}

	bool TerrainNode::updateQuad = true;

	void TerrainNode::update(Matrix4d world, Matrix4d projection, Matrix4d view, Vector3d eyePosition)
	{
		Matrix4d localToCamera = world * view; //n->getWorldMatrix() instead of IDENTITY

		_deformedCameraPos = world.inverse() * eyePosition;//world * eyePosition;

		Matrix4d matrix = localToCamera * projection;
		getFrustumPlanes(matrix, _deformedFrustumPlanes);
		_localCameraPos = _deformation->deformedToLocal(_deformedCameraPos);

		Matrix4d m = _deformation->localToDeformedDifferential(_localCameraPos, true);
		_distFactor = (std::max)(Vector3d(m.at(0, 0), m.at(1, 0), m.at(2, 0)).magnitude(), Vector3d(m.at(0, 1), m.at(1, 1), m.at(2, 1)).magnitude());

		Vector3f left = _deformedFrustumPlanes[0].getNormal();
		Vector3f right = _deformedFrustumPlanes[1].getNormal();
		left.normalize();
		right.normalize();

		float fov = (float)acosf(Vector3f::dot(-left, right));
		_splitDist = _splitFactor * 1280.0f / 1024.0f * tan(40.0f / 180.0f * Math::PI) / tan(fov / 2.0f); //viewport width other than 1280
		if (_splitDist < 1.1f /*|| !(isFinite(splitDist))*/) 
		{
			_splitDist = 1.1f;
		}

		if (_horizonCulling && _localCameraPos.z <= _root->getMaxZ()) 
		{
			Vector3d deformedDir = localToCamera.inverse().cast<double>().getForwardVector();
			Vector3d v = _deformation->deformedToLocal(deformedDir) - _localCameraPos;
			Vector2d localDir = Vector2d(v.x, v.y);
			localDir.normalize();
			_localCameraDir = Vector4d(localDir.y, -localDir.x, -localDir.x, -localDir.y);
			for (int32 i = 0; i < HORIZON_SIZE; ++i) 
			{
				_horizon[i] = -INFINITY;
			}
		}

		if (updateQuad)
		{
			_root->update();
		}
	}

	TerrainQuad::Visibility TerrainNode::getVisibility(const AxisAlignedBoundingBoxd &localBox) const
	{
		return _deformation->getVisibility(this, localBox);
	}

	void TerrainNode::getFrustumPlanes(const Matrix4d& m, Planef* frustumPlanes)
	{
		//left
		frustumPlanes[0].setNormal(Vector3f(
			m.at(0, 3) + m.at(0, 0),
			m.at(1, 3) + m.at(1, 0),
			m.at(2, 3) + m.at(2, 0)));
		frustumPlanes[0].setDistance(m.at(3, 3) + m.at(3, 0));
		//right
		frustumPlanes[1].setNormal(Vector3f(
			m.at(0, 3) - m.at(0, 0),
			m.at(1, 3) - m.at(1, 0),
			m.at(2, 3) - m.at(2, 0)));
		frustumPlanes[1].setDistance(m.at(3, 3) - m.at(3, 0));
		//bottom
		frustumPlanes[2].setNormal(Vector3f(
			m.at(0, 3) + m.at(0, 1),
			m.at(1, 3) + m.at(1, 1),
			m.at(2, 3) + m.at(2, 1)));
		frustumPlanes[2].setDistance(m.at(3, 3) + m.at(3, 1));
		//top
		frustumPlanes[3].setNormal(Vector3f(
			m.at(0, 3) - m.at(0, 1),
			m.at(1, 3) - m.at(1, 1),
			m.at(2, 3) - m.at(2, 1)));
		frustumPlanes[3].setDistance(m.at(3, 3) - m.at(3, 1));
		//near
		frustumPlanes[4].setNormal(Vector3f(
			m.at(0, 2),
			m.at(1, 2),
			m.at(2, 2)));
		frustumPlanes[4].setDistance(m.at(3, 2));
		//far
		frustumPlanes[5].setNormal(Vector3f(
			m.at(0, 3) - m.at(0, 2),
			m.at(1, 3) - m.at(1, 2),
			m.at(2, 3) - m.at(2, 2)));
		frustumPlanes[5].setDistance(m.at(3, 3) - m.at(3, 2));
	}

	TerrainQuad::Visibility TerrainNode::getVisibility(const Planef* frustumPlanes, const AxisAlignedBoundingBoxd& localBox)
	{
		TerrainQuad::Visibility v0 = getVisibility(frustumPlanes[0], localBox);
		if (v0 == TerrainQuad::Invisible)
		{
			return TerrainQuad::Invisible;
		}
		TerrainQuad::Visibility v1 = getVisibility(frustumPlanes[1], localBox);
		if (v1 == TerrainQuad::Invisible)
		{
			return TerrainQuad::Invisible;
		}
		TerrainQuad::Visibility v2 = getVisibility(frustumPlanes[2], localBox);
		if (v2 == TerrainQuad::Invisible)
		{
			return TerrainQuad::Invisible;
		}
		TerrainQuad::Visibility v3 = getVisibility(frustumPlanes[3], localBox);
		if (v3 == TerrainQuad::Invisible)
		{
			return TerrainQuad::Invisible;
		}
		TerrainQuad::Visibility v4 = getVisibility(frustumPlanes[4], localBox);
		if (v4 == TerrainQuad::Invisible)
		{
			return TerrainQuad::Invisible;
		}

		if (v0 == TerrainQuad::FullyVisible && v1 == TerrainQuad::FullyVisible &&
			v2 == TerrainQuad::FullyVisible && v3 == TerrainQuad::FullyVisible &&
			v4 == TerrainQuad::FullyVisible)
		{
			return TerrainQuad::FullyVisible;
		}

		return TerrainQuad::PartiallyVisible;
	}

	TerrainQuad::Visibility TerrainNode::getVisibility(const Planef& clip, const AxisAlignedBoundingBoxd& b)
	{
		double x0 = b.getMin().x * clip.getNormal().x;
		double x1 = b.getMax().x * clip.getNormal().x;
		double y0 = b.getMin().y * clip.getNormal().y;
		double y1 = b.getMax().y * clip.getNormal().y;
		double z0 = b.getMin().z * clip.getNormal().z + clip.getDistance();
		double z1 = b.getMax().z * clip.getNormal().z + clip.getDistance();
		double p1 = x0 + y0 + z0;
		double p2 = x1 + y0 + z0;
		double p3 = x1 + y1 + z0;
		double p4 = x0 + y1 + z0;
		double p5 = x0 + y0 + z1;
		double p6 = x1 + y0 + z1;
		double p7 = x1 + y1 + z1;
		double p8 = x0 + y1 + z1;
		if (p1 <= 0 && p2 <= 0 && p3 <= 0 && p4 <= 0 && p5 <= 0 && p6 <= 0 && p7 <= 0 && p8 <= 0)
		{
			return TerrainQuad::Invisible;
		}
		if (p1 > 0 && p2 > 0 && p3 > 0 && p4 > 0 && p5 > 0 && p6 > 0 && p7 > 0 && p8 > 0)
		{
			return TerrainQuad::FullyVisible;
		}

		return TerrainQuad::PartiallyVisible;
	}

	Vector2d mult(Vector4d vec4, Vector2d vec2)
	{
		Vector2d kProd;

		kProd.x = vec4.x * vec2.x +
			vec4.y * vec2.y;

		kProd.y = vec4.z * vec2.x +
			vec4.w * vec2.y;

		return kProd;
	}

	bool TerrainNode::addOccluder(const AxisAlignedBoundingBoxd& occluder)
	{
		if (!_horizonCulling || _localCameraPos.z > _root->getMaxZ()) 
		{
			return false;
		}
		Vector2d corners[4];
		Vector2d o = Vector2d(_localCameraPos.x, _localCameraPos.y);
		corners[0] = mult(_localCameraDir, (Vector2d(occluder.getMin().x, occluder.getMin().y) - o));
		corners[1] = mult(_localCameraDir, (Vector2d(occluder.getMin().x, occluder.getMax().x) - o));
		corners[2] = mult(_localCameraDir, (Vector2d(occluder.getMax().x, occluder.getMin().y) - o));
		corners[3] = mult(_localCameraDir, (Vector2d(occluder.getMax().x, occluder.getMax().y) - o));
		if (corners[0].y <= 0.0 || corners[1].y <= 0.0 || corners[2].y <= 0.0 || corners[3].y <= 0.0)
		{
			return false;
		}
		double dzmin = double(occluder.getMin().z - _localCameraPos.z);
		double dzmax = double(occluder.getMax().z - _localCameraPos.z);
		Vector3f bounds[4];
		bounds[0] = Vector3f(corners[0].x, dzmin, dzmax) / corners[0].y;
		bounds[1] = Vector3f(corners[1].x, dzmin, dzmax) / corners[1].y;
		bounds[2] = Vector3f(corners[2].x, dzmin, dzmax) / corners[2].y;
		bounds[3] = Vector3f(corners[3].x, dzmin, dzmax) / corners[3].y;
		double xmin = (std::min)((std::min)(bounds[0].x, bounds[1].x), (std::min)(bounds[2].x, bounds[3].x)) * 0.33 + 0.5;
		double xmax = (std::max)((std::max)(bounds[0].x, bounds[1].x), (std::max)(bounds[2].x, bounds[3].x)) * 0.33 + 0.5;
		double zmin = (std::min)((std::min)(bounds[0].y, bounds[1].y), (std::min)(bounds[2].y, bounds[3].y));
		double zmax = (std::max)((std::max)(bounds[0].z, bounds[1].z), (std::max)(bounds[2].z, bounds[3].z));

		int imin = (std::max)(int(floor(xmin * HORIZON_SIZE)), 0);
		int imax = (std::min)(int(ceil(xmax * HORIZON_SIZE)), HORIZON_SIZE - 1);

		bool occluded = imax >= imin;
		for (int i = imin; i <= imax; ++i) 
		{
			if (zmax > _horizon[i])
			{
				occluded = false;
				break;
			}
		}
		if (!occluded) 
		{
			imin = (std::max)(int(ceil(xmin * HORIZON_SIZE)), 0);
			imax = (std::min)(int(floor(xmax * HORIZON_SIZE)), HORIZON_SIZE - 1);
			for (int i = imin; i <= imax; ++i)
			{
				_horizon[i] = (std::max)(_horizon[i], zmin);
			}
		}
		return occluded;
	}

	bool TerrainNode::isOccluded(const AxisAlignedBoundingBoxd& box)
	{
		if (!_horizonCulling || _localCameraPos.z > _root->getMaxZ())
		{
			return false;
		}
		Vector2d corners[4];
		Vector2d o = Vector2d(_localCameraPos.x, _localCameraPos.y);
		corners[0] = mult(_localCameraDir, (Vector2d(box.getMin().x, box.getMin().y) - o));
		corners[1] = mult(_localCameraDir, (Vector2d(box.getMin().x, box.getMax().y) - o));
		corners[2] = mult(_localCameraDir, (Vector2d(box.getMax().x, box.getMin().y) - o));
		corners[3] = mult(_localCameraDir, (Vector2d(box.getMax().x, box.getMax().y) - o));
		if (corners[0].y <= 0.0 || corners[1].y <= 0.0 || corners[2].y <= 0.0 || corners[3].y <= 0.0)
		{
			return false;
		}
		double dz = double(box.getMax().z - _localCameraPos.z);
		corners[0] = Vector2d(corners[0].x, dz) / corners[0].y;
		corners[1] = Vector2d(corners[1].x, dz) / corners[1].y;
		corners[2] = Vector2d(corners[2].x, dz) / corners[2].y;
		corners[3] = Vector2d(corners[3].x, dz) / corners[3].y;
		double xmin = (std::min)((std::min)(corners[0].x, corners[1].x), (std::min)(corners[2].x, corners[3].x)) * 0.33 + 0.5;
		double xmax = (std::max)((std::max)(corners[0].x, corners[1].x), (std::max)(corners[2].x, corners[3].x)) * 0.33 + 0.5;
		double zmax = (std::max)((std::max)(corners[0].y, corners[1].y), (std::max)(corners[2].y, corners[3].y));
		int imin = (std::max)(int(floor(xmin * HORIZON_SIZE)), 0);
		int imax = (std::min)(int(ceil(xmax * HORIZON_SIZE)), HORIZON_SIZE - 1);
		for (int i = imin; i <= imax; ++i) 
		{
			if (zmax > _horizon[i]) 
			{
				return false;
			}
		}
		return imax >= imin;
	}
}
