// WindowsPlatformClient.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "WindowsPlatformClient.h"

#include "Application.h"

#include "WindowsWindowDefinition.h"
#include "WindowsApplicationDefinition.h"

#include "NoDataEvents.h"

#include "Key.h"
#include "Controller.h"

//vld
#include <vld.h>

//dependencies
#include "CoreModule.h"
#include "InputModule.h"
#include "ArcanaLog.h"
#include "ArcanaMath.h"

using namespace Arcana;

class TestListener : public EventListener
{
public:

	TestListener()
	{
		listenForEvent(EventID::KeyEventID);
	}
	~TestListener()
	{

	}

	virtual bool processEvent(Event& event, EventHandler& handler) override
	{

		if (event.getInt("event") < 2)
		{

			LOGF(Info, CoreEngine, "Key '%s' %s...", Keys::get(event.getInt("keyCode")).getGlobalObjectID().getName().c_str(), event.getInt("event") == 0 ? "pressed" : "released");

			if (event.getInt("keyCode") == KeyCode::ControllerSpecialRight || (event.getBool("shift") && event.getInt("keyCode") == KeyCode::Escape))
			{
				return handler.broadcast(WindowClosedEvent()).isSuccess();
			}
		}
		else
		{
			LOGF(Info, CoreEngine, "Float Axis Key '%s' from controller %d at %f...", Keys::get(event.getInt("keyCode")).getGlobalObjectID().getName().c_str(), event.getInt("controller"), event.getFloat("axis"));
		}

		return true;
	}
};


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

	/*XBOXController controller;
	LOG(Info, CoreEngine, "Controller connected: " + std::to_string(controller.isConnected()));

	while (true)
	{
		if (controller.isConnected())
		{
			if (controller.getState().Gamepad.wButtons & XINPUT_GAMEPAD_A)
			{
				controller.vibrate(10000, 10000);
			}
			if (controller.getState().Gamepad.wButtons & XINPUT_GAMEPAD_B)
			{
				controller.vibrate();
				break;
			}
		}
		else
		{
			break;
		}
	}*/




	LOG(Debug, CoreEngine, "Instance: " + std::to_string((int)hInstance));

	WindowsWindowDefinition windowDef;
	windowDef.setWidth(800);
	windowDef.setHeight(600);
	windowDef.setStyle(Style::Default);

	WindowsApplicationDefinition appDefinition;
	appDefinition.setAppName("Windows Platform Client");
	appDefinition.setWindowClass(L"WINDOWS_PLATFORM_CLIENT");
	appDefinition.setInstance(hInstance);
	appDefinition.setCommandLineArgs(lpCmdLine);
	appDefinition.setShowCommand(nCmdShow);
	appDefinition.addWindowDefinition(windowDef);

	Application app = Application(appDefinition);

	app.getEventHandler().addEventListener(std::shared_ptr<EventListener>(new TestListener()));

	app.start();

	return 1;
}