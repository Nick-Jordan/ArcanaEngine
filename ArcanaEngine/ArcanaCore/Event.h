#ifndef EVENT_H_
#define EVENT_H_

#include "CoreDefines.h"

#include "Object.h"
#include "Types.h"
#include "SmartPtr.h"
#include "Callback.h"

#include "Array.h"

namespace Arcana
{
	/** \brief Event callback typedef.
	 *  If a function is bound by the event, it is called when the event is handled by the EventHandler.
	 */

	REGISTER_CALLBACK(EventCallback)

	/** \brief Base class for all event types.
	 *  
	 *  
	 */

	class ARCANA_CORE_API Event : public Object
	{
	public:

		/** \brief A struct that contains a name string and associated data.
		 */
	
		struct DataPoint
		{
			std::string _name;   ///< The name of the data point.

			union
			{
				double _double;  ///< A double value.
				float _float;    ///< A float value.
				int _int;        ///< An integer value.
				bool _bool;      ///< A boolean value.
			};
		};

		/** \brief A wrapper for an array of data points.
		 */

		class Data
		{
		public:

			/** \brief Data default constructor.
			 */

			Data();

			/** \brief Data copy constructor.
			 */

			Data(const Data& data);

			/** \brief Data copy constructor with move.
			 */

			Data(Data&& data);

			/** \brief Data destructor.
			 */

			~Data();

			/** \brief Adds a data point to the array with a double value.
			 */

			void Event::Data::addDouble(std::string name, double entry)
			{
				DataPoint point;
				point._double = entry;
				point._name = name;

				_values.add(point);
			};

			/** \brief Adds a data point to the array with a float value.
			 */

			void Event::Data::addFloat(std::string name, float entry)
			{
				DataPoint point;
				point._float = entry;
				point._name = name;

				_values.add(point);
			};

			/** \brief Adds a data point to the array with an integer value.
			 */

			void Event::Data::addInt(std::string name, int entry)
			{
				DataPoint point;
				point._int = entry;
				point._name = name;

				_values.add(point);
			};

			/** \brief Adds a data point to the array with a boolean value.
			 */

			void Event::Data::addBool(std::string name, bool entry)
			{
				DataPoint point;
				point._bool = entry;
				point._name = name;

				_values.add(point);
			};

			/** \brief DataPoint bracket operator.
			 *  Returns the data point associated with the provided string.
			 */

			DataPoint& operator[](std::string name);

			/** \brief Data assignment operator.
			 */

			Data& operator=(const Data& other)
			{
				_values = Array<DataPoint>(other._values);
				return *this;
			};

			/** \brief Data assignment operator with move.
			 */

			Data& operator=(Data&& other)
			{
				_values = Array<DataPoint>(other._values);
				other._values.empty();
				return *this;
			};

		private:

			Array<DataPoint> _values;  ///< Array of data points.
		};
	

		/** \brief Event default constructor.
		 */

		Event();

		/** \brief Event constructor taking the event id as an argument.
		 */
		
		Event(uint64 id);

		/** \brief Event copy constructor.
		 */
		
		Event(const Event& event);

		/** \brief Event copy constructor with move.
		 */

		Event(Event&& event);

		/** \brief Event virtual destructor.
		 */
		
		virtual ~Event();
		
		
		/** \brief Accessor for the event id.
		 */

		uint64 getEventId() const;

		/** \brief Returns a reference to the event's data.
		 */
		
		virtual Data& getData();

		/** \brief Accessor for the event's callback.
		 */

		EventCallback& getEventCallback();

		/** \brief Relational equivalence operator.
		 */
				
		bool operator ==(const Event& other);

		/** \brief Relational 'is not equal to' operator.
		 */
		
		bool operator !=(const Event& other);

		/** \brief Event assignment operator.
		 */

		Event& operator=(const Event& other);
		
		/** \brief Event assignment operator with move.
		 */

		Event& operator=(Event&& other);


	private:
	
		Data _data;				  ///< The object storing the event's data values.

		EventCallback _callback;  ///< The event callback.
	
		uint64 _id;				  ///< The event's id.
	};
}

#endif // !EVENT_H_