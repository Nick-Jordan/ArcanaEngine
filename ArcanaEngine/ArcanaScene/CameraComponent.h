#ifndef CAMERA_COMPONENT_H_
#define CAMERA_COMPONENT_H_

#include "SceneDefines.h"

#include "SceneComponent.h"
#include "Camera.h"
#include "Sphere.h"

namespace Arcana
{

	class ARCANA_SCENE_API CameraComponent : public SceneComponent
	{
	public:

		CameraComponent();

		CameraComponent(float fov, float aspect, float nearPlane, float farPlane);

		CameraComponent(float zoomX, float zoomY, float aspect, float nearPlane, float farPlane);

		CameraComponent(const CameraComponent& component);

		virtual ~CameraComponent();

		virtual const Sphered& getBoundingSphere() override;

		Camera* getCamera() const;

		void initialize(float fov, float aspect, float nearPlane, float farPlane);

		void initialize(float zoomX, float zoomY, float aspect, float nearPlane, float farPlane);

		const Matrix4d& getProjectionMatrix() const;

	private:

		Camera* _camera;

		Sphered _boundingSphere;
	};

}

#endif // !CAMERA_COMPONENT_H_

