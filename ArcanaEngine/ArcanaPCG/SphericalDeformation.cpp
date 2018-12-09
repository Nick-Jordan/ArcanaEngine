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
		if (deformedPt.z >= abs(deformedPt.x) && deformedPt.z >= abs(deformedPt.y)) {
			return Vector3d(deformedPt.x / deformedPt.z * R, deformedPt.y / deformedPt.z * R, l - R);
		}
		if (deformedPt.z <= -abs(deformedPt.x) && deformedPt.z <= -abs(deformedPt.y)) {
			return Vector3d(INFINITY, INFINITY, INFINITY);
		}
		if (deformedPt.y >= abs(deformedPt.x) && deformedPt.y >= abs(deformedPt.z)) {
			return Vector3d(deformedPt.x / deformedPt.y * R, (2.0 - deformedPt.z / deformedPt.y) * R, l - R);
		}
		if (deformedPt.y <= -abs(deformedPt.x) && deformedPt.y <= -abs(deformedPt.z)) {
			return Vector3d(-deformedPt.x / deformedPt.y * R, (-2.0 - deformedPt.z / deformedPt.y) * R, l - R);
		}
		if (deformedPt.x >= abs(deformedPt.y) && deformedPt.x >= abs(deformedPt.z)) {
			return Vector3d((2.0 - deformedPt.z / deformedPt.x) * R, deformedPt.y / deformedPt.x * R, l - R);
		}
		if (deformedPt.x <= -abs(deformedPt.y) && deformedPt.x <= -abs(deformedPt.z)) {
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

	void SphericalDeformation::setUniforms(Matrix4f projection, Matrix4f view, Vector3d eyePosition, TerrainNode* n, Material* material) const
	{
		Deformation::setUniforms(projection, view, eyePosition, n, material);

		Matrix4f ltow = Matrix4f::IDENTITY;// = context->getWorldMatrix();

		Shader* shader = material->getCurrentTechnique()->getPass(0);

		shader->getUniform("deformation.localToWorld")->setValue(Matrix3f(
			ltow.at(0, 0), ltow.at(0, 1), ltow.at(0, 2),
			ltow.at(1, 0), ltow.at(1, 1), ltow.at(1, 2),
			ltow.at(2, 0), ltow.at(2, 1), ltow.at(2, 2)));


		shader->getUniform("deformation.radius")->setValue(R);

	}

	void SphericalDeformation::setUniforms(TerrainQuad* q, Material* material) const
	{
		Deformation::setUniforms(q, material);
	}

	void SphericalDeformation::setScreenUniforms(TerrainQuad* q, Material* material) const
	{
		Vector3f p0 = Vector3f(q->getPhysicalXCoordinate(), q->getPhysicalYCoordinate(), R);
		Vector3f p1 = Vector3f(q->getPhysicalXCoordinate() + q->getPhysicalLevel(), q->getPhysicalYCoordinate(), R);
		Vector3f p2 = Vector3f(q->getPhysicalXCoordinate(), q->getPhysicalYCoordinate() + q->getPhysicalLevel(), R);
		Vector3f p3 = Vector3f(q->getPhysicalXCoordinate() + q->getPhysicalLevel(), q->getPhysicalYCoordinate() + q->getPhysicalLevel(), R);
		Vector3f pc = (p0 + p3) * 0.5;
		double l0 = p0.magnitude(),
			l1 = p1.magnitude(),
			l2 = p2.magnitude(),
			l3 = p3.magnitude();
		Vector3f v0 = Vector3f::normalize(p0);
		Vector3f v1 = Vector3f::normalize(p1);
		Vector3f v2 = Vector3f::normalize(p2);
		Vector3f v3 = Vector3f::normalize(p3);

		Matrix4f deformedCorners = Matrix4f(
			v0.x * R, v1.x * R, v2.x * R, v3.x * R,
			v0.y * R, v1.y * R, v2.y * R, v3.y * R,
			v0.z * R, v1.z * R, v2.z * R, v3.z * R,
			1.0, 1.0, 1.0, 1.0).transpose();

		/*std::cout << ((localToScreen[0][0] * deformedCorners[0][0])
			+ (localToScreen[0][1] * deformedCorners[1][0])
			+ (localToScreen[0][2] * deformedCorners[2][0])
			+ (localToScreen[0][3] * deformedCorners[3][0])) << std::endl;

		std::cout << "Yee: " << (localToScreen * deformedCorners)[0][0] << std::endl;*/

		Shader* shader = material->getCurrentTechnique()->getPass(0);

		shader->getUniform("deformation.screenQuadCorners")->setValue(deformedCorners * _localToScreen);

		Matrix4f deformedVerticals = Matrix4f(
			v0.x, v1.x, v2.x, v3.x,
			v0.y, v1.y, v2.y, v3.y,
			v0.z, v1.z, v2.z, v3.z,
			0.0, 0.0, 0.0, 0.0).transpose();

		shader->getUniform("deformation.screenQuadVerticals")->setValue(deformedVerticals * _localToScreen);

		shader->getUniform("deformation.screenQuadCornerNorms")->setValue(Vector4f(l0, l1, l2, l3));

		Vector3f uz = Vector3f::normalize(pc);
		Vector3f ux = Vector3f::cross(Vector3f::unitY(), uz);
		ux.normalize();
		Vector3f uy = Vector3f::cross(uz, ux);

		Matrix4f ltow;// = context->getWorldMatrix();
		Matrix3f tangentFrameToWorld = Matrix3f(
			ux.x, uy.x, uz.x,
			ux.y, uy.y, uz.y,
			ux.z, uy.z, uz.z).transpose()
			* Matrix3f(
				ltow.at(0, 0), ltow.at(0, 1), ltow.at(0, 2),
				ltow.at(1, 0), ltow.at(1, 1), ltow.at(1, 2),
				ltow.at(2, 0), ltow.at(2, 1), ltow.at(2, 2));

		shader->getUniform("deformation.tangentFrameToWorld")->setValue(tangentFrameToWorld);

		shader->getUniform("worldSunDir")->setValue(Vector3f(0, -1, 0));

		shader->getUniform("hdrExposure")->setValue(0.4f);

	}

	/*SceneManager::Visibility SphericalDeformation::getVisibility(const TerrainNode *t, const BoundingBoxf &localBox) const
	{
		Vector3d deformedBox[4];
		deformedBox[0] = localToDeformed(Vector3d(localBox.Min().x, localBox.Min().y, localBox.Min().z));
		deformedBox[1] = localToDeformed(Vector3d(localBox.Max().x, localBox.Min().y, localBox.Min().z));
		deformedBox[2] = localToDeformed(Vector3d(localBox.Max().x, localBox.Max().y, localBox.Min().z));
		deformedBox[3] = localToDeformed(Vector3d(localBox.Min().x, localBox.Max().y, localBox.Min().z));
		double a = (localBox.Max().z + R) / (localBox.Min().z + R);
		double dx = (localBox.Max().x - localBox.Min().x) / 2 * a;
		double dy = (localBox.Max().y - localBox.Min().y) / 2 * a;
		double dz = localBox.Max().z + R;
		double f = sqrt(dx * dx + dy * dy + dz * dz) / (localBox.Min().z + R);

		const Vector4f *deformedFrustumPlanes = t->getDeformedFrustumPlanes();
		SceneManager::Visibility v0 = getVisibility(deformedFrustumPlanes[0], deformedBox, f);
		if (v0 == SceneManager::INVISIBLE) {
			return SceneManager::INVISIBLE;
		}
		SceneManager::Visibility v1 = getVisibility(deformedFrustumPlanes[1], deformedBox, f);
		if (v1 == SceneManager::INVISIBLE) {
			return SceneManager::INVISIBLE;
		}
		SceneManager::Visibility v2 = getVisibility(deformedFrustumPlanes[2], deformedBox, f);
		if (v2 == SceneManager::INVISIBLE) {
			return SceneManager::INVISIBLE;
		}
		SceneManager::Visibility v3 = getVisibility(deformedFrustumPlanes[3], deformedBox, f);
		if (v3 == SceneManager::INVISIBLE) {
			return SceneManager::INVISIBLE;
		}
		SceneManager::Visibility v4 = getVisibility(deformedFrustumPlanes[4], deformedBox, f);
		if (v4 == SceneManager::INVISIBLE) {
			return SceneManager::INVISIBLE;
		}

		Vector3f c = t->getDeformedCamera();
		double lSq = c.magnitudeSq();
		double rm = R + (std::min)(0.0f, localBox.Min().z);
		double rM = R + localBox.Max().z;
		double rmSq = rm * rm;
		double rMSq = rM * rM;
		Vector4f farPlane = Vector4f(c.x, c.y, c.z, sqrt((lSq - rmSq) * (rMSq - rmSq)) - rmSq);

		SceneManager::Visibility v5 = getVisibility(farPlane, deformedBox, f);
		if (v5 == SceneManager::INVISIBLE) {
			return SceneManager::INVISIBLE;
		}

		if (v0 == SceneManager::FULLY_VISIBLE && v1 == SceneManager::FULLY_VISIBLE &&
			v2 == SceneManager::FULLY_VISIBLE && v3 == SceneManager::FULLY_VISIBLE &&
			v4 == SceneManager::FULLY_VISIBLE && v5 == SceneManager::FULLY_VISIBLE)
		{
			return SceneManager::FULLY_VISIBLE;
		}
		return SceneManager::PARTIALLY_VISIBLE;
	}

	SceneManager::Visibility SphericalDeformation::getVisibility(const Vector4f &clip, const Vector3d *b, double f)
	{
		double o = b[0].x * clip.x + b[0].y * clip.y + b[0].z * clip.z;
		bool p = o + clip.w > 0.0;
		if ((o * f + clip.w > 0.0) == p) {
			o = b[1].x * clip.x + b[1].y * clip.y + b[1].z * clip.z;
			if ((o + clip.w > 0.0) == p && (o * f + clip.w > 0.0) == p) {
				o = b[2].x * clip.x + b[2].y * clip.y + b[2].z * clip.z;
				if ((o + clip.w > 0.0) == p && (o * f + clip.w > 0.0) == p) {
					o = b[3].x * clip.x + b[3].y * clip.y + b[3].z * clip.z;
					return (o + clip.w > 0.0) == p && (o * f + clip.w > 0.0) == p ? (p ? SceneManager::FULLY_VISIBLE : SceneManager::INVISIBLE) : SceneManager::PARTIALLY_VISIBLE;
				}
			}
		}
		return SceneManager::PARTIALLY_VISIBLE;
	}*/

}
