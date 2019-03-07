#include "SphericalDeformation.h"
#include <algorithm>

namespace Arcana
{

	SphericalDeformation::SphericalDeformation(double R) :
		Deformation(), R(R)
	{
	}

	SphericalDeformation::~SphericalDeformation()
	{
	}

	Vector3d SphericalDeformation::localToDeformed(const Vector3d &localPt) const
	{
		return Vector3d::normalize(Vector3d(localPt.x, localPt.y, R)) * (localPt.z + R);
	}

	Matrix4d SphericalDeformation::localToDeformedDifferential(const Vector3d &localPt, bool clamp) const
	{
		//if (!isFinite(localPt.x) || !isFinite(localPt.y) || !isFinite(localPt.z)) {
		//	return mat4d::IDENTITY;
		//}

		Vector3d pt = localPt;
		if (clamp) {
			pt.x = pt.x - floor((pt.x + R) / (2.0 * R)) * 2.0 * R;
			pt.y = pt.y - floor((pt.y + R) / (2.0 * R)) * 2.0 * R;
		}

		double l = pt.x*pt.x + pt.y*pt.y + R * R;
		double c0 = 1.0 / sqrt(l);
		double c1 = c0 * R / l;
		return Matrix4d((pt.y*pt.y + R * R)*c1, -pt.x*pt.y*c1, pt.x*c0, R*pt.x*c0,
			-pt.x*pt.y*c1, (pt.x*pt.x + R * R)*c1, pt.y*c0, R*pt.y*c0,
			-pt.x*R*c1, -pt.y*R*c1, R*c0, (R*R)*c0,
			0.0, 0.0, 0.0, 1.0);
	}

	Vector3d SphericalDeformation::deformedToLocal(const Vector3d &deformedPt) const
	{
		double l = deformedPt.magnitude();
		if (deformedPt.z >= abs(deformedPt.x) && deformedPt.z >= abs(deformedPt.y)) 
		{
			return Vector3d(deformedPt.x / deformedPt.z * R, deformedPt.y / deformedPt.z * R, l - R);
		}
		if (deformedPt.z <= -abs(deformedPt.x) && deformedPt.z <= -abs(deformedPt.y))
		{
			return Vector3d(INFINITY, INFINITY, INFINITY);
		}
		if (deformedPt.y >= abs(deformedPt.x) && deformedPt.y >= abs(deformedPt.z))
		{
			return Vector3d(deformedPt.x / deformedPt.y * R, (2.0 - deformedPt.z / deformedPt.y) * R, l - R);
		}
		if (deformedPt.y <= -abs(deformedPt.x) && deformedPt.y <= -abs(deformedPt.z))
		{
			return Vector3d(-deformedPt.x / deformedPt.y * R, (-2.0 - deformedPt.z / deformedPt.y) * R, l - R);
		}
		if (deformedPt.x >= abs(deformedPt.y) && deformedPt.x >= abs(deformedPt.z)) 
		{
			return Vector3d((2.0 - deformedPt.z / deformedPt.x) * R, deformedPt.y / deformedPt.x * R, l - R);
		}
		if (deformedPt.x <= -abs(deformedPt.y) && deformedPt.x <= -abs(deformedPt.z))
		{
			return Vector3d((-2.0 - deformedPt.z / deformedPt.x) * R, -deformedPt.y / deformedPt.x * R, l - R);
		}
		assert(false);
		return Vector3d::zero();
	}

	Vector4f SphericalDeformation::deformedToLocalBounds(const Vector3d &deformedCenter, double deformedRadius) const
	{
		Vector3d p = deformedToLocal(deformedCenter);
		double r = deformedRadius;

		//if (isInf(p.x) || isInf(p.y)) {
		//	return Vector4f();
		//}

		double k = (1.0 - r * r / (2.0 * R * R)) * Vector3f(p.x, p.y, R).magnitude();
		double A = k * k - p.x * p.x;
		double B = k * k - p.y * p.y;
		double C = -2.0 * p.x * p.y;
		double D = -2.0 * R * R * p.x;
		double E = -2.0 * R * R * p.y;
		double F = R * R * (k * k - R * R);

		double a = C * C - 4.0 * A * B;
		double b = 2.0 * C * E - 4.0 * B * D;
		double c = E * E - 4.0 * B * F;
		double d = sqrt(b * b - 4.0 * a * c);
		double x1 = (-b - d) / (2.0 * a);
		double x2 = (-b + d) / (2.0 * a);

		b = 2.0 * C * D - 4.0 * A * E;
		c = D * D - 4.0 * A * F;
		d = sqrt(b * b - 4.0 * a * c);
		double y1 = (-b - d) / (2.0 * a);
		double y2 = (-b + d) / (2.0 * a);

		return Vector4f(x1, y1, x2, y2);
	}

	Matrix4d SphericalDeformation::deformedToTangentFrame(const Vector3d &deformedPt) const
	{
		Vector3d Uz = deformedPt;
		Uz.normalize();
		Vector3d Ux = Vector3d::cross(Vector3d::unitY(), Uz);
		Ux.normalize();
		Vector3d Uy = Vector3d::cross(Uz, Ux);
		return Matrix4d(Ux.x, Ux.y, Ux.z, 0.0,
			Uy.x, Uy.y, Uy.z, 0.0,
			Uz.x, Uz.y, Uz.z, -R,
			0.0, 0.0, 0.0, 1.0);
	}

	void SphericalDeformation::setUniforms(Matrix4d world, Matrix4d projection, Matrix4d view, Vector3d eyePosition, TerrainNode* n, Shader* shader) const
	{
		Deformation::setUniforms(world, projection, view, eyePosition, n, shader);

		_world = world;

		shader->getUniform("deformation.radius").setValue((float)R);
	}

	void SphericalDeformation::setUniforms(TerrainQuad* q, Shader* shader) const
	{
		Deformation::setUniforms(q, shader);
	}

	TerrainQuad::Visibility SphericalDeformation::getVisibility(const TerrainNode* t, const AxisAlignedBoundingBoxd& localBox) const
	{
		Vector3d deformedBox[4];
		deformedBox[0] = localToDeformed(Vector3d(localBox.getMin().x, localBox.getMin().y, localBox.getMin().z));
		deformedBox[1] = localToDeformed(Vector3d(localBox.getMax().x, localBox.getMin().y, localBox.getMin().z));
		deformedBox[2] = localToDeformed(Vector3d(localBox.getMax().x, localBox.getMax().y, localBox.getMin().z));
		deformedBox[3] = localToDeformed(Vector3d(localBox.getMin().x, localBox.getMax().y, localBox.getMin().z));
		double a = (localBox.getMax().z + R) / (localBox.getMin().z + R);
		double dx = (localBox.getMax().x - localBox.getMin().x) / 2 * a;
		double dy = (localBox.getMax().y - localBox.getMin().y) / 2 * a;
		double dz = localBox.getMax().z + R;
		double f = sqrt(dx * dx + dy * dy + dz * dz) / (localBox.getMin().z + R);

		const Planef* deformedFrustumPlanes = t->getDeformedFrustumPlanes();
		TerrainQuad::Visibility v0 = getVisibility(deformedFrustumPlanes[0], deformedBox, f);
		if (v0 == TerrainQuad::Invisible) 
		{
			return TerrainQuad::Invisible;
		}
		TerrainQuad::Visibility v1 = getVisibility(deformedFrustumPlanes[1], deformedBox, f);
		if (v1 == TerrainQuad::Invisible)
		{
			return TerrainQuad::Invisible;
		}
		TerrainQuad::Visibility v2 = getVisibility(deformedFrustumPlanes[2], deformedBox, f);
		if (v2 == TerrainQuad::Invisible)
		{
			return TerrainQuad::Invisible;
		}
		TerrainQuad::Visibility v3 = getVisibility(deformedFrustumPlanes[3], deformedBox, f);
		if (v3 == TerrainQuad::Invisible)
		{
			return TerrainQuad::Invisible;
		}
		TerrainQuad::Visibility v4 = getVisibility(deformedFrustumPlanes[4], deformedBox, f);
		if (v4 == TerrainQuad::Invisible)
		{
			return TerrainQuad::Invisible;
		}

		Vector3d c = t->getDeformedCamera();
		double lSq = c.magnitudeSq();
		double rm = R + (std::min)(0.0, localBox.getMin().z);
		double rM = R + localBox.getMax().z;
		double rmSq = rm * rm;
		double rMSq = rM * rM;
		Planef farPlane = Planef(sqrt((lSq - rmSq) * (rMSq - rmSq)) - rmSq, c.cast<float>());

		TerrainQuad::Visibility v5 = getVisibility(farPlane, deformedBox, f);
		if (v5 == TerrainQuad::Invisible) 
		{
			return TerrainQuad::Invisible;
		}

		if (v0 == TerrainQuad::FullyVisible && v1 == TerrainQuad::FullyVisible &&
			v2 == TerrainQuad::FullyVisible && v3 == TerrainQuad::FullyVisible &&
			v4 == TerrainQuad::FullyVisible && v5 == TerrainQuad::FullyVisible)
		{
			return TerrainQuad::FullyVisible;
		}
		return TerrainQuad::PartiallyVisible;
	}

	TerrainQuad::Visibility SphericalDeformation::getVisibility(const Planef& clip, const Vector3d* b, double f)
	{
		double o = b[0].x * clip.getNormal().x + b[0].y * clip.getNormal().y + b[0].z * clip.getNormal().z;
		bool p = o + clip.getDistance() > 0.0;
		if ((o * f + clip.getDistance() > 0.0) == p)
		{
			o = b[1].x * clip.getNormal().x + b[1].y * clip.getNormal().y + b[1].z * clip.getNormal().z;
			if ((o + clip.getDistance() > 0.0) == p && (o * f + clip.getDistance() > 0.0) == p)
			{
				o = b[2].x * clip.getNormal().x + b[2].y * clip.getNormal().y + b[2].z * clip.getNormal().z;
				if ((o + clip.getDistance() > 0.0) == p && (o * f + clip.getDistance() > 0.0) == p)
				{
					o = b[3].x * clip.getNormal().x + b[3].y * clip.getNormal().y + b[3].z * clip.getNormal().z;
					return (o + clip.getDistance() > 0.0) == p && (o * f + clip.getDistance() > 0.0) == p 
						? (p ? TerrainQuad::FullyVisible : TerrainQuad::Invisible) : TerrainQuad::PartiallyVisible;
				}
			}
		}
		return TerrainQuad::PartiallyVisible;
	}
}
