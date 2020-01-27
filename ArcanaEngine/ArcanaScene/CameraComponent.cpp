#include "CameraComponent.h"

#include "ResourceManager.h"
#include "ResourceCreator.h"

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

	class CameraComponentResource : public ResourceCreator<CameraComponent>
	{
	public:

		CameraComponentResource(const GlobalObjectID& id, const std::string& type, const ResourceData& data, Scheduler* dependencyScheduler)
			: ResourceCreator<CameraComponent>(id, type, data, dependencyScheduler)
		{
			//scene component stuff

			std::string s = data.getStringParameter("type");

			if (s == "perspective")
			{
				float fov = data.getFloatParameter("fov");
				float aspect = data.getFloatParameter("aspect");
				float nearPlane = data.getFloatParameter("nearPlane");
				float farPlane = data.getFloatParameter("farPlane");
				initialize(fov, aspect, nearPlane, farPlane);
			}
			else
			{
				float zoomX = data.getFloatParameter("zoomX");
				float zoomY = data.getFloatParameter("zoomY");
				float aspect = data.getFloatParameter("aspect");
				float nearPlane = data.getFloatParameter("nearPlane");
				float farPlane = data.getFloatParameter("farPlane");
				initialize(zoomX, zoomY, aspect, nearPlane, farPlane);
			}
		}
	};

	Resource::Type<CameraComponentResource> cameraComponentResource("cameraComponent");
}