// GraphicsClient.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "GraphicsClient.h"

#include "Application.h"

#include "WindowsWindowDefinition.h"
#include "WindowsApplicationDefinition.h"

#include "Renderer.h"
#include "EventListener.h"
#include "KeyEvent.h"
#include "Key.h"
#include "NoDataEvents.h"
#include "Globals.h"

#include <vld.h>

using namespace Arcana;


Application* app;

class MyListener : public EventListener
{
public:

	MyListener()
	{
		listenForEvent(EventID::KeyEventID);
	}

	~MyListener()
	{

	}

	virtual bool processEvent(Event& event, EventHandler& handler) override
	{
		static bool vsyncEnabled = true;

		if (event.getInt("keyCode") == KeyCode::V)
		{
			vsyncEnabled = !vsyncEnabled;
			app->getActiveWindow().setVerticalSync(vsyncEnabled);
		}
		if (event.getInt("keyCode") == KeyCode::Escape)
		{
			handler.broadcast(WindowClosedEvent());
		}


		return true;
	}
};

class TestShape : public Shape
{
public:

	TestShape(Mesh* mesh) : Shape(mesh)
	{

	}

	virtual ~TestShape() {};
};


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	InitEngine();

	WindowsWindowDefinition windowDef;
	windowDef.setWidth(800);
	windowDef.setHeight(600);
	windowDef.setStyle(Style::Default);

	WindowsApplicationDefinition appDefinition;
	appDefinition.setAppName("Graphics Client");
	appDefinition.setWindowClass(L"GRAPHICS_CLIENT");
	appDefinition.setInstance(hInstance);
	appDefinition.setCommandLineArgs(lpCmdLine);
	appDefinition.setShowCommand(nCmdShow);
	appDefinition.addWindowDefinition(windowDef);

	GEngine->createApplication(appDefinition);
	GEngine->getApplicationInstance()->getEventHandler().addEventListener(std::shared_ptr<MyListener>(new MyListener()));

	RenderSettings settings;
	settings.bitsPerPixel = 32;
	settings.depthBits = 8;
	settings.stencilBits = 8;
	settings.antialiasingLevel = 4;
	settings.majorVersion = 4;
	settings.minorVersion = 5;
	settings.attributeFlags = RenderSettings::Default;
	settings.sRgb = false;

	GEngine->setRenderer(settings);

	World* world = new World("world");
	Actor* actor = new Actor();
	VertexFormat::Attribute attribs[] =
	{
		VertexFormat::Attribute(VertexFormat::Semantic::Position, 3),
		VertexFormat::Attribute(VertexFormat::Semantic::Color, 4)
	};
	VertexFormat format(2, attribs);
	Mesh* mesh = new Mesh(format, Mesh::Triangles);
	VertexBuffer* buffer = new VertexBuffer(format, 3);
	float vertices[] = {
		-1.0f, -1.0f, 0.0f,  1.0f, 0.0f, 0.0f, 1.0f, 
		0.0f, 1.0f, 0.0f,  0.0f, 0.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 0.0f,   0.0f, 1.0f, 0.0f, 1.0f
	};
	buffer->setVertexData(vertices);
	mesh->setVertexBuffer(buffer);
	actor->setShape(new TestShape(mesh));
	world->addActor(actor);

	GEngine->setWorld(world);

	GEngine->start();
	GEngine->exit();

	DestroyEngine();

	AE_DELETE(app);

	return 1;
}