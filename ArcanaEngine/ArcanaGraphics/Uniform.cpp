#include "Uniform.h"

#include "Shader.h"

namespace Arcana
{
	Uniform& Uniform::get(Shader* shader, const std::string& name)
	{
		GLint location = glGetUniformLocation(shader->getId(), name.c_str());

		if (location != -1)
		{
			return Uniform(shader, location);
		}

		return Uniform(nullptr, -1);
	}

	void Uniform::setValue(float f)
	{
		glUniform1f(_location, f);
	}


	bool Uniform::exists() const
	{
		return _exists;
	}

	Uniform::Uniform(const Uniform& uniform) : _parent(uniform._parent), _location(uniform._location), _exists(uniform._exists)
	{

	}

	Uniform& Uniform::operator=(const Uniform& uniform)
	{
		_parent = uniform._parent;
		_location = uniform._location;
		_exists = uniform._exists;

		return *this;
	}

	Uniform::Uniform(Shader* parent, GLint location) : _parent(parent), _location(location)
	{
		_exists = parent != nullptr && _location != -1;
	}
}
