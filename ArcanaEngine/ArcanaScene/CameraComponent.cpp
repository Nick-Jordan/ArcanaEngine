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
		AE_REFERENCE(_camera);
	}

	CameraComponent::~CameraComponent()
	{
		AE_RELEASE(_camera);
	}

	const Sphered& CameraComponent::getBoundingSphere()
	{
		if (_camera)
		{
			_boundingSphere.setRadius((double)_camera->getNearPlane());
		}

		_boundingSphere.setCenter(getWorldTransform().getTranslation());

		return _boundingSphere;
	}

	Camera* CameraComponent::getCamera() const
	{
		return _camera;
	}

	void CameraComponent::initialize(float fov, float aspect, float nearPlane, float farPlane)
	{
		SceneComponent::initialize();

		_camera = new Camera(fov, aspect, nearPlane, farPlane);
		_camera->reference();

		_boundingSphere.setRadius((double)nearPlane);
	}

	void CameraComponent::initialize(float zoomX, float zoomY, float aspect, float nearPlane, float farPlane)
	{
		SceneComponent::initialize();

		_camera = new Camera(zoomX, zoomY, aspect, nearPlane, farPlane);
		_camera->reference();

		_boundingSphere.setRadius((double)nearPlane);
	}

	const Matrix4d& CameraComponent::getProjectionMatrix() const
	{
		return _camera->getProjectionMatrix();
	}
}