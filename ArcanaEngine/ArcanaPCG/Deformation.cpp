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

	void Deformation::setUniforms(Matrix4f projection, Matrix4f view, Vector3d eyePosition, TerrainNode* n, Material* material) const
	{
		float d1 = n->getSplitDistance() + 1.0f;
		float d2 = 2.0f * n->getSplitDistance();
		material->getCurrentTechnique()->getPass(0)->getUniform("deformation.blending")->setValue(Vector2f(d1, d2 - d1));

		_cameraToScreen = projection;
		_localToScreen = Matrix4f::IDENTITY * view * _cameraToScreen; //context->getWorldMatrix() instead of IDENTITY

		Vector3d localCameraPos = n->getLocalCamera();
		Vector3d worldCamera = eyePosition;
		Vector3d deformedCamera = localToDeformed(localCameraPos);
		Matrix4d A = localToDeformedDifferential(localCameraPos);
		Matrix4d B = deformedToTangentFrame(worldCamera);
		Matrix4d ltow = Matrix4d::IDENTITY;//context->getWorldMatrix() instead of IDENTITY
		Matrix4d ltot = B * ltow * A;
		_localToTangent = Matrix3f(ltot.at(0, 0), ltot.at(0, 1), ltot.at(0, 3),
			ltot.at(1, 0), ltot.at(1, 1), ltot.at(1, 3),
			ltot.at(3, 0), ltot.at(3, 1), ltot.at(3, 3));
	}

	void Deformation::setUniforms(TerrainQuad* q, Material* material) const
	{
		//MeshRenderContext::UniformParameter deformationOffset;
		//deformationOffset.name = "deformation.offset";
		//deformationOffset.value.type = Uniform::Value::Vec4f;
		//deformationOffset.value.vec4 = Vector4f(q->getPhysicalXCoordinate(), q->getPhysicalYCoordinate(), q->getPhysicalLevel(), q->getLevel());

		material->getCurrentTechnique()->getPass(0)->getUniform("deformation.offset")->setValue(Vector4f(q->getPhysicalXCoordinate(), q->getPhysicalYCoordinate(), q->getPhysicalLevel(), q->getLevel()));

		Vector3d camera = q->getOwner()->getLocalCamera();

		float ground = 0.0;

		//MeshRenderContext::UniformParameter deformationCamera;
		//deformationCamera.name = "deformation.camera";
		//deformationCamera.value.type = Uniform::Value::Vec4f;
		//deformationCamera.value.vec4 = Vector4f(float((camera.x - q->getPhysicalXCoordinate()) / q->getPhysicalLevel()),
		//	float((camera.y - q->getPhysicalYCoordinate()) / q->getPhysicalLevel()),
		//	float((camera.z - ground) / (q->getPhysicalLevel() * q->getOwner()->getDistFactor())),
		//	camera.z);

		material->getCurrentTechnique()->getPass(0)->getUniform("deformation.camera")->setValue(Vector4f(float((camera.x - q->getPhysicalXCoordinate()) / q->getPhysicalLevel()),
			float((camera.y - q->getPhysicalYCoordinate()) / q->getPhysicalLevel()),
			float((camera.z - ground) / (q->getPhysicalLevel() * q->getOwner()->getDistFactor())),
			camera.z));

		Matrix3f m = _localToTangent * Matrix3f(q->getPhysicalLevel(), 0.0, q->getPhysicalXCoordinate() - camera.x, 0.0, q->getPhysicalLevel(), q->getPhysicalYCoordinate() - camera.y, 0.0, 0.0, 1.0);
		
		MeshRenderContext::UniformParameter deformationTileToTangent;
		deformationTileToTangent.name = "deformation.tileToTangent";
		deformationTileToTangent.value.type = Uniform::Value::Mat3f;
		deformationTileToTangent.value.mat3 = m;

		material->getCurrentTechnique()->getPass(0)->getUniform("deformation.tileToTangent")->setValue(m);

		/*{
			PROFILE("Vector push");
			data.push_back(deformationOffset);
			data.push_back(deformationCamera);
			data.push_back(deformationTileToTangent);
		}*/

		//{
			//PROFILE("Deformation setScreenUniforms");
			setScreenUniforms(q, material);
		//}
	}

	void Deformation::setScreenUniforms(TerrainQuad* q, Material* material) const
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

		//MeshRenderContext::UniformParameter screenQuadCorners;
		//screenQuadCorners.name = "screenQuadCorners";
		//screenQuadCorners.value.type = Uniform::Value::Mat4f;
		//screenQuadCorners.value.mat4 = _localToScreen * corners;

		material->getCurrentTechnique()->getPass(0)->getUniform("screenQuadCorners")->setValue(_localToScreen * corners);

		Matrix4f verticals = Matrix4f(
			0.0, 0.0, 0.0, 0.0,
			0.0, 0.0, 0.0, 0.0,
			1.0, 1.0, 1.0, 1.0,
			0.0, 0.0, 0.0, 0.0);
		//MeshRenderContext::UniformParameter screenQuadVerticals;
		//screenQuadVerticals.name = "screenQuadVerticals";
		//screenQuadVerticals.value.type = Uniform::Value::Mat4f;
		//screenQuadVerticals.value.mat4 = _localToScreen * verticals;

		material->getCurrentTechnique()->getPass(0)->getUniform("screenQuadVerticals")->setValue(_localToScreen * verticals);

		//MeshRenderContext::UniformParameter worldSunDir;
		//worldSunDir.name = "worldSunDir";
		//worldSunDir.value.type = Uniform::Value::Vec3f;
		//worldSunDir.value.vec3 = Vector3f(0, -1, 0);

		material->getCurrentTechnique()->getPass(0)->getUniform("worldSunDir")->setValue(Vector3f(0, -1, 0));

		//MeshRenderContext::UniformParameter hdrExposure;
		//hdrExposure.name = "hdrExposure";
		//hdrExposure.value.type = Uniform::Value::Float;
		//hdrExposure.value.vec3 = 0.4f;

		material->getCurrentTechnique()->getPass(0)->getUniform("hdrExposure")->setValue(0.4f);


		//data.push_back(screenQuadCorners);
		//data.push_back(screenQuadVerticals);
		//data.push_back(worldSunDir);
		//data.push_back(hdrExposure);
	}
}