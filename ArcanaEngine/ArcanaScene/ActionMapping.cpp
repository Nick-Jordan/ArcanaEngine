#include "ActionMapping.h"

namespace Arcana
{
	std::vector<ActionMapping> ActionMapping::__actionMappings;

	void ActionMapping::registerActionMapping(const std::string& name, Key key)
	{
		if (get(name) == nullptr)
		{
			__actionMappings.push_back(ActionMapping(name, key));
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

	ActionMapping::ActionMapping(const std::string& name, Key key)
		: _name(name), _key(key)
	{

	}

	ActionMapping::ActionMapping(const ActionMapping& copy) : _name(copy._name), _key(copy._key)
	{

	}

	ActionMapping::~ActionMapping()
	{

	}
}
