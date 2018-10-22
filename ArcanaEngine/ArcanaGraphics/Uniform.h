#ifndef UNIFORM_H_
#define UNIFORM_H_

#include "GraphicsDefines.h"

#include "opengl/include.h"
#include <string>

namespace Arcana
{
	class ARCANA_GRAPHICS_API Shader;

	class ARCANA_GRAPHICS_API Uniform
	{
	public:
		
		static Uniform& get(Shader* shader, const std::string& name);


		//test
		void setValue(float f);


		bool exists() const;

		Uniform(const Uniform& uniform);

		Uniform& operator=(const Uniform& uniform);

	private:

		Uniform(Shader* parent, GLint location);

	private:

		Shader* _parent;
		GLint _location;
		bool _exists;
	};

}

#endif // !UNIFORM_H_

