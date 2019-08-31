#include "Deformation.h"

#include "TerrainNode.h"
#include "Profiler.h"

namespace Arcana
{
	Deformation::Deformation()
	{
	}

	Deformation::~Deformation()
	{
	}

	Vector3d Deformation::localToDeformed(const Vector3d &localPt) const
	{
		return localPt;
	}

	Matrix4d Deformation::localToDeformedDifferential(const Vector3d &localPt, bool clamp) const
	{
		return Matrix4d::createTranslation(Vector3d(localPt.x, localPt.y, 0.0));
	}

	Vector3d Deformation::deformedToLocal(const Vector3d &deformedPt) const
	{
		return deformedPt;
	}

	Vector4f Deformation::deformedToLocalBounds(const Vector3d &deformedCenter, double deformedRadius) const
	{
		return Vector4f(deformedCenter.x - deformedRadius, deformedCenter.x + deformedRadius,
			deformedCenter.y - deformedRadius, deformedCenter.y + deformedRadius);
	}

	Matrix4d Deformation::deformedToTangentFrame(const Vector3d &deformedPt) const
	{
		return Matrix4d::createTranslation(Vector3d(-deformedPt.x, -deformedPt.y, 0.0));
	}

	void Deformation::setUniforms(Matrix4d world, Matrix4d projection, Matrix4d view, Vector3d eyePosition, TerrainNode* n, Shader* shader) const
	{
		float d1 = n->getSplitDistance() + 1.0f;
		float d2 = 2.0f * n->getSplitDistance();
		shader->getUniform("deformation.blending").setValue(Vector2f(d1, d2 - d1));

		_cameraToScreen = projection;
		_localToWorld = world;
		_viewMatrix = Matrix4d(view.at(0, 0), view.at(0, 1), view.at(0, 2), 0.0,
			view.at(1, 0), view.at(1, 1), view.at(1, 2), 0.0,
			view.at(2, 0), view.at(2, 1), view.at(2, 2), 0.0,
			0.0, 0.0, 0.0, 1.0);
		_cameraPosition = eyePosition;

		_localToScreen = world * view * _cameraToScreen;

		Vector3d localCameraPos = n->getLocalCamera();
		Vector3d deformedCamera = localToDeformed(localCameraPos);
		Matrix4d A = localToDeformedDifferential(localCameraPos);
		Matrix4d B = deformedToTangentFrame(_cameraPosition);
		Matrix4d ltow = world;
		Matrix4d ltot = B * ltow * A;
		_localToTangent = Matrix3d(ltot.at(0, 0), ltot.at(0, 1), ltot.at(0, 3),
			ltot.at(1, 0), ltot.at(1, 1), ltot.at(1, 3),
			ltot.at(3, 0), ltot.at(3, 1), ltot.at(3, 3));
	}

	double Deformation::getLocalDist(const Vector3d& localPt, const AxisAlignedBoundingBoxd& localBox) const
	{
		return (std::max)(abs(localPt.z - localBox.getMax().z),
			(std::max)((std::min)(abs(localPt.x - localBox.getMin().x), abs(localPt.x - localBox.getMax().x)),
			(std::min)(abs(localPt.y - localBox.getMin().y), abs(localPt.y - localBox.getMax().y))));
	}

	void Deformation::setUniforms(TerrainQuad* q, Shader* shader) const
	{
		shader->getUniform("deformation.offset").setValue(Vector4f(q->getPhysicalXCoordinate(), q->getPhysicalYCoordinate(), q->getPhysicalLevel(), q->getLevel()));

		Vector3d camera = q->getOwner()->getLocalCamera();
		float ground = 0.0;
		Vector4f c = Vector4f(float((camera.x - q->getPhysicalXCoordinate()) / q->getPhysicalLevel()),
			float((camera.y - q->getPhysicalYCoordinate()) / q->getPhysicalLevel()),
			float((camera.z - ground) / (q->getPhysicalLevel() * q->getOwner()->getDistFactor())),
			camera.z);

		shader->getUniform("deformation.camera").setValue(c);

		Matrix3d m = _localToTangent * Matrix3d(q->getPhysicalLevel(), 0.0, q->getPhysicalXCoordinate() - camera.x, 0.0, q->getPhysicalLevel(), q->getPhysicalYCoordinate() - camera.y, 0.0, 0.0, 1.0);

		shader->getUniform("deformation.tileToTangent").setValue(m.cast<float>());

		setScreenUniforms(q, shader);
	}

	void Deformation::setScreenUniforms(TerrainQuad* q, Shader* shader) const
	{
		Vector3f p0 = Vector3f(q->getPhysicalXCoordinate(), q->getPhysicalYCoordinate(), 0.0);
		Vector3f p1 = Vector3f(q->getPhysicalXCoordinate() + q->getPhysicalLevel(), q->getPhysicalYCoordinate(), 0.0);
		Vector3f p2 = Vector3f(q->getPhysicalXCoordinate(), q->getPhysicalYCoordinate() + q->getPhysicalLevel(), 0.0);
		Vector3f p3 = Vector3f(q->getPhysicalXCoordinate() + q->getPhysicalLevel(), q->getPhysicalYCoordinate() + q->getPhysicalLevel(), 0.0);
		Matrix4f corners = Matrix4f(
			p0.x, p1.x, p2.x, p3.x,
			p0.y, p1.y, p2.y, p3.y,
			p0.z, p1.z, p2.z, p3.z,
			1.0, 1.0, 1.0, 1.0);

		shader->getUniform("deformation.screenQuadCorners").setValue(_localToScreen.cast<float>() * corners);

		Matrix4f verticals = Matrix4f(
			0.0, 0.0, 0.0, 0.0,
			0.0, 0.0, 0.0, 0.0,
			1.0, 1.0, 1.0, 1.0,
			0.0, 0.0, 0.0, 0.0);

		shader->getUniform("deformation.screenQuadVerticals").setValue(_localToScreen.cast<float>() * verticals);

		shader->getUniform("u_WorldSunDir").setValue(Vector3f(0, -1, 0));
	}

	TerrainQuad::Visibility Deformation::getVisibility(const TerrainNode* t, const AxisAlignedBoundingBoxd &localBox) const
	{
		return TerrainNode::getVisibility(t->getDeformedFrustumPlanes(), localBox);
	}
}