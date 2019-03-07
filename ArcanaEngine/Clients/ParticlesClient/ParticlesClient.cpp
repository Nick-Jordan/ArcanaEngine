// ParticlesClient.cpp : Defines the entry point for the application.
//
#include "stdafx.h"
#include "ParticlesClient.h"

#include "Application.h"

#include "WindowsWindowDefinition.h"
#include "WindowsApplicationDefinition.h"

#include "Renderer.h"
#include "EventListener.h"
#include "KeyEvent.h"
#include "Key.h"
#include "NoDataEvents.h"
#include "Globals.h"
#include "GlobalShaders.h"

#include "MeshComponent.h"
#include "CameraComponent.h"
#include "ResourceManager.h"
#include "Input.h"
#include "MeshRenderProcedure.h"
#include "MeshParticleEmitterComponent.h"
#include "DynamicField.h"

#include "PointLightComponent.h"
#include "DirectionalLightComponent.h"


//FasterThanLight
//#define BUILD_LIGHTING
//#include "FasterThanLight.h"

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
		static bool vsyncEnabled = true;

		if (event.getInt("keyCode") == KeyCode::V)
		{
			vsyncEnabled = !vsyncEnabled;
			//GEngine->getApplicationInstance()->getActiveWindow().setVerticalSync(vsyncEnabled);
		}
		if (event.getInt("keyCode") == KeyCode::Escape || event.getInt("keyCode") == KeyCode::ControllerSpecialRight)
		{
			handler.broadcast(WindowClosedEvent());
		}


		return true;
	}
};

Vector3d evalSin(double x, double y, double z)
{
	return Vector3d(sin(y), sin(x), cos(z));
}

Vector3d evalNeg(double x, double y, double z)
{
	return Vector3d(-x, -y, -z);
}

Vector3d evalZero(double x, double y, double z)
{
	return Vector3d::zero();
}

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
	appDefinition.setAppName("Particles Client");
	appDefinition.setWindowClass(L"PARTICLES_CLIENT");
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

	Actor* particles = world->createActor("particles", new Transform(Vector3d(0.0, 0.0, 0.0), Vector3d::one(), Matrix4d::IDENTITY));
	VertexFormat::Attribute attribs[] =
	{
		VertexFormat::Attribute(VertexFormat::Semantic::Position, 3),
	};
	VertexFormat format(1, attribs);
	Mesh* mesh = new Mesh(format, Mesh::Triangles);

	float vertices[] = {
		// back face
		-1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,        
		 1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		// front face
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,
		// left face
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		// right face
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,       
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,    
		// bottom face
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		// top face
		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f , 1.0f,
		 1.0f,  1.0f, -1.0f,   
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f       
	};

	mesh->setVertexBuffer(format, 36)->setVertexData(vertices);
	/*Mesh* mesh = new Mesh(format, Mesh::TriangleStrip);
	float vertices[] =
	{
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 0.0f, 0.0f
	};
	mesh->setVertexBuffer(format, 4)->setVertexData(vertices);*/
	ParticleEmitterProperties properties;
	properties.emissionRate = 100;
	properties.ellipsoid = false;
	properties.sizeStartMin = 0.2f;
	properties.sizeStartMax = 0.5f;
	properties.sizeEndMin = 0.1f;
	properties.sizeEndMax = 0.4f;
	properties.energyMin = 1000.0;
	properties.energyMax = 20000.0;
	properties.colorStart = Vector4f::one();
	properties.colorStartVar = Vector4f::zero();
	properties.colorEnd = Vector4f(1.0, 0.0, 0.0, 0.0);
	properties.colorEndVar = Vector4f::zero();
	properties.rotationPerParticleSpeedMin = 0.0f;
	properties.rotationPerParticleSpeedMax = 45.0f;
	properties.rotationAxisVar = Vector3f::zero();
	properties.rotationAxis = Vector3d::unitZ();
	properties.rotationSpeedMin = 0.0f;
	properties.rotationSpeedMax = 0.0f;
	properties.accelerationVar = Vector3f::zero();
	properties.velocityVar = Vector3f::zero();
	properties.positionVar = Vector3f::one()*25.0;
	properties.frameRandomOffset = 0;
	properties.orbitPosition = false;
	properties.orbitVelocity = false;
	properties.orbitAcceleration = false;
	properties.useVelocityField = true;

	MeshParticleEmitterComponent* particleEmitter = new MeshParticleEmitterComponent(mesh, 20000, properties, *GlobalShaders::get(GlobalShaders::MeshParticles));
	
	Image<uint8> image;
	image.init("resources/arcana/textures/particles/particle17.png");
	Texture* texture = Texture::create2D(Texture::RGBA, 64, 64, Texture::RGBA8, Texture::UnsignedByte, image.getPixelsPtr());
	particleEmitter->setTexture(texture);
	AE_RELEASE(texture);

	DynamicVectorField* field = new DynamicVectorField(evalSin);
	particleEmitter->setVelocityVectorField(field);
	particleEmitter->start();
	particles->addComponent(particleEmitter);

	Actor* camera = world->createActor("camera", new Transform(Vector3d(0.0, 0.0, 0.0), Vector3d::one(), Matrix4d::IDENTITY));
	CameraComponent* cameraComponent = new CameraComponent(90.0f, GEngine->getApplicationInstance()->getActiveWindow().getAspectRatio(), 0.1, 1000.0);
	cameraComponent->setPosition(Vector3d(0.0, 0.0, 2.0));
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
	input->addAxisBinding(bindingRollController);*/

	//Keyboard
	InputAxisBinding bindingForwardKeyboard;
	bindingForwardKeyboard.axis.addKeyMapping(Keys::W, 1.0);
	bindingForwardKeyboard.axis.addKeyMapping(Keys::S, -1.0);
	bindingForwardKeyboard.axis.addKeyMapping(Keys::Up, 1.0);
	bindingForwardKeyboard.axis.addKeyMapping(Keys::Down, -1.0);
	bindingForwardKeyboard.axis.addKeyMapping(Keys::ControllerLeftAnalogY);
	bindingForwardKeyboard.axisCallback.bind(camera, &Actor::moveForward);
	input->addAxisBinding(bindingForwardKeyboard);

	InputAxisBinding bindingRightKeyboard;
	bindingRightKeyboard.axis.addKeyMapping(Keys::D, 1.0);
	bindingRightKeyboard.axis.addKeyMapping(Keys::A, -1.0);
	bindingRightKeyboard.axis.addKeyMapping(Keys::Right, 1.0);
	bindingRightKeyboard.axis.addKeyMapping(Keys::Left, -1.0);
	bindingRightKeyboard.axis.addKeyMapping(Keys::ControllerLeftAnalogX);
	bindingRightKeyboard.axisCallback.bind(camera, &Actor::moveRight);
	input->addAxisBinding(bindingRightKeyboard);

	InputAxisBinding bindingUpKeyboard;
	bindingUpKeyboard.axis.addKeyMapping(Keys::Space, 1.0);
	bindingUpKeyboard.axis.addKeyMapping(Keys::LeftControl, -1.0);
	bindingUpKeyboard.axis.addKeyMapping(Keys::ControllerLeftTriggerAxis, -1.0);
	bindingUpKeyboard.axis.addKeyMapping(Keys::ControllerRightTriggerAxis, 1.0);
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

	/*InputAxisBinding bindingMoveDirectionalLightZ;
	bindingMoveDirectionalLightZ.axis.addKeyMapping(Keys::Home, 1.0);
	bindingMoveDirectionalLightZ.axis.addKeyMapping(Keys::End, -1.0);
	bindingMoveDirectionalLightZ.axisCallback.bind(&moveDirectionalLightZ);
	input->addAxisBinding(bindingMoveDirectionalLightZ);

	InputAxisBinding bindingMoveDirectionalLightX;
	bindingMoveDirectionalLightX.axis.addKeyMapping(Keys::PageDown, 1.0);
	bindingMoveDirectionalLightX.axis.addKeyMapping(Keys::Delete, -1.0);
	bindingMoveDirectionalLightX.axisCallback.bind(&moveDirectionalLightX);
	input->addAxisBinding(bindingMoveDirectionalLightX);

	InputAxisBinding bindingMoveDirectionalLightY;
	bindingMoveDirectionalLightY.axis.addKeyMapping(Keys::Insert, 1.0);
	bindingMoveDirectionalLightY.axis.addKeyMapping(Keys::PageUp, -1.0);
	bindingMoveDirectionalLightY.axisCallback.bind(&moveDirectionalLightY);
	input->addAxisBinding(bindingMoveDirectionalLightY);*/

	/*InputAxisKeyBinding bindingMousePitch;
	bindingMousePitch.axisKey = Keys::MouseY;
	bindingMousePitch.axisCallback.bind(camera, &Actor::mousePitch);
	input->addAxisKeyBinding(bindingMousePitch);

	InputAxisKeyBinding bindingMouseYaw;
	bindingMouseYaw.axisKey = Keys::MouseX;
	bindingMouseYaw.axisCallback.bind(camera, &Actor::mouseYaw);
	input->addAxisKeyBinding(bindingMouseYaw);*/

	camera->addComponent(input);

	GEngine->setWorld(world);

	//FASTER_THAN_LIGHT(world);

	GEngine->start();
	GEngine->exit();

	AE_DELETE(field);

	DestroyEngine();

	return 1;
}