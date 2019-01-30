#ifndef PROCEDURAL_PARAMETERS_H_
#define PROCEDURAL_PARAMETERS_H_

#include "PCGDefines.h"

#include "Shader.h"

namespace Arcana
{
	class ARCANA_PCG_API ProceduralParameters
	{
	public:
		
		//add base procedural params

		ProceduralParameters() { };

		virtual ~ProceduralParameters() { };

		virtual void setUniforms(Shader& shader) const { };
	};
}

#endif // !PROCEDURAL_PARAMETERS_H_

