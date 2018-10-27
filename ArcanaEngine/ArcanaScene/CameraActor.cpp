#include "CameraActor.h"

namespace Arcana
{
	CameraActor::CameraActor(float fov, float aspect, float nearPlane, float farPlane)
	{
		_camera = new Camera(fov, aspect, nearPlane, farPlane);
	}

	CameraActor::CameraActor(float zoomX, float zoomY, float aspect, float nearPlane, float farPlane)
	{
		_camera = new Camera(zoomX, zoomY, aspect, nearPlane, farPlane);
	}

	CameraActor::CameraActor(Camera* camera)
	{
		_camera = camera;
	}

	CameraActor::~CameraActor()
	{
		AE_RELEASE(_camera);
	}

	void CameraActor::update(double elapsedTime)
	{

	}

	void CameraActor::render(ObjectRenderer& renderer, const Matrix4f& view, const Matrix4f& projection)
	{

	}

	const Matrix4f& CameraActor::getViewMatrix()
	{
		return _camera->getViewMatrix();
	}

	const Matrix4f& CameraActor::getProjectionMatrix()
	{
		return _camera->getProjectionMatrix();
	}

	Camera* CameraActor::getCamera()
	{
		return _camera;
	}
}
