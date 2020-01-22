#include "UUID.h"

namespace Arcana
{
	UUID UUID::newUUID()
	{
		UUID uuid;
		uuid._id = uuids::uuid_system_generator{}();
		return uuid;
	}

	UUID UUID::Empty = UUID();

	UUID::UUID() : _id()
	{

	}

	UUID::UUID(const UUID& uuid) : _id(uuid._id)
	{

	}

	UUID::~UUID()
	{

	}

	bool UUID::isEmpty() const
	{
		return _id.is_nil();
	}

	std::string UUID::toString() const
	{
		return uuids::to_string(_id);
	}

	UUID& UUID::operator=(const UUID& rhs)
	{
		_id = rhs._id;
		return *this;
	}

	bool UUID::operator==(const UUID& rhs) const
	{
		return _id == rhs._id;
	}

	bool UUID::operator!=(const UUID& rhs) const
	{
		return _id != rhs._id;
	}

	bool UUID::operator<(const UUID& rhs) const
	{
		return _id < rhs._id;
	}
}