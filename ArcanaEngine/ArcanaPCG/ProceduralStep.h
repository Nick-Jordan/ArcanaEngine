#ifndef PROCEDURAL_STEP_H_
#define PROCEDURAL_STEP_H_

#include "PCGDefines.h"

#include "Object.h"

namespace Arcana
{
	template<class ProceduralObjectType, class ProceduralParametersType>
	class ProceduralStep : public Object
	{
	public:

		virtual void perform(
			const ProceduralParametersType& params,
			ProceduralStep<ProceduralObjectType, ProceduralParametersType>* previous, 
			ProceduralObjectType** object) = 0;
	};
}

#endif // !PROCEDURAL_STEP_H_

