#ifndef CAMERA_ACTOR_H_
#define CAMERA_ACTOR_H_

#include "GraphicsDefines.h"

#include "Actor.h"
#include "Camera.h"

namespace Arcana
{
	class ARCANA_SCENE_API CameraActor : public Actor
	{
	public:

		CameraActor(float fov, float aspect, float nearPlane, float farPlane);

		CameraActor(float zoomX, float zoomY, float aspect, float nearPlane, float farPlane);

		CameraActor(Camera* camera);

		virtual ~CameraActor();

		virtual void update(double elapsedTime) override;

		virtual void render(ObjectRenderer& renderer, const Matrix4f& view, const Matrix4f& projection) override;

		virtual const Matrix4f& getViewMatrix() override;

		virtual const Matrix4f& getProjectionMatrix() override;


		Camera* getCamera();

	private:

		Camera* _camera;
	};
}

#endif // !CAMERA_ACTOR_H_

