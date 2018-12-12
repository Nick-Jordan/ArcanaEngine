#ifndef DEFORMATION_H_
#define DEFORMATION_H_

#include "PCGDefines.h"

#include "Object.h"
#include "Vector3.h"
#include "Matrix4.h"
#include "TerrainQuadRenderData.h"
#include "TerrainQuad.h"

namespace Arcana
{
	class ARCANA_PCG_API TerrainNode;

	class ARCANA_PCG_API Deformation : public Object
	{
	public:

		Deformation();

		virtual ~Deformation();

		virtual Vector3d localToDeformed(const Vector3d &localPt) const;

		virtual Matrix4d localToDeformedDifferential(const Vector3d &localPt, bool clamp = false) const;

		virtual Vector3d deformedToLocal(const Vector3d &deformedPt) const;

		virtual Vector4f deformedToLocalBounds(const Vector3d &deformedCenter, double deformedRadius) const;

		virtual Matrix4d deformedToTangentFrame(const Vector3d &deformedPt) const;

		virtual void setUniforms(Matrix4f projection, Matrix4f view, Vector3d eyePosition, TerrainNode* n, Material* material) const;

		virtual void setUniforms(TerrainQuad* q, Material* material) const;

		virtual double getLocalDist(const Vector3d& localPt, const AxisAlignedBoundingBoxd& localBox) const;

		virtual TerrainQuad::Visibility getVisibility(const TerrainNode* t, const AxisAlignedBoundingBoxd &localBox) const;

		virtual void setScreenUniforms(TerrainQuad* q, Material* material) const;

	protected:

		mutable Matrix4f _cameraToScreen;

		mutable Matrix4f _localToScreen;

		mutable Matrix3f _localToTangent;
	};

}

#endif // !DEFORMATION_H_

