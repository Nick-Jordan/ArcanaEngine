#ifndef EVENT_H_
#define EVENT_H_

#include "ArcanaCoreAPIDefinition.h"

#include "Object.h"
#include "Types.h"
#include "SmartPtr.h"

namespace Arcana
{
	class ARCANA_CORE_API Event : public Object
	{
	public:
	
		struct Data
		{
			virtual ~Data() {};
		};
	
		Event();
		
		Event(uint64 id);
		
		Event(const Event& event);
		
		virtual ~Event();
		
		
		uint64 getEventId() const;
		
		template<typename DataType>
		DataType getData()
		{
			if (_data)
			{
				DataType* data = dynamic_cast<DataType*>(_data);

				if (data)
				{
					return *data;
				}
			}
			
			return DataType();
		};
		
		void setData(Data* data) { _data = data; };
		
		bool operator ==(const Event& other);
		
		bool operator !=(const Event& other);
		
		
	private:
	
		Data* _data;
	
		uint64 _id;
	};
}

#endif // !EVENT_H_