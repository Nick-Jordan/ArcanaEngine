#include "ProceduralDataGraph.h"

namespace Arcana
{

	ProceduralDataGraph::ProceduralDataGraph()
	{
	}


	ProceduralDataGraph::~ProceduralDataGraph()
	{
		for (auto i = _dependentData.createIterator(); i; i++)
		{
			AE_RELEASE(*i);
		}
		_dependentData.empty();
	}

	void ProceduralDataGraph::registerData(ProceduralData* data)
	{
		if (data)
		{
			data->reference();
			_dependentData.add(data);
		}
	}

	void ProceduralDataGraph::generate(const ProceduralParameters& params, const Seed& seed)
	{
		for (auto i = _dependentData.createIterator(); i; i++)
		{
			ProceduralData* data = (*i);

			if (data)
			{
				data->generate(params, seed);
			}
		}
	}
}