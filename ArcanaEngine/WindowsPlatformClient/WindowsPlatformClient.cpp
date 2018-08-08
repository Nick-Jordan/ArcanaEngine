// WindowsPlatformClient.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "WindowsPlatformClient.h"

#include "Application.h"

#include "WindowsWindowDefinition.h"
#include "WindowsApplicationDefinition.h"

#include "NoDataEvents.h"

#include "Key.h"
#include "XBOXController.h"

#include <vld.h>

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
		LOGF(Info, CoreEngine, "Key '%s' handled...", Keys::get(event.getInt("keyCode")).getGlobalObjectID().getName().c_str());

		if (event.getBool("shift") && event.getInt("keyCode") == KeyCode::Escape)
		{
			return handler.broadcast(WindowClosedEvent()).isSuccess();
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

	XBOXController controller;

	bool test = false;

	while (true)
	{
		if (controller.isConnected())
		{
			if (controller.getState().Gamepad.wButtons & XINPUT_GAMEPAD_A)
			{
				controller.vibrate(65535, 0);
			}

			if (controller.getState().Gamepad.wButtons & XINPUT_GAMEPAD_B)
			{
				controller.vibrate(0, 65535);
			}

			if (controller.getState().Gamepad.wButtons & XINPUT_GAMEPAD_X)
			{
				controller.vibrate(65535, 65535);
			}

			if (controller.getState().Gamepad.wButtons & XINPUT_GAMEPAD_Y)
			{
				controller.vibrate();
			}

			if (controller.getState().Gamepad.wButtons & XINPUT_GAMEPAD_BACK)
			{
				break;
			}

			if (controller.getState().Gamepad.wButtons & XINPUT_GAMEPAD_START)
			{
				test = !test;
			}

			if (test)
			{
				controller.vibrate(controller.getState().Gamepad.bLeftTrigger * 257, controller.getState().Gamepad.bRightTrigger * 257);
			}
		}
		else
		{
			break;
		}
	}

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