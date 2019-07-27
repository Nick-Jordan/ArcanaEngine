// PCGTerrainClient.cpp : Defines the entry point for the application.
//
#include "stdafx.h"
#include "PCGTerrainClient.h"

#include "Application.h"

#include "WindowsWindowDefinition.h"
#include "WindowsApplicationDefinition.h"

#include "Renderer.h"
#include "EventListener.h"
#include "KeyEvent.h"
#include "Key.h"
#include "NoDataEvents.h"
#include "Globals.h"

#include "GeometryComponent.h"
#include "CameraComponent.h"
#include "ResourceManager.h"
#include "Input.h"
//#include "TerrainComponent.h"
//#include "AtmosphereComponent.h"
//#include "CloudsComponent.h"
#include "Profiler.h"

//vld
#include <vld.h>

//dependencies
#include "CoreModule.h"
#include "EngineModule.h"
#include "GraphicsModule.h"
#include "InputModule.h"
#include "IOModule.h"
#include "ArcanaLog.h"
#include "ArcanaMath.h"
#include "SceneModule.h"

using namespace Arcana;


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
		if (event.getInt("keyCode") == KeyCode::Escape || event.getInt("keyCode") == KeyCode::ControllerSpecialRight)
		{
			//handler.broadcast(WindowClosedEvent());
		}
		if (event.getInt("keyCode") == KeyCode::Add)
		{
			Actor::speed *= 1.2;
		}
		if (event.getInt("keyCode") == KeyCode::Subtract)
		{
			Actor::speed /= 1.2;
		}
		if (event.getInt("keyCode") == KeyCode::J)
		{
			//TerrainNode::updateQuad = false;// !TerrainNode::updateQuad;
		}
		if (event.getInt("keyCode") == KeyCode::K)
		{
			//TerrainNode::updateQuad = true;// !TerrainNode::updateQuad;
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

	InitEngine();

	WindowsWindowDefinition windowDef;
	windowDef.setWidth(1920);
	windowDef.setHeight(1080);
	windowDef.setStyle(Style::Default);

	WindowsApplicationDefinition appDefinition;
	appDefinition.setAppName("PCG Terrain Client");
	appDefinition.setWindowClass(L"PCG_TERRAIN_CLIENT");
	appDefinition.setInstance(hInstance);
	appDefinition.setCommandLineArgs(lpCmdLine);
	appDefinition.setShowCommand(nCmdShow);
	appDefinition.addWindowDefinition(windowDef);

	GEngine->createApplication(appDefinition);
	GEngine->getApplicationInstance()->getEventHandler().addEventListener(std::shared_ptr<MyListener>(new MyListener()));

	Input::setStaticRelativeWindow(GEngine->getApplicationInstance()->getActiveWindow());

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


	GEngine->getApplicationInstance()->getActiveWindow().setVerticalSync(false);

	World* world = new World("world");

	/*Terrain::Parameters params;
	params.maxLevel = 20;
	params.radius = 6361000.0;
	params.deformation = "sphere";
	params.splitFactor = 2.0;
	params.zmin = 0.0;
	params.zmax = 10000.0;

	Terrain::Parameters cloudsParams;
	cloudsParams.maxLevel = 0;
	cloudsParams.radius = 6367000.0;
	cloudsParams.deformation = "sphere";
	cloudsParams.splitFactor = 2.0;
	cloudsParams.zmin = 0.0;
	cloudsParams.zmax = 0.0;

	Actor* actor = world->createActor("actor", new Transform());

	TerrainComponent* front = new TerrainComponent(params, new Transform());
	actor->addComponent(front);

	Actor* camera = world->createActor("camera", new Transform(Vector3d(0.0, 0.0, 0.0), Vector3d::one(), Matrix4d::IDENTITY));
	CameraComponent* cameraComponent = new CameraComponent(45.0f, GEngine->getApplicationInstance()->getActiveWindow().getAspectRatio(), 0.000001, pow(10.0, 10.0));
	cameraComponent->setPosition(Vector3d(0.0, 0.0, 7481000 * 4));
	camera->addComponent(cameraComponent);

	InputComponent* input = new InputComponent();


	//Controller
	/*InputAxisKeyBinding bindingForwardController;
	bindingForwardController.axisKey = Keys::ControllerLeftAnalogY;
	bindingForwardController.axisCallback.bind(camera, &Actor::moveForward);
	input->addAxisKeyBinding(bindingForwardController);

	InputAxisKeyBinding bindingRightController;
	bindingRightController.axisKey = Keys::ControllerLeftAnalogX;
	bindingRightController.axisCallback.bind(camera, &Actor::moveRight);
	input->addAxisKeyBinding(bindingRightController);

	InputAxisBinding bindingUpController;
	bindingUpController.axis.addKeyMapping(Keys::ControllerRightTriggerAxis, 1.0);
	bindingUpController.axis.addKeyMapping(Keys::ControllerLeftTriggerAxis, -1.0);
	bindingUpController.axisCallback.bind(camera, &Actor::moveUp);
	input->addAxisBinding(bindingUpController);

	InputAxisKeyBinding bindingPitchController;
	bindingPitchController.axisKey = Keys::ControllerRightAnalogY;
	bindingPitchController.axisCallback.bind(camera, &Actor::pitch);
	input->addAxisKeyBinding(bindingPitchController);

	InputAxisKeyBinding bindingYawController;
	bindingYawController.axisKey = Keys::ControllerRightAnalogX;
	bindingYawController.axisCallback.bind(camera, &Actor::yaw);
	input->addAxisKeyBinding(bindingYawController);

	InputAxisBinding bindingRollController;
	bindingRollController.axis.addKeyMapping(Keys::ControllerLeftShoulder, 1.0);
	bindingRollController.axis.addKeyMapping(Keys::ControllerRightShoulder, -1.0);
	bindingRollController.axisCallback.bind(camera, &Actor::roll);
	input->addAxisBinding(bindingRollController);

	//Keyboard
	InputAxisBinding bindingForwardKeyboard;
	bindingForwardKeyboard.axis.addKeyMapping(Keys::W, 1.0);
	bindingForwardKeyboard.axis.addKeyMapping(Keys::S, -1.0);
	bindingForwardKeyboard.axisCallback.bind(camera, &Actor::moveForward);
	input->addAxisBinding(bindingForwardKeyboard);

	InputAxisBinding bindingRightKeyboard;
	bindingRightKeyboard.axis.addKeyMapping(Keys::D, 1.0);
	bindingRightKeyboard.axis.addKeyMapping(Keys::A, -1.0);
	bindingRightKeyboard.axisCallback.bind(camera, &Actor::moveRight);
	input->addAxisBinding(bindingRightKeyboard);

	InputAxisBinding bindingUpKeyboard;
	bindingUpKeyboard.axis.addKeyMapping(Keys::Space, 1.0);
	bindingUpKeyboard.axis.addKeyMapping(Keys::LeftControl, -1.0);
	bindingUpKeyboard.axisCallback.bind(camera, &Actor::moveUp);
	input->addAxisBinding(bindingUpKeyboard);

	InputAxisBinding bindingPitchKeyboard;
	bindingPitchKeyboard.axis.addKeyMapping(Keys::I, 1.0);
	bindingPitchKeyboard.axis.addKeyMapping(Keys::K, -1.0);
	bindingPitchKeyboard.axisCallback.bind(camera, &Actor::pitch);
	input->addAxisBinding(bindingPitchKeyboard);

	InputAxisBinding bindingYawKeyboard;
	bindingYawKeyboard.axis.addKeyMapping(Keys::L, 1.0);
	bindingYawKeyboard.axis.addKeyMapping(Keys::J, -1.0);
	bindingYawKeyboard.axisCallback.bind(camera, &Actor::yaw);
	input->addAxisBinding(bindingYawKeyboard);

	InputAxisBinding bindingRollKeyboard;
	bindingRollKeyboard.axis.addKeyMapping(Keys::Q, 1.0);
	bindingRollKeyboard.axis.addKeyMapping(Keys::E, -1.0);
	bindingRollKeyboard.axisCallback.bind(camera, &Actor::roll);
	input->addAxisBinding(bindingRollKeyboard);

	/*InputAxisKeyBinding bindingMousePitch;
	bindingMousePitch.axisKey = Keys::MouseY;
	bindingMousePitch.axisCallback.bind(camera, &Actor::mousePitch);
	input->addAxisKeyBinding(bindingMousePitch);

	InputAxisKeyBinding bindingMouseYaw;
	bindingMouseYaw.axisKey = Keys::MouseX;
	bindingMouseYaw.axisCallback.bind(camera, &Actor::mouseYaw);
	input->addAxisKeyBinding(bindingMouseYaw);

	camera->addComponent(input);


	GEngine->setWorld(world);*/


	GEngine->start();
	GEngine->exit();

	DestroyEngine();

	return 1;
}