// EngineClient.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "EngineClient.h"

#include "Globals.h"
#include "WindowsWindowDefinition.h"
#include "WindowsApplicationDefinition.h"
#include "Timer.h"

//test
#include "Renderer.h"
#include "NoDataEvents.h"

#include <vld.h>

using namespace Arcana;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	Timer timer;

	InitEngine();

	WindowsWindowDefinition windowDef;
	windowDef.setWidth(800);
	windowDef.setHeight(600);
	windowDef.setStyle(Style::Default);

	WindowsApplicationDefinition appDefinition;
	appDefinition.setAppName("Engine Client");
	appDefinition.setWindowClass(L"ENGINE_CLIENT");
	appDefinition.setInstance(hInstance);
	appDefinition.setCommandLineArgs(lpCmdLine);
	appDefinition.setShowCommand(nCmdShow);
	appDefinition.addWindowDefinition(windowDef);

	//EngineParameters params;
	//set params

	GEngine->createApplication(appDefinition);

	//test
	RenderSettings settings;
	settings.bitsPerPixel = 32;
	settings.depthBits = 8;
	settings.stencilBits = 8;
	settings.antialiasingLevel = 4;
	settings.majorVersion = 4;
	settings.minorVersion = 5;
	settings.attributeFlags = RenderSettings::Default;
	settings.sRgb = false;
	Renderer renderer(settings, &GEngine->getApplicationInstance()->getActiveWindow());
	GEngine->getTimeline().addEvent(5.0, WindowClosedEvent());
	
	GEngine->start();
	GEngine->exit();

	DestroyEngine();

	LOGF(Error, CoreEngine, "Time: %f", timer.getElapsedTime().toSeconds());

	return 1;
}