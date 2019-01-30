#include "ProceduralObject.h"

namespace Arcana
{

	ProceduralObject::ProceduralObject(const std::string& type, const std::string& objectTypeId, const ProceduralParameters& params)
		: Object(type), _rootData(new ProceduralDataGraph()), _defaultParameters(params)
	{
	}


	ProceduralObject::~ProceduralObject()
	{
		AE_DELETE(_rootData);
	}

	void ProceduralObject::registerData(ProceduralData* data)
	{
		if (_rootData)
		{
			_rootData->registerData(data);
		}
	}

	void ProceduralObject::generate(const Seed& seed)
	{
		if (_rootData)
		{
			_rootData->generate(_defaultParameters, seed);
		}
	}

	void ProceduralObject::generate(const ProceduralParameters& params, const Seed& seed)
	{
		if (_rootData)
		{
			_rootData->generate(params, seed);
		}
	}
}