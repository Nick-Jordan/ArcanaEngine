#ifndef EVENT_H_
#define EVENT_H_

#include "ArcanaCoreAPIDefinition.h"

#include "Object.h"
#include "Types.h"
#include "SmartPtr.h"

#include "Array.h"

namespace Arcana
{
	class ARCANA_CORE_API Event : public Object
	{
	public:
	
		struct DataPoint
		{
			std::string _name;

			union
			{
				double _double;
				float _float;
				int _int;
				bool _bool;
			};
		};

		class Data
		{
		public:

			Data();
			Data(const Data& data);
			Data(Data&& data);
			~Data();

			void Event::Data::addDouble(std::string name, double entry)
			{
				DataPoint point;
				point._double = entry;
				point._name = name;

				_values.add(point);
			};
			void Event::Data::addFloat(std::string name, float entry)
			{
				DataPoint point;
				point._float = entry;
				point._name = name;

				_values.add(point);
			};
			void Event::Data::addInt(std::string name, int entry)
			{
				DataPoint point;
				point._int = entry;
				point._name = name;

				_values.add(point);
			};
			void Event::Data::addBool(std::string name, bool entry)
			{
				DataPoint point;
				point._bool = entry;
				point._name = name;

				_values.add(point);
			};

			DataPoint& operator[](std::string name);
			Data& operator=(const Data& other)
			{
				_values = Array<DataPoint>(other._values);
				return *this;
			};
			Data& operator=(Data&& other)
			{
				_values = Array<DataPoint>(other._values);
				other._values.empty();
				return *this;
			};

		private:

			Array<DataPoint> _values;
		};
	
		Event();
		
		Event(uint64 id);
		
		Event(const Event& event);

		Event(Event&& event);
		
		virtual ~Event();
		
		
		uint64 getEventId() const;
		
		virtual Data& getData();
				
		bool operator ==(const Event& other);
		
		bool operator !=(const Event& other);

		Event& operator=(const Event& other);
		
		Event& operator=(Event&& other);


	private:
	
		Data _data;
	
		uint64 _id;
	};
}

#endif // !EVENT_H_