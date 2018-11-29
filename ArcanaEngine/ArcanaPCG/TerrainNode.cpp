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
		_horizon = new float[HORIZON_SIZE];
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

	//SceneManager::Visibility getVisibility(const BoundingBoxf &localBox) const;

	float TerrainNode::getSplitDistance() const
	{
		return _splitDist;
	}

	float TerrainNode::getDistFactor() const
	{
		return _distFactor;
	}

	void TerrainNode::update(Matrix4f projection, Matrix4f view, Vector3d eyePosition)
	{
		Matrix4f localToCamera = Matrix4f::IDENTITY * view; //n->getWorldMatrix() instead of IDENTITY

		_deformedCameraPos = eyePosition;

		//TerrainNode::getFrustumPlanes(localToCamera * owner->getProjectionMatrix(), deformedFrustumPlanes);
		_localCameraPos = _deformation->deformedToLocal(_deformedCameraPos);

		Matrix4d m = _deformation->localToDeformedDifferential(_localCameraPos, true);
		_distFactor = (std::max)(Vector3d(m.at(0, 0), m.at(1, 0), m.at(2, 0)).magnitude(), Vector3d(m.at(0, 1), m.at(1, 1), m.at(2, 1)).magnitude());

		float fov = 60.0f;// (float)acosf(Vector3f::dot(-left, right));
		_splitDist = _splitFactor * 1280.0f / 1024.0f * tan(40.0f / 180.0f * Math::PI) / tan(fov / 2.0f); //viewport width other than 1280
		if (_splitDist < 1.1f /*|| !(isFinite(splitDist))*/) 
		{
			_splitDist = 1.1f;
		}

		if (_horizonCulling && _localCameraPos.z <= _root->getMaxZ()) 
		{
			Vector3d deformedDir = localToCamera.inverse().cast<double>() * Vector3d::unitZ();
			Vector3d v = _deformation->deformedToLocal(deformedDir) - _localCameraPos;
			Vector2f localDir = Vector2f(v.x, v.y);
			localDir.normalize();
			_localCameraDir = Vector4f(localDir.y, -localDir.x, -localDir.x, -localDir.y);
			for (int i = 0; i < HORIZON_SIZE; ++i) {
				_horizon[i] = -INFINITY;
			}
		}

		_root->update();
	}
}
