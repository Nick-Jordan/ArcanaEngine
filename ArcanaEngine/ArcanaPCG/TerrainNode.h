#ifndef TERRAIN_NODE_H_
#define TERRAIN_NODE_H_

#include "PCGDefines.h"

#include "Object.h"
#include "TerrainQuad.h"
#include "Deformation.h"

namespace Arcana
{

	class ARCANA_PCG_API TerrainNode : public Object
	{
		friend class TerrainQuad;

	public:

		TerrainNode(TerrainQuad* root, Deformation* deformation, float splitFactor, int32 maxLevel);
		
		virtual ~TerrainNode();


		Deformation* getDeformation() const;

		TerrainQuad* getRootQuad();

		float getSplitFactor() const;

		bool isCullingHorizon() const;

		int32 getMaxLevel() const;

		Vector3d getDeformedCamera() const;

		Vector3d getLocalCamera() const;

		double getCameraDist(const AxisAlignedBoundingBoxd &localBox) const;

		//SceneManager::Visibility getVisibility(const BoundingBoxf &localBox) const;

		float getSplitDistance() const;

		float getDistFactor() const;

		void update(Matrix4f projection, Matrix4f view, Vector3d eyePosition);

	private:

		Deformation* _deformation;

		TerrainQuad* _root;

		float _splitFactor;

		bool _splitInvisibleQuads;

		bool _horizonCulling;

		int32 _maxLevel;


		Vector3d _deformedCameraPos;

		//Vector4d _deformedFrustumPlanes[6];

		Vector3d _localCameraPos;

		float _splitDist;

		float _distFactor;

		Vector4f _localCameraDir;

		float* _horizon;
	};

}

#endif // !TERRAIN_NODE_H_

