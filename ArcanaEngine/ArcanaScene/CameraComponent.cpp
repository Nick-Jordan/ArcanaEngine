#include "CameraComponent.h"

namespace Arcana
{
	CameraComponent::CameraComponent() : SceneComponent(), _camera(nullptr)
	{
	}

	CameraComponent::CameraComponent(float fov, float aspect, float nearPlane, float farPlane)
	{
		initialize(fov, aspect, nearPlane, farPlane);
	}

	CameraComponent::CameraComponent(float zoomX, float zoomY, float aspect, float nearPlane, float farPlane)
	{
		initialize(zoomX, zoomY, aspect, nearPlane, farPlane);
	}

	CameraComponent::CameraComponent(const CameraComponent& component) : _camera(component._camera)
	{
		_camera->reference();
	}

	CameraComponent::~CameraComponent()
	{
		AE_RELEASE(_camera);
	}

	void CameraComponent::initialize(float fov, float aspect, float nearPlane, float farPlane)
	{
		SceneComponent::initialize();

		_camera = new Camera(fov, aspect, nearPlane, farPlane);
		_camera->reference();
	}

	void CameraComponent::initialize(float zoomX, float zoomY, float aspect, float nearPlane, float farPlane)
	{
		SceneComponent::initialize();

		_camera = new Camera(zoomX, zoomY, aspect, nearPlane, farPlane);
		_camera->reference();
	}

	const Matrix4d& CameraComponent::getProjectionMatrix() const
	{
		return _camera->getProjectionMatrix();
	}
}
