#ifndef TRANSFORM_H_
#define TRANSFORM_H_

#include "SceneDefines.h"

#include "Vector3.h"
#include "Quaternion.h"
#include "Array.h"

namespace Arcana
{

	class ARCANA_SCENE_API Transform
	{
	public:

		class ARCANA_SCENE_API Listener
		{
		public:

			virtual ~Listener() { }

			virtual void transformChanged(Transform* transform) = 0;
		};

		Transform();
		Transform(const Transform& transform);
		Transform(const Vector3d& translation, const Vector3d& scale, const Quaterniond& rotation);
		Transform(const Vector3d& translation, const Vector3d& scale, const Matrix4d& rotation);
		Transform(const Vector3d& translation, const Vector3d& scale, const Vector3d& rotationAxis, double rotationAngle);

		~Transform();

		const Matrix4d& getMatrix();

		const Vector3d& getTranslation() const;
		const Vector3d& getScale() const;
		const Quaterniond& getRotation() const;
		
		Vector3d getForwardVector();
		Vector3d getBackVector();
		Vector3d getUpVector();
		Vector3d getDownVector();
		Vector3d getLeftVector();
		Vector3d getRightVector();


		void set(const Transform& transform);
		void set(const Vector3d& translation, const Vector3d& scale, const Quaterniond& rotation);
		void set(const Vector3d& translation, const Vector3d& scale, const Matrix4d& rotation);
		void set(const Vector3d& translation, const Vector3d& scale, const Vector3d& rotationAxis, double rotationAngle);

		void setTranslation(double x, double y, double z);
		void setTranslationX(double x);
		void setTranslationY(double y);
		void setTranslationz(double z);
		void setTranslation(const Vector3d& translation);

		void translate(double x, double y, double z);
		void translateX(double x);
		void translateY(double y);
		void translateZ(double z);
		void translate(const Vector3d& translation);

		void setScale(double x, double y, double z);
		void setScaleX(double x);
		void setScaleY(double y);
		void setScaleZ(double z);
		void setScale(const Vector3d& scale);

		void scale(double x, double y, double z);
		void scaleX(double x);
		void scaleY(double y);
		void scaleZ(double z);
		void scale(const Vector3d& scale);

		void setRotation(double qw, double qx, double qy, double qz);
		void setRotation(const Vector3d& axis, double angle);
		void setRotation(const Quaterniond& rotation);
		void setRotation(const Matrix4d& rotation);

		void rotate(double qw, double qx, double qy, double qz);
		void rotate(const Vector3d& axis, double angle);
		void rotate(const Quaterniond& rotation);
		void rotate(const Matrix4d& rotation);
		void rotateX(double x);
		void rotateY(double y);
		void rotateZ(double z);

		void setIdentity();
		bool isIdentity() const;

		void addListener(Transform::Listener* listener);

		void removeListener(Transform::Listener* listener);

	private:

		enum DirtyBits : int32
		{
			None = 0,
			Translation = 1 << 0,
			Scale = 1 << 1,
			Rotation = 1 << 2
		};

		void dirty(int32 bit);

		void dirtyClear(int32 bit);

		bool isDirty(int32 bit) const;

		void transformChanged();

	private:

		int32 _dirty;

		Vector3d _translation;
		Vector3d _scale;
		Quaterniond _rotation;

		mutable Matrix4d _matrix;

		Array<Transform::Listener*> _listeners;
	};

}

#endif // !TRANSFORM_H_
