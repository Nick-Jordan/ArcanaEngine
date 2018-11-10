#ifndef SAMPLER_H_
#define SAMPLER_H_

#include "GraphicsDefines.h"

#include "../Dependencies/include/opengl/include.h"

namespace Arcana
{

	class ARCANA_GRAPHICS_API Sampler
	{
	public:

		Sampler();
		~Sampler();

		GLuint getId() const
		{
			return 0;
		}
	};
}

#endif // !SAMPLER_H_