#include "GlobalObjectID.h"

namespace Arcana
{

	GlobalObjectID::GlobalObjectID(const std::string& name) : _name(name)
	{
		_id = hashString(name);
	}

	GlobalObjectID::GlobalObjectID(const std::string& name, int64 id) : _name(name), _id(id)
	{

	}

	GlobalObjectID::~GlobalObjectID()
	{

	}

	int64 GlobalObjectID::hashString(const std::string& string)
	{
		int64 h = 0;

		if (string.size() > 0)
		{
			for (unsigned int i = 0; i < string.size(); i++)
			{
				h = (h * 54059) ^ (string[i] * 76963);
			}
		}

		return h;
	}


	const std::string& GlobalObjectID::getName() const
	{
		return _name;
	}

	int64 GlobalObjectID::getId() const
	{
		return _id;
	}

	bool GlobalObjectID::operator=(const GlobalObjectID& other)
	{
		return _id = other._id;
	}
}
