#ifndef DEFORMATION_H_
#define DEFORMATION_H_

#include "TerrainDefines.h"

#include "Object.h"
#include "Vector3.h"
#include "Matrix4.h"
#include "TerrainQuadRenderData.h"
#include "TerrainQuad.h"

namespace Arcana
{
	class ARCANA_TERRAIN_API TerrainNode;

	class ARCANA_TERRAIN_API Deformation : public Object
	{
	public:

		Deformation();

		virtual ~Deformation();

		virtual Vector3d localToDeformed(const Vector3d &localPt) const;

		virtual Matrix4d localToDeformedDifferential(const Vector3d &localPt, bool clamp = false) const;

		virtual Vector3d deformedToLocal(const Vector3d &deformedPt) const;

		virtual Vector4f deformedToLocalBounds(const Vector3d &deformedCenter, double deformedRadius) const;

		virtual Matrix4d deformedToTangentFrame(const Vector3d &deformedPt) const;

		virtual void setUniforms(Matrix4d world, Matrix4d projection, Matrix4d view, Vector3d eyePosition, TerrainNode* n, Shader* shader) const;

		virtual void setUniforms(TerrainQuad* q, Shader* shader) const;

		virtual double getLocalDist(const Vector3d& localPt, const AxisAlignedBoundingBoxd& localBox) const;

		virtual TerrainQuad::Visibility getVisibility(const TerrainNode* t, const AxisAlignedBoundingBoxd &localBox) const;

		virtual void setScreenUniforms(TerrainQuad* q, Shader* shader) const;

		//instancing

		virtual void setUniforms(Matrix4d world, Matrix4d projection, Matrix4d view, Vector3d eyePosition, TerrainNode* n, std::vector<Vector4f>& data, float r) const;

		virtual void setUniforms(TerrainQuad* q, Array<Vector4f>& data) const;

		virtual void setScreenUniforms(TerrainQuad* q, Array<Vector4f>& data) const;

	protected:

		mutable Matrix4d _cameraToScreen;

		mutable Matrix4d _localToScreen;

		mutable Matrix3d _localToTangent;
	};

}

#endif // !DEFORMATION_H_

