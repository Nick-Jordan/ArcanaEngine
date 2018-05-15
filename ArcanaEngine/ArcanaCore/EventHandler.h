#ifndef EVENT_HANDLER_H_
#define EVENT_HANDLER_H_

#include "ArcanaCoreAPIDefinition.h"

#include "Types.h"

#include <string>

namespace Arcana
{

	class ARCANA_CORE_API EventHandler
	{
	public:

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

		private:

			uint8 _code;
		};


		EventHandler();
		~EventHandler();

		BroadcastResult broadcast();
	};

}

#endif // !EVENT_HANDLER_H_

