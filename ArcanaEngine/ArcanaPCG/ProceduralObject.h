#ifndef PROCEDURAL_OBJECT_H_
#define PROCEDURAL_OBJECT_H_

#include "PCGDefines.h"

#include "Object.h"
#include "ProceduralDataGraph.h"

namespace Arcana
{
	class ARCANA_PCG_API ProceduralObject : public Object
	{
	public:

		ProceduralObject(const std::string& type, const std::string& objectTypeId, const ProceduralParameters& params = ProceduralParameters());

		virtual ~ProceduralObject();

		void registerData(ProceduralData* data);

		void generate(const Seed& seed);

		void generate(const ProceduralParameters& params, const Seed& seed);

	private:

		const ProceduralParameters& _defaultParameters;

		ProceduralDataGraph* _rootData;
	};
}

#endif // !PROCEDURAL_OBJECT_H_

