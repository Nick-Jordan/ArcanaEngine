#ifndef WINDOWS_CONTROLLER_CONTEXT_H_
#define WINDOWS_CONTROLLER_CONTEXT_H_

#include "InputDefines.h"

#include "Controller.h"
#include "ControllerState.h"

#include <Windows.h>
#include <Xinput.h>

#pragma comment(lib, "XInput.lib")

namespace Arcana
{

	class ARCANA_INPUT_API ControllerContext
	{
	public:


		static void initialize();

		static void cleanup();

		static bool isConnected(uint32 index);

		static void setLazyUpdates(bool status);

		static void updateConnections();

		bool open(uint32 index);

		void close();

		Controller::Specification getSpecification() const;

		ControllerState update();

	private:

		static int32 keyToButtonConversion(int32 key);


		uint32 _index;
		Controller::Specification _specification;

		static bool lazyUpdates;
	};

}

#endif // !WINDOWS_CONTROLLER_CONTEXT_H_

