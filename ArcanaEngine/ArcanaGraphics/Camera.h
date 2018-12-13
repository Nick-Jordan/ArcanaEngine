#ifndef CAMERA_H_

#include "GraphicsDefines.h"

#include "Object.h"
#include "Transform.h"

namespace Arcana
{

	class ARCANA_GRAPHICS_API Camera : public Transform
	{
	public:
		
		enum Type
		{
			Unknown = -1,
			Perspective,
			Orthographic,

			NUM_CAMERA_TYPES
		};

		Camera();

		Camera(float fov, float aspect, float nearPlane, float farPlane);

		Camera(float zoomX, float zoomY, float aspect, float nearPlane, float farPlane);

		virtual ~Camera();

		
		void createPerspective(float fov, float aspect, float nearPlane, float farPlane);

		void createOrthographic(float zoomX, float zoomY, float aspect, float nearPlane, float farPlane);


		Type getType() const;

		void setFieldOfView(float fov);

		float getFieldOfView() const;

		void setAspectRatio(float aspect);

		float getAspectRatio() const;

		void setNearPlane(float nearPlane);

		float getNearPlane() const;

		void setFarPlane(float farPlane);

		float getFarPlane() const;

		void setZoomX(float zoomX);

		float getZoomX() const;

		void setZoomY(float zoomY);

		float getZoomY() const;


		const Matrix4d& getViewMatrix();

		const Matrix4d& getProjectionMatrix();

	private:

		enum DirtyBits : int32
		{
			None = 0,
			Projection = 1 << 0,
			View = 1 << 1
		};

		void dirty(int32 bit);

		void dirtyClear(int32 bit);

		bool isDirty(int32 bit) const;

	private:

		Type _type;
		float _fieldOfView;
		float _aspectRatio;
		float _nearPlane;
		float _farPlane;
		float _zoomX;
		float _zoomY;

		Matrix4d _projection;
		Matrix4d _view;

		int32 _dirty;
	};

}

#endif // !CAMERA_H_

