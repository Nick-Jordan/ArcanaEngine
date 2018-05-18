#include "Event.h"
#include "ArcanaLog.h"

namespace Arcana
{
	Event::Event() : Object("Event"), _id(0), _data(nullptr)
	{
	}
		
	Event::Event(uint64 id) : Object("Event"), _id(id), _data(nullptr)
	{
	}
		
	Event::Event(const Event& event) : Object("Event"), _id(event._id), _data(event._data)
	{
	}
		
	Event::~Event()
	{
		//delete _data;
	}
		
		
	uint64 Event::getEventId() const
	{
		return _id;
	}
		
		
	bool Event::operator ==(const Event& other)
	{
		return _id == other._id;
	}
	
	bool Event::operator !=(const Event& other)
	{
		return _id != other._id;
	}
}