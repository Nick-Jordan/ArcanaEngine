#ifndef PROCEDURAL_DATA_GRAPH_H_
#define PROCEDURAL_DATA_GRAPH_H_

#include "PCGDefines.h"

#include "ProceduralData.h"
#include "Array.h"

namespace Arcana
{
	class ARCANA_PCG_API ProceduralDataGraph : public ProceduralData
	{
	public:

		ProceduralDataGraph();

		virtual ~ProceduralDataGraph();

		void registerData(ProceduralData* data);

		virtual void generate(const ProceduralParameters& params, const Seed& seed) override;

	private:

		Array<ProceduralData*> _dependentData;
	};
}
#endif // !PROCEDURAL_DATA_GRAPH_H_

