#include "GlobalObjectID.h"

namespace Arcana
{
	GlobalObjectID GlobalObjectID::Empty = GlobalObjectID();
	std::unordered_map<std::string, UUID> GlobalObjectID::__uuidMap;

	GlobalObjectID::GlobalObjectID() : _name("Empty"), _id()
	{
	}

	GlobalObjectID::GlobalObjectID(const std::string& name) : _name(name)
	{
		auto it = __uuidMap.find(name);
		if (it == __uuidMap.end())
		{
			_id = UUID::newUUID();
			__uuidMap.emplace(name, _id);
		}
		else
		{
			_id = (*it).second;
		}
	}

	GlobalObjectID::~GlobalObjectID()
	{

	}

	const std::string& GlobalObjectID::getName() const
	{
		return _name;
	}

	const std::string& GlobalObjectID::getIdString() const
	{
		return _id.toString();
	}

	const UUID& GlobalObjectID::getId() const
	{
		return _id;
	}

	bool GlobalObjectID::isEmpty() const
	{
		return _id.isEmpty();
	}

	bool GlobalObjectID::operator==(const GlobalObjectID& other) const
	{
		return _id == other._id;
	}
}
