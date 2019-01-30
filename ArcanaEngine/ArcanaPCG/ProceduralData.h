#ifndef PROCEDURAL_DATA_H_
#define PROCEDURAL_DATA_H_

#include "PCGDefines.h"

#include "Object.h"
#include "ProceduralParameters.h"
#include "Seed.h"

namespace Arcana
{

	class ARCANA_PCG_API ProceduralData : public Object
	{
	public:

		ProceduralData();

		virtual ~ProceduralData();

		virtual void generate(const ProceduralParameters& params, const Seed& seed) = 0;
	};

}

#endif // !PROCEDURAL_DATA_H_

