#ifndef SPHERICALDEFORMATION_H_
#define SPHERICALDEFORMATION_H_

#include "PCGDefines.h"

#include "Deformation.h"
#include "TerrainNode.h"

namespace Arcana
{

	class SphericalDeformation : public Deformation
	{
	public:

		const double R;

		SphericalDeformation(double R);

		virtual ~SphericalDeformation();

		virtual Vector3d localToDeformed(const Vector3d &localPt) const override;

		virtual Matrix4d localToDeformedDifferential(const Vector3d &localPt, bool clamp = false) const override;

		virtual Vector3d deformedToLocal(const Vector3d &deformedPt) const override;

		virtual Vector4f deformedToLocalBounds(const Vector3d &deformedCenter, double deformedRadius) const override;

		virtual Matrix4d deformedToTangentFrame(const Vector3d &deformedPt) const override;

		virtual void setUniforms(Matrix4f projection, Matrix4f view, Vector3d eyePosition, TerrainNode* n, Material* material) const override;

		virtual void setUniforms(TerrainQuad* q, Material* material) const override;

		//virtual double getLocalDist(const Vector3d& localPt, const AxisAlignedBoundingBoxd& localBox) const;

		virtual TerrainQuad::Visibility getVisibility(const TerrainNode* t, const AxisAlignedBoundingBoxd& localBox) const; //Visibility enum

		virtual void setScreenUniforms(TerrainQuad* q, Material* material) const override;

	private:

		static TerrainQuad::Visibility getVisibility(const Planef& clip, const Vector3d* b, double f);
	};

}

#endif