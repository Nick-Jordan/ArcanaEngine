#ifndef SEED_H_
#define SEED_H_

#include "PCGDefines.h"

#include "Shader.h"

namespace Arcana
{
	class ARCANA_PCG_API Seed
	{
	public:

		virtual ~Seed();

		virtual void setUniforms(Shader& shader) const { };
	};
}

#endif // !SEED_H_

