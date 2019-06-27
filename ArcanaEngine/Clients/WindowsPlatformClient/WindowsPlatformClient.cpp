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
#include "KeyEvent.h"
#include "Sleep.h"

//vld
#include <vld.h>

//dependencies
#include "CoreModule.h"
#include "InputModule.h"
#include "ArcanaLog.h"
#include "ArcanaMath.h"

using namespace Arcana;

Application* toggleFullscreenApplication = nullptr;

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

		if (event.getInt("keyCode") == KeyCode::Escape && event.getInt("event") == KeyEvent::Pressed)
		{
			handler.broadcast(WindowClosedEvent());
		}
		else if (event.getInt("keyCode") == KeyCode::F && event.getInt("event") == KeyEvent::Pressed)
		{
			if (toggleFullscreenApplication)
			{
				toggleFullscreenApplication->getActiveWindow().toggleFullscreen();
			}
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

	/* Style None */

	WindowsWindowDefinition windowDefStyleNone;
	windowDefStyleNone.setWidth(1280);
	windowDefStyleNone.setHeight(720);
	windowDefStyleNone.setStyle(Style::None);

	WindowsApplicationDefinition appDefinitionStyleNone;
	appDefinitionStyleNone.setAppName("Style None");
	appDefinitionStyleNone.setWindowClass(L"WINDOWS_PLATFORM_CLIENT");
	appDefinitionStyleNone.setInstance(hInstance);
	appDefinitionStyleNone.setCommandLineArgs(lpCmdLine);
	appDefinitionStyleNone.setShowCommand(nCmdShow);
	appDefinitionStyleNone.addWindowDefinition(windowDefStyleNone);

	Application appStyleNone(appDefinitionStyleNone);
	appStyleNone.getEventHandler().addEventListener(std::shared_ptr<EventListener>(new TestListener()));
	appStyleNone.start();


	/* Style Titlebar */

	WindowsWindowDefinition windowDefStyleTitlebar;
	windowDefStyleTitlebar.setWidth(1280);
	windowDefStyleTitlebar.setHeight(720);
	windowDefStyleTitlebar.setStyle(Style::Titlebar);

	WindowsApplicationDefinition appDefinitionStyleTitlebar;
	appDefinitionStyleTitlebar.setAppName("Style Titlebar");
	appDefinitionStyleTitlebar.setWindowClass(L"WINDOWS_PLATFORM_CLIENT");
	appDefinitionStyleTitlebar.setInstance(hInstance);
	appDefinitionStyleTitlebar.setCommandLineArgs(lpCmdLine);
	appDefinitionStyleTitlebar.setShowCommand(nCmdShow);
	appDefinitionStyleTitlebar.addWindowDefinition(windowDefStyleTitlebar);

	Application appStyleTitlebar(appDefinitionStyleTitlebar);
	appStyleTitlebar.getEventHandler().addEventListener(std::shared_ptr<EventListener>(new TestListener()));
	appStyleTitlebar.start();


	/* Style Titlebar|Close */

	WindowsWindowDefinition windowDefStyleTitlebarClose;
	windowDefStyleTitlebarClose.setWidth(1280);
	windowDefStyleTitlebarClose.setHeight(720);
	windowDefStyleTitlebarClose.setStyle(Style::Titlebar | Style::Close);

	WindowsApplicationDefinition appDefinitionStyleTitlebarClose;
	appDefinitionStyleTitlebarClose.setAppName("Style Titlebar|Close");
	appDefinitionStyleTitlebarClose.setWindowClass(L"WINDOWS_PLATFORM_CLIENT");
	appDefinitionStyleTitlebarClose.setInstance(hInstance);
	appDefinitionStyleTitlebarClose.setCommandLineArgs(lpCmdLine);
	appDefinitionStyleTitlebarClose.setShowCommand(nCmdShow);
	appDefinitionStyleTitlebarClose.addWindowDefinition(windowDefStyleTitlebarClose);

	Application appStyleTitlebarClose(appDefinitionStyleTitlebarClose);
	appStyleTitlebarClose.getEventHandler().addEventListener(std::shared_ptr<EventListener>(new TestListener()));
	appStyleTitlebarClose.start();

	
	/* Style Default */

	WindowsWindowDefinition windowDefStyleDefault;
	windowDefStyleDefault.setWidth(1280);
	windowDefStyleDefault.setHeight(720);
	windowDefStyleDefault.setStyle(Style::Default);

	WindowsApplicationDefinition appDefinitionStyleDefault;
	appDefinitionStyleDefault.setAppName("Style Default");
	appDefinitionStyleDefault.setWindowClass(L"WINDOWS_PLATFORM_CLIENT");
	appDefinitionStyleDefault.setInstance(hInstance);
	appDefinitionStyleDefault.setCommandLineArgs(lpCmdLine);
	appDefinitionStyleDefault.setShowCommand(nCmdShow);
	appDefinitionStyleDefault.addWindowDefinition(windowDefStyleDefault);

	Application appStyleDefault(appDefinitionStyleDefault);
	appStyleDefault.getEventHandler().addEventListener(std::shared_ptr<EventListener>(new TestListener()));
	appStyleDefault.start();


	/* Style Fullscreen */

	WindowsWindowDefinition windowDefStyleFullscreen;
	windowDefStyleFullscreen.setWidth(1280);
	windowDefStyleFullscreen.setHeight(720);
	windowDefStyleFullscreen.setStyle(Style::Fullscreen);

	WindowsApplicationDefinition appDefinitionStyleFullscreen;
	appDefinitionStyleFullscreen.setAppName("Style Fullscreen");
	appDefinitionStyleFullscreen.setWindowClass(L"WINDOWS_PLATFORM_CLIENT");
	appDefinitionStyleFullscreen.setInstance(hInstance);
	appDefinitionStyleFullscreen.setCommandLineArgs(lpCmdLine);
	appDefinitionStyleFullscreen.setShowCommand(nCmdShow);
	appDefinitionStyleFullscreen.addWindowDefinition(windowDefStyleFullscreen);

	Application appStyleFullscreen(appDefinitionStyleFullscreen);
	appStyleFullscreen.getEventHandler().addEventListener(std::shared_ptr<EventListener>(new TestListener()));
	appStyleFullscreen.start();


	/* Style Fullscreen Toggle*/

	WindowsWindowDefinition windowDefStyleFullscreenToggle;
	windowDefStyleFullscreenToggle.setWidth(1280);
	windowDefStyleFullscreenToggle.setHeight(720);
	windowDefStyleFullscreenToggle.setStyle(Style::Default);

	WindowsApplicationDefinition appDefinitionStyleFullscreenToggle;
	appDefinitionStyleFullscreenToggle.setAppName("Style Fullscreen Toggle");
	appDefinitionStyleFullscreenToggle.setWindowClass(L"WINDOWS_PLATFORM_CLIENT");
	appDefinitionStyleFullscreenToggle.setInstance(hInstance);
	appDefinitionStyleFullscreenToggle.setCommandLineArgs(lpCmdLine);
	appDefinitionStyleFullscreenToggle.setShowCommand(nCmdShow);
	appDefinitionStyleFullscreenToggle.addWindowDefinition(windowDefStyleFullscreenToggle);

	Application appStyleFullscreenToggle(appDefinitionStyleFullscreenToggle);
	toggleFullscreenApplication = &appStyleFullscreenToggle;
	appStyleFullscreenToggle.getEventHandler().addEventListener(std::shared_ptr<EventListener>(new TestListener()));
	appStyleFullscreenToggle.start();

	return 1;
}