#include "Camera.h"

namespace Arcana
{
	Camera::Camera() : _type(Unknown), 
		_fieldOfView(0.0f), _aspectRatio(0.0f), _nearPlane(0.0f), _farPlane(0.0f), _zoomX(0.0f), _zoomY(0.0f)
	{
		set(Vector3d::zero(), Vector3d::one(), Matrix4d::IDENTITY);
	}

	Camera::Camera(float fov, float aspect, float nearPlane, float farPlane)
	{
		set(Vector3d::zero(), Vector3d::one(), Matrix4d::IDENTITY);
		createPerspective(fov, aspect, nearPlane, farPlane);
	}

	Camera::Camera(float zoomX, float zoomY, float aspect, float nearPlane, float farPlane)
	{
		set(Vector3d::zero(), Vector3d::one(), Matrix4d::IDENTITY);
		createOrthographic(zoomX, zoomY, aspect, nearPlane, farPlane);
	}

	Camera::~Camera()
	{

	}


	void Camera::createPerspective(float fov, float aspect, float nearPlane, float farPlane)
	{
		_type = Perspective;
		_fieldOfView = fov;
		_aspectRatio = aspect;
		_nearPlane = nearPlane;
		_farPlane = farPlane;
		_zoomX = 0.0f;
		_zoomY = 0.0f;

		dirty(View | Projection);
	}

	void Camera::createOrthographic(float zoomX, float zoomY, float aspect, float nearPlane, float farPlane)
	{
		_type = Orthographic;
		_fieldOfView = 0.0f;
		_aspectRatio = aspect;
		_nearPlane = nearPlane;
		_farPlane = farPlane;
		_zoomX = zoomX;
		_zoomY = zoomY;

		dirty(View | Projection);
	}


	Camera::Type Camera::getType() const
	{
		return _type;
	}

	void Camera::setFieldOfView(float fov)
	{
		if (_type == Perspective)
		{
			_fieldOfView = fov;

			dirty(Projection);
		}
	}

	float Camera::getFieldOfView() const
	{
		return _fieldOfView;
	}

	void Camera::setAspectRatio(float aspect)
	{
		_aspectRatio = aspect;

		dirty(Projection);
	}

	float Camera::getAspectRatio() const
	{
		return _aspectRatio;
	}

	void Camera::setNearPlane(float nearPlane)
	{
		_nearPlane = nearPlane;

		dirty(Projection);
	}

	float Camera::getNearPlane() const
	{
		return _nearPlane;
	}

	void Camera::setFarPlane(float farPlane)
	{
		_farPlane = farPlane;

		dirty(Projection);
	}

	float Camera::getFarPlane() const
	{
		return _farPlane;
	}

	void Camera::setZoomX(float zoomX)
	{
		if (_type == Orthographic)
		{
			_zoomX = zoomX;

			dirty(Projection);
		}
	}

	float Camera::getZoomX() const
	{
		return _zoomX;
	}

	void Camera::setZoomY(float zoomY)
	{
		if (_type == Orthographic)
		{
			_zoomY = zoomY;

			dirty(Projection);
		}
	}

	float Camera::getZoomY() const
	{
		return _zoomX;
	}

	const Matrix4f& Camera::getViewMatrix()
	{
		if (isDirty(View))
		{
			_view = getMatrix().cast<float>();

			dirtyClear(View);
		}

		return _view;
	}

	const Matrix4f& Camera::getProjectionMatrix()
	{
		if (isDirty(Projection))
		{
			if (_type == Perspective)
			{
				_projection = Matrix4f::createPerspective(_fieldOfView, _aspectRatio, _nearPlane, _farPlane);
			}
			else if (_type == Orthographic)
			{
				_projection = Matrix4f::createOrthographic(_zoomX, _zoomY, _nearPlane, _farPlane);
			}

			dirtyClear(Projection);
		}

		return _projection;
	}

	void Camera::dirty(int32 bit)
	{
		_dirty |= bit;
	}

	void Camera::dirtyClear(int32 bit)
	{
		_dirty &= ~bit;
	}

	bool Camera::isDirty(int32 bit) const
	{
		return (_dirty & bit);
	}
}
