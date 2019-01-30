#ifndef TERRAIN_NODE_H_
#define TERRAIN_NODE_H_

#include "PCGDefines.h"

#include "Object.h"
#include "TerrainQuad.h"
#include "Deformation.h"

//test
#include "Plane.h"

namespace Arcana
{

	class ARCANA_PCG_API TerrainNode : public Object
	{
		friend class TerrainQuad;
		friend class TileSampler;

	public:


		TerrainNode(TerrainQuad* root, Deformation* deformation, float splitFactor, int32 maxLevel);
		
		virtual ~TerrainNode();


		Deformation* getDeformation() const;

		TerrainQuad* getRootQuad();

		float getSplitFactor() const;

		bool isCullingHorizon() const;

		int32 getMaxLevel() const;

		Vector3d getDeformedCamera() const;

		const Planef* getDeformedFrustumPlanes() const;

		Vector3d getLocalCamera() const;

		double getCameraDist(const AxisAlignedBoundingBoxd &localBox) const;

		TerrainQuad::Visibility getVisibility(const AxisAlignedBoundingBoxd &localBox) const;

		float getSplitDistance() const;

		float getDistFactor() const;

		void update(Matrix4d world, Matrix4d projection, Matrix4d view, Vector3d eyePosition);

		bool addOccluder(const AxisAlignedBoundingBoxd& occluder);

		bool isOccluded(const AxisAlignedBoundingBoxd& box);


		static void getFrustumPlanes(const Matrix4d& m, Planef* frustumPlanes);

		static TerrainQuad::Visibility getVisibility(const Planef* frustumPlanes, const AxisAlignedBoundingBoxd& localBox);

		static TerrainQuad::Visibility getVisibility(const Planef& clip, const AxisAlignedBoundingBoxd& b);

		static bool updateQuad;

	private:

		Deformation* _deformation;

		TerrainQuad* _root;

		float _splitFactor;

		bool _splitInvisibleQuads;

		bool _horizonCulling;

		int32 _maxLevel;


		Vector3d _deformedCameraPos;

		Planef _deformedFrustumPlanes[6];

		Vector3d _localCameraPos;

		float _splitDist;

		float _distFactor;

		Vector4d _localCameraDir;

		double* _horizon;
	};

}

#endif // !TERRAIN_NODE_H_

