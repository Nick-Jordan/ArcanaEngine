#include "Event.h"
#include "ArcanaLog.h"

namespace Arcana
{
	Event::Event() : Object("Event"), _id(EventID::DefaultID), _data(Data())
	{
	}
		
	Event::Event(uint64 id) : Object("Event"), _id(id), _data(Data())
	{
	}
		
	Event::Event(const Event& event) : Object("Event"), _id(event._id), _data(event._data)
	{
	}

	Event::Event(Event&& event) : Object("Event"), _id(event._id), _data(event._data)
	{
		event._id = EventID::DefaultID;
		event._data = Data();

		LOG(Error, CoreEngine, "Event copy constructor");
	}
		
	Event::~Event()
	{
		LOG(Error, CoreEngine, "Event Destructed");
	}
		
		
	uint64 Event::getEventId() const
	{
		return _id;
	}
		
	Event::Data& Event::getData()
	{
		return _data;
	}

	EventCallback& Event::getEventCallback()
	{
		return _callback;
	}

	double Event::getDouble(const std::string& name)
	{
		return getData()[name]._double;
	}

	float Event::getFloat(const std::string& name)
	{
		return getData()[name]._float;
	}

	int Event::getInt(const std::string& name)
	{
		return getData()[name]._int;
	}

	bool Event::getBool(const std::string& name)
	{
		return getData()[name]._bool;
	}
		
	bool Event::operator ==(const Event& other)
	{
		return _id == other._id;
	}

	bool Event::operator ==(uint64 id)
	{
		return _id == id;
	}
	
	bool Event::operator !=(const Event& other)
	{
		return _id != other._id;
	}

	Event& Event::operator=(const Event& other)
	{
		_data = other._data;
		_id = other._id;

		return *this;
	}

	Event& Event::operator=(Event&& other)
	{
		_data = other._data;
		_id = other._id;

		return *this;
	}


	//Data

	Event::Data::Data()
	{

	}

	Event::Data::Data(const Data& data) : _values(data._values)
	{

	}

	Event::Data::Data(Data&& data) : _values(data._values)
	{
		LOG(Error, CoreEngine, "Data copy constructor");
	}

	Event::Data::~Data()
	{
		LOG(Error, CoreEngine, "Data destructor");
	}

	Event::DataPoint& Event::Data::operator[](std::string name)
	{
		DataPoint* dataPoint = _values.findByPredicate([=](DataPoint point) {return point._name == name; });

		if (dataPoint != nullptr)
			return *dataPoint;

		return DataPoint();
	}
}