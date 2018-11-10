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

