#ifndef UNIFORM_H_
#define UNIFORM_H_

#include "GraphicsDefines.h"

#include "../Dependencies/include/opengl/include.h"
#include <string>

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix3.h"
#include "Matrix4.h"

#include "Types.h"
#include "SmartPtr.h"

#include "Texture.h"

namespace Arcana
{
	class ARCANA_GRAPHICS_API Shader;

	class ARCANA_GRAPHICS_API Uniform
	{
	public:

		class ARCANA_GRAPHICS_API Value
		{
		public:

			Value() {}
			~Value() {}

			enum Type
			{
				Bool,
				Int32,
				Uint32,
				Float,
				Double,
				Vec2b,
				Vec3b,
				Vec4b,
				Vec2i,
				Vec3i,
				Vec4i,
				Vec2u,
				Vec3u,
				Vec4u,
				Vec2f,
				Vec3f,
				Vec4f,
				Vec2d,
				Vec3d,
				Vec4d,
				Mat3f,
				Mat4f,
				Mat3d,
				Mat4d
			};

			Type type;

			union
			{
				bool b;
				int32 i;
				uint32 ui;
				float f;
				double d;

				Vector2<bool> bvec2;
				Vector3<bool> bvec3;
				Vector4<bool> bvec4;

				Vector2<int32> ivec2;
				Vector3<int32> ivec3;
				Vector4<int32> ivec4;

				Vector2<uint32> uivec2;
				Vector3<uint32> uivec3;
				Vector4<uint32> uivec4;

				Vector2<float> vec2;
				Vector3<float> vec3;
				Vector4<float> vec4;

				Vector2<double> dvec2;
				Vector3<double> dvec3;
				Vector4<double> dvec4;

				Matrix3f mat3;
				Matrix4f mat4;

				Matrix3d dmat3;
				Matrix4d dmat4;
			};
		};
		
		static SmartPtr<Uniform> get(Shader* shader, const std::string& name);

		void setValue(bool b);
		void setValue(int32 i);
		void setValue(uint32 ui);
		void setValue(float f);
		void setValue(double d);

		void setValue(const Vector2<bool>& bvec2);
		void setValue(const Vector3<bool>& bvec3);
		void setValue(const Vector4<bool>& bvec4);
		void setValue(const Vector2<int32>& ivec2);
		void setValue(const Vector3<int32>& ivec3);
		void setValue(const Vector4<int32>& ivec4);
		void setValue(const Vector2<uint32>& uivec2);
		void setValue(const Vector3<uint32>& uivec3);
		void setValue(const Vector4<uint32>& uivec4);
		void setValue(const Vector2<float>& vec2);
		void setValue(const Vector3<float>& vec3);
		void setValue(const Vector4<float>& vec4);
		void setValue(const Vector2<double>& dvec2);
		void setValue(const Vector3<double>& dvec3);
		void setValue(const Vector4<double>& dvec4);

		void setValue(const Matrix3f& mat3);
		void setValue(const Matrix4f& mat4);
		void setValue(const Matrix3d& dmat3);
		void setValue(const Matrix4d& dmat4);

		void setValue(const Value& value);

		Uniform(const Uniform& uniform);

		Uniform& operator=(const Uniform& uniform);

	private:

		Uniform(Shader* parent, GLint location);

	private:

		Shader* _parent;
		GLint _location;
	};

}

#endif // !UNIFORM_H_

