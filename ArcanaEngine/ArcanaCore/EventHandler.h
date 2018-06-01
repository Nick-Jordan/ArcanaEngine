#ifndef EVENT_HANDLER_H_
#define EVENT_HANDLER_H_

#include "CoreDefines.h"

#include "Event.h"

#include "SmartPtr.h"
#include "EventListener.h"
#include "Object.h"
#include "Types.h"
#include "Array.h"

#include <string>

namespace Arcana
{
	
	class ARCANA_CORE_API EventHandler : public Object
	{
	public:
	
		typedef Array<EventListener*> ListenerArray;

		enum BroadcastResultCode : uint8
		{
			EVENT_SUCCESS,
			EVENT_ERROR
		};

		class BroadcastResult
		{
		public:
			
			BroadcastResult(uint8 code) : _code(code) {};
			BroadcastResult(BroadcastResultCode code) : _code(code) {};

			uint8 getCode() const { return _code; };

			std::string getCodeString() const
			{
				std::string codeStrings[2] = { "Success", "Error" };
				return _code < 2 ? codeStrings[_code] : "";
			};

			bool isSuccess() const { return _code == EVENT_SUCCESS; };
			
			operator bool() const { return isSuccess(); };

		private:

			uint8 _code;
		};


		EventHandler();
		
		~EventHandler();

		BroadcastResult broadcast(Event& event);
		
		void addEventListener(EventListener* ptr);
		
		int32 removeEventListener(EventListener* ptr);
		
	private:
	
		ListenerArray _listeners;
	};

}

#endif // !EVENT_HANDLER_H_

