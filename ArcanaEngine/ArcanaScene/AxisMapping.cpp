#include "AxisMapping.h"

namespace Arcana
{
	std::vector<AxisMapping> AxisMapping::__axisMappings;

	void AxisMapping::registerAxisMapping(const std::string& name, const std::vector<KeyMapping>& keyMappings)
	{
		if (get(name) == nullptr)
		{
			__axisMappings.push_back(AxisMapping(name, keyMappings));
		}
	}

	const AxisMapping* AxisMapping::get(const std::string& name)
	{
		for (uint32 i = 0; i < __axisMappings.size(); i++)
		{
			if (__axisMappings[i]._name.compare(name) == 0)
			{
				return &__axisMappings[i];
			}
		}

		return nullptr;
	}

	AxisMapping::AxisMapping(const std::string& name, const std::vector<KeyMapping>& keyMappings)
		: _name(name), _keyMappings(keyMappings)
	{

	}

	AxisMapping::AxisMapping(const AxisMapping& copy) : _name(copy._name), _keyMappings(copy._keyMappings)
	{

	}

	AxisMapping::~AxisMapping()
	{

	}
}
