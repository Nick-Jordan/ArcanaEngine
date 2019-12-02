#ifndef SPHERICALDEFORMATION_H_
#define SPHERICALDEFORMATION_H_

#include "TerrainDefines.h"

#include "TerrainDefines.h"

#include "Deformation.h"
#include "TerrainNode.h"

namespace Arcana
{

	class ARCANA_TERRAIN_API SphericalDeformation : public Deformation
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

		virtual void setUniforms(Matrix4d world, Matrix4d projection, Matrix4d view, Vector3d eyePosition, TerrainNode* n, Shader* shader) const override;

		virtual void setUniforms(TerrainQuad* q, Array<Vector4f>& data) const override;

		//virtual double getLocalDist(const Vector3d& localPt, const AxisAlignedBoundingBoxd& localBox) const;

		virtual void setScreenUniforms(TerrainQuad* q, Array<Vector4f>& data) const override;

		virtual TerrainQuad::Visibility getVisibility(const TerrainNode* t, const AxisAlignedBoundingBoxd& localBox) const; //Visibility enum

	private:

		static TerrainQuad::Visibility getVisibility(const Planef& clip, const Vector3d* b, double f);
	};

}

#endif