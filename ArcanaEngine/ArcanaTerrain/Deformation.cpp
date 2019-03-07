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

	//test
	Vector3d cubeToSphere(const Vector3d& v, const double r)
	{
		double x = v.x / r;
		double y = v.y / r;
		double z = v.z / r;

		double dx = x * sqrt(1 - y * y / 2 - z * z / 2 + y * y * z * z / 3);
		double dy = y * sqrt(1 - z * z / 2 - x * x / 2 + z * z * x * x / 3);
		double dz = z * sqrt(1 - x * x / 2 - y * y / 2 + x * x * y * y / 3);

		return v;//Vector3d(dx, dy, dz) * r;
	}

	void Deformation::setUniforms(TerrainQuad* q, Shader* shader) const
	{
		//shader->getUniform("deformation.offset").setValue(Vector4f(q->getPhysicalXCoordinate(), q->getPhysicalYCoordinate(), q->getPhysicalLevel(), q->getLevel()));

		Vector3d camera = q->getOwner()->getLocalCamera();
		float ground = 0.0;
		Vector4f c = Vector4f(float((camera.x - q->getPhysicalXCoordinate()) / q->getPhysicalLevel()),
			float((camera.y - q->getPhysicalYCoordinate()) / q->getPhysicalLevel()),
			float((camera.z - ground) / (q->getPhysicalLevel() * q->getOwner()->getDistFactor())),
			camera.z);

		//shader->getUniform("deformation.camera").setValue(c);

		Vector3d s = Vector3d(q->getPhysicalLevel(), q->getPhysicalLevel(), 1.0);
		Matrix4d scale = Matrix4d::createScale(s);
		Vector3d t = Vector3d(q->getPhysicalXCoordinate(), q->getPhysicalYCoordinate(), 0.0);
		Matrix4d grid = scale * Matrix4d::createTranslation(t);
		Matrix4d model = scale * Matrix4d::createTranslation(t - _cameraPosition) * _localToWorld;
		Matrix4d modelViewProjection = model * _viewMatrix * _cameraToScreen;

		shader->getUniform("gridMatrix").setValue(grid.cast<float>());
		shader->getUniform("quadMVP").setValue(modelViewProjection.cast<float>());
	}

	TerrainQuad::Visibility Deformation::getVisibility(const TerrainNode* t, const AxisAlignedBoundingBoxd &localBox) const
	{
		return TerrainNode::getVisibility(t->getDeformedFrustumPlanes(), localBox);
	}
}