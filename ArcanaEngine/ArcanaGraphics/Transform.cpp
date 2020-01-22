#include "Transform.h"

#include "ResourceManager.h"
#include "ResourceCreator.h"

namespace Arcana
{
	Transform::Transform()
	{
		set(Vector3d::zero(), Vector3d::one(), Matrix4d::IDENTITY);
	}
	Transform::Transform(const Transform& transform)
	{
		set(transform);
	}
	Transform::Transform(const Vector3d& translation, const Vector3d& scale, const Quaterniond& rotation) : _dirty(None)
	{
		set(translation, scale, rotation);
	}
	Transform::Transform(const Vector3d& translation, const Vector3d& scale, const Matrix4d& rotation) : _dirty(None)
	{
		set(translation, scale, rotation);
	}
	Transform::Transform(const Vector3d& translation, const Vector3d& scale, const Vector3d& rotationAxis, double rotationAngle) : _dirty(None)
	{
		set(translation, scale, rotationAxis, rotationAngle);
	}

	Transform::~Transform()
	{

	}

	const Matrix4d& Transform::getMatrix() const
	{
		if (isDirty(Translation) || isDirty(Scale) || isDirty(Rotation))
		{
			Matrix4d translation = Matrix4d::createTranslation(_translation);
			Matrix4d scale = Matrix4d::createScale(_scale);
			Matrix4d rotation = _rotation.toMatrix4();

			_matrix.identity();

			_matrix = scale * rotation * translation;
		}

		return _matrix;
	}

	const Vector3d& Transform::getTranslation() const
	{
		return _translation;
	}
	const Vector3d& Transform::getScale() const
	{
		return _scale;
	}
	const Quaterniond& Transform::getRotation() const
	{
		return _rotation;
	}

	Vector3d Transform::getForwardVector()
	{
		return getMatrix().getForwardVector();
	}
	Vector3d Transform::getBackVector()
	{
		return getMatrix().getBackVector();
	}
	Vector3d Transform::getUpVector()
	{
		return getMatrix().getUpVector();
	}
	Vector3d Transform::getDownVector()
	{
		return getMatrix().getDownVector();
	}
	Vector3d Transform::getLeftVector()
	{
		return getMatrix().getLeftVector();
	}
	Vector3d Transform::getRightVector()
	{
		return getMatrix().getRightVector();
	}


	void Transform::set(const Transform& transform)
	{
		setTranslation(transform.getTranslation());
		setScale(transform.getScale());
		setRotation(transform.getRotation());
	}

	void Transform::set(const Vector3d& translation, const Vector3d& scale, const Quaterniond& rotation)
	{
		setTranslation(translation);
		setScale(scale);
		setRotation(rotation);
	}
	void Transform::set(const Vector3d& translation, const Vector3d& scale, const Matrix4d& rotation)
	{
		setTranslation(translation);
		setScale(scale);
		setRotation(rotation);
	}
	void Transform::set(const Vector3d& translation, const Vector3d& scale, const Vector3d& rotationAxis, double rotationAngle)
	{
		setTranslation(translation);
		setScale(scale);
		setRotation(rotationAxis, rotationAngle);
	}

	void Transform::transform(const Transform& transform)
	{
		translate(transform.getTranslation());
		scale(transform.getScale());
		rotate(transform.getRotation());
	}

	void Transform::setTranslation(double x, double y, double z)
	{
		_translation.x = x;
		_translation.y = y;
		_translation.z = z;

		dirty(Translation);
	}
	void Transform::setTranslationX(double x)
	{
		_translation.x = x;

		dirty(Translation);
	}
	void Transform::setTranslationY(double y)
	{
		_translation.y = y;

		dirty(Translation);
	}
	void Transform::setTranslationZ(double z)
	{
		_translation.z = z;

		dirty(Translation);
	}
	void Transform::setTranslation(const Vector3d& translation)
	{
		_translation = translation;

		dirty(Translation);
	}

	void Transform::translate(double x, double y, double z)
	{
		_translation.x += x;
		_translation.y += y;
		_translation.z += z;

		dirty(Translation);
	}
	void Transform::translateX(double x)
	{
		_translation.x += x;

		dirty(Translation);
	}
	void Transform::translateY(double y)
	{
		_translation.y += y;

		dirty(Translation);
	}
	void Transform::translateZ(double z)
	{
		_translation.z += z;

		dirty(Translation);
	}
	void Transform::translate(const Vector3d& translation)
	{
		_translation += translation;

		dirty(Translation);
	}

	void Transform::setScale(double x, double y, double z)
	{
		_scale.x = x;
		_scale.y = y;
		_scale.z = z;

		dirty(Scale);
	}
	void Transform::setScaleX(double x)
	{
		_scale.x = x;

		dirty(Scale);
	}
	void Transform::setScaleY(double y)
	{
		_scale.y = y;

		dirty(Scale);
	}
	void Transform::setScaleZ(double z)
	{
		_scale.z = z;

		dirty(Scale);
	}
	void Transform::setScale(const Vector3d& scale)
	{
		_scale = scale;

		dirty(Scale);
	}

	void Transform::scale(double x, double y, double z)
	{
		_scale.x *= x;
		_scale.y *= y;
		_scale.z *= z;

		dirty(Scale);
	}
	void Transform::scaleX(double x)
	{
		_scale.x *= x;

		dirty(Scale);
	}
	void Transform::scaleY(double y)
	{
		_scale.y *= y;

		dirty(Scale);
	}
	void Transform::scaleZ(double z)
	{
		_scale.z *= z;

		dirty(Scale);
	}
	void Transform::scale(const Vector3d& scale)
	{
		_scale.x *= scale.x;
		_scale.y *= scale.y;
		_scale.z *= scale.z;

		dirty(Scale);
	}

	void Transform::setRotation(double qw, double qx, double qy, double qz)
	{
		_rotation = Quaterniond(qw, qx, qy, qz);

		dirty(Rotation);
	}
	void Transform::setRotation(const Vector3d& axis, double angle)
	{
		_rotation.fromAxisAngle(axis, angle);

		dirty(Rotation);
	}
	void Transform::setRotation(const Quaterniond& rotation)
	{
		_rotation = rotation;

		dirty(Rotation);
	}
	void Transform::setRotation(const Matrix4d& rotation)
	{
		_rotation.fromMatrix4(rotation);

		dirty(Rotation);
	}

	void Transform::rotate(double qw, double qx, double qy, double qz)
	{
		Quaterniond rotation = Quaterniond(qw, qx, qy, qz);
		_rotation *= rotation;

		dirty(Rotation);
	}
	void Transform::rotate(const Vector3d& axis, double angle)
	{
		Quaterniond rotation;
		rotation.fromAxisAngle(axis, angle);
		_rotation *= rotation;

		dirty(Rotation);
	}
	void Transform::rotate(const Quaterniond& rotation)
	{
		_rotation *= rotation;

		dirty(Rotation);
	}
	void Transform::rotate(const Matrix4d& rotation)
	{
		Quaterniond rotationQuat;
		rotationQuat.fromMatrix4(rotation);
		_rotation *= rotationQuat;

		dirty(Rotation);
	}
	void Transform::rotateX(double x)
	{
		Quaterniond rotation;
		rotation.fromAxisAngle(Vector3d::unitX(), x);
		_rotation *= rotation;

		dirty(Rotation);
	}
	void Transform::rotateY(double y)
	{
		Quaterniond rotation;
		rotation.fromAxisAngle(Vector3d::unitY(), y);
		_rotation *= rotation;

		dirty(Rotation);
	}
	void Transform::rotateZ(double z)
	{
		Quaterniond rotation;
		rotation.fromAxisAngle(Vector3d::unitZ(), z);
		_rotation *= rotation;

		dirty(Rotation);
	}

	void Transform::setIdentity()
	{
		_translation.set(0.0, 0.0, 0.0);
		_scale.set(0.0, 0.0, 0.0);
		_rotation.set(1.0, 0.0, 0.0, 0.0);
	}

	bool Transform::isIdentity() const
	{
		return _translation.isZero() && _scale.isOne() && _rotation.isIdentity();
	}

	void Transform::addListener(Transform::Listener* listener)
	{
		_listeners.add(listener);
	}

	void Transform::removeListener(Transform::Listener* listener)
	{
		_listeners.remove(listener);
	}

	Transform Transform::getRelativeTransform(const Transform& other) const
	{
		Transform relative;

		relative.setTranslation(other._translation - _translation);
		relative.setRotation(other._rotation - _rotation);
		Vector3d relScale;
		relScale.x = other._scale.x / _scale.x;
		relScale.y = other._scale.y / _scale.y;
		relScale.z = other._scale.z / _scale.z;
		relative.setScale(relScale);

		return relative;
	}

	Transform& Transform::operator=(const Transform& other)
	{
		set(other);
		return *this;
	}


	void Transform::dirty(int32 bit)
	{
		_dirty |= bit;

		transformChanged();
	}

	void Transform::dirtyClear(int32 bit)
	{
		_dirty &= ~bit;
	}

	bool Transform::isDirty(int32 bit) const
	{
		return (_dirty & bit);
	}

	void Transform::transformChanged()
	{
		for (auto i = _listeners.createIterator(); i; i++)
		{
			(*i)->transformChanged(this);
		}
	}

	class TransformResource : public ResourceCreator<Transform>
	{
	public:

		TransformResource(const GlobalObjectID& id, const std::string& type, const ResourceData& data)
			: ResourceCreator<Transform>(id, type, data)
		{
			setTranslationX(data.getDoubleParameter("x"));
			setTranslationY(data.getDoubleParameter("y"));
			setTranslationZ(data.getDoubleParameter("z"));

			setScaleX(data.getDoubleParameter("scaleX", 1.0));
			setScaleY(data.getDoubleParameter("scaleY", 1.0));
			setScaleZ(data.getDoubleParameter("scaleZ", 1.0));

			//rotation
		}
	};

	Resource::Type<TransformResource> transformResource("transform");
}
