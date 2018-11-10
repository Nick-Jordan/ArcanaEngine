#include "Uniform.h"

#include "Shader.h"

namespace Arcana
{

	SmartPtr<Uniform> Uniform::get(Shader* shader, const std::string& name)
	{
		GLint location = glGetUniformLocation(shader->getId(), name.c_str());

		if (location != -1)
		{
			return new Uniform(shader, location);
		}

		return nullptr;
	}

	void Uniform::setValue(bool b)
	{
		_parent->bind();
		glUniform1i(_location, b);
	}
	void Uniform::setValue(int32 i)
	{
		_parent->bind();
		glUniform1i(_location, i);
	}
	void Uniform::setValue(uint32 ui)
	{
		_parent->bind();
		glUniform1ui(_location, ui);
	}
	void Uniform::setValue(float f)
	{
		_parent->bind();
		glUniform1f(_location, f);
	}
	void Uniform::setValue(double d)
	{
		_parent->bind();
		glUniform1d(_location, d);
	}

	void Uniform::setValue(const Vector2<bool>& bvec2)
	{
		_parent->bind();
		glUniform2i(_location, bvec2.x, bvec2.y);
	}
	void Uniform::setValue(const Vector3<bool>& bvec3)
	{
		_parent->bind();
		glUniform3i(_location, bvec3.x, bvec3.y, bvec3.z);
	}
	void Uniform::setValue(const Vector4<bool>& bvec4)
	{
		_parent->bind();
		glUniform4i(_location, bvec4.x, bvec4.y, bvec4.z, bvec4.w);
	}
	void Uniform::setValue(const Vector2<int32>& ivec2)
	{
		_parent->bind();
		glUniform2i(_location, ivec2.x, ivec2.y);
	}
	void Uniform::setValue(const Vector3<int32>& ivec3)
	{
		_parent->bind();
		glUniform3i(_location, ivec3.x, ivec3.y, ivec3.z);
	}
	void Uniform::setValue(const Vector4<int32>& ivec4)
	{
		_parent->bind();
		glUniform4i(_location, ivec4.x, ivec4.y, ivec4.z, ivec4.w);
	}
	void Uniform::setValue(const Vector2<uint32>& uivec2)
	{
		_parent->bind();
		glUniform2ui(_location, uivec2.x, uivec2.y);
	}
	void Uniform::setValue(const Vector3<uint32>& uivec3)
	{
		_parent->bind();
		glUniform3ui(_location, uivec3.x, uivec3.y, uivec3.z);
	}
	void Uniform::setValue(const Vector4<uint32>& uivec4)
	{
		_parent->bind();
		glUniform4ui(_location, uivec4.x, uivec4.y, uivec4.z, uivec4.w);
	}
	void Uniform::setValue(const Vector2<float>& vec2)
	{
		_parent->bind();
		glUniform2f(_location, vec2.x, vec2.y);
	}
	void Uniform::setValue(const Vector3<float>& vec3)
	{
		_parent->bind();
		glUniform3f(_location, vec3.x, vec3.y, vec3.z);
	}
	void Uniform::setValue(const Vector4<float>& vec4)
	{
		_parent->bind();
		glUniform4f(_location, vec4.x, vec4.y, vec4.z, vec4.w);
	}
	void Uniform::setValue(const Vector2<double>& dvec2)
	{
		_parent->bind();
		glUniform2d(_location, dvec2.x, dvec2.y);
	}
	void Uniform::setValue(const Vector3<double>& dvec3)
	{
		_parent->bind();
		glUniform3d(_location, dvec3.x, dvec3.y, dvec3.z);
	}

	void Uniform::setValue(const Vector4<double>& dvec4)
	{
		_parent->bind();
		glUniform4d(_location, dvec4.x, dvec4.y, dvec4.z, dvec4.w);
	}

	void Uniform::setValue(const Matrix3f& mat3)
	{
		_parent->bind();
		glUniformMatrix3fv(_location, 1, false, mat3.getValuePointer());
	}
	void Uniform::setValue(const Matrix4f& mat4)
	{
		_parent->bind();
		glUniformMatrix4fv(_location, 1, false, mat4.getValuePointer());
	}
	void Uniform::setValue(const Matrix3d& dmat3)
	{
		_parent->bind();
		glUniformMatrix3dv(_location, 1, false, dmat3.getValuePointer());
	}
	void Uniform::setValue(const Matrix4d& dmat4)
	{
		_parent->bind();
		glUniformMatrix4dv(_location, 1, false, dmat4.getValuePointer());
	}


	Uniform::Uniform(const Uniform& uniform) : _parent(uniform._parent), _location(uniform._location)
	{

	}

	Uniform& Uniform::operator=(const Uniform& uniform)
	{
		_parent = uniform._parent;
		_location = uniform._location;

		return *this;
	}

	Uniform::Uniform(Shader* parent, GLint location) : _parent(parent), _location(location)
	{

	}
}
