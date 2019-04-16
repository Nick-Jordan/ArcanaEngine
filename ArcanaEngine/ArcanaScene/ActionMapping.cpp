#include "ActionMapping.h"

namespace Arcana
{
	std::vector<ActionMapping> ActionMapping::__actionMappings;

	void ActionMapping::registerActionMapping(const std::string& name, std::vector<Key> keys)
	{
		if (get(name) == nullptr)
		{
			__actionMappings.push_back(ActionMapping(name, keys));
		}
	}

	const ActionMapping* ActionMapping::get(const std::string& name)
	{
		for (uint32 i = 0; i < __actionMappings.size(); i++)
		{
			if (__actionMappings[i]._name.compare(name) == 0)
			{
				return &__actionMappings[i];
			}
		}

		return nullptr;
	}

	ActionMapping::ActionMapping(const std::string& name, std::vector<Key> keys)
		: _name(name), _keys(keys)
	{

	}

	ActionMapping::ActionMapping(const ActionMapping& copy) : _name(copy._name), _keys(copy._keys)
	{

	}

	ActionMapping::~ActionMapping()
	{

	}
}
