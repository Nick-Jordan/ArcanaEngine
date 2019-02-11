// SponzaClient.cpp : Defines the entry point for the application.
//
#include "stdafx.h"
#include "SponzaClient.h"

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
#include "MeshRenderProcedure.h"
#include "MeshLoader.h"

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

class Atrium : public GeometryComponent
{
public:

	Atrium(std::string file, Material* material, std::string stage, bool castsShadow) : _file(file), _stage(stage), material(material)
	{ 
		initialize();
		_lightProperties.CastsDynamicShadow = castsShadow;
	}

	virtual ~Atrium() {}

	virtual void initialize() override
	{
		GeometryComponent::initialize();
	}

	virtual bool createRenderProcedure() override
	{
		renderState.setCullEnabled(true);
		renderState.setCullFaceSide(RenderState::Back);
		renderState.setDepthTestEnabled(true);
		renderState.setBlendEnabled(false);

		mesh = MeshLoader::instance().createMesh(_file);

		_renderProcedure = new MeshRenderProcedure(mesh, material, renderState, _stage);
		_renderProcedure->reference();

		_renderProcedure->createRenderData();

		return true;
	}

private:

	Mesh* mesh;
	Material* material;
	RenderState renderState;
	std::string _stage;
	std::string _file;
};

class CubeComponent : public GeometryComponent
{
public:

	CubeComponent(Material* material, std::string stage, bool castsShadow) : _stage(stage), material(material)
	{
		initialize();
		_lightProperties.CastsDynamicShadow = castsShadow;
	}

	virtual ~CubeComponent() {}

	virtual void initialize() override
	{
		GeometryComponent::initialize();
	}

	virtual bool createRenderProcedure() override
	{
		renderState.setCullEnabled(true);
		renderState.setCullFaceSide(RenderState::Back);
		renderState.setDepthTestEnabled(true);
		renderState.setBlendEnabled(false);

		VertexFormat::Attribute attribs[] =
		{
			VertexFormat::Attribute(VertexFormat::Semantic::Position, 3),
			VertexFormat::Attribute(VertexFormat::Semantic::Normal, 3),
			VertexFormat::Attribute(VertexFormat::Semantic::TexCoord0, 2),
		};
		VertexFormat format(3, attribs);
		mesh = new Mesh(format, Mesh::Triangles);

		float vertices[] = {
			// back face
			-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
			 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
			 1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
			 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
			-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
			-1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
			// front face
			-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
			 1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
			 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
			 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
			-1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
			-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
			// left face
			-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
			-1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
			-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
			-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
			-1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
			-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
			// right face
			 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
			 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
			 1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
			 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
			 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
			 1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
			// bottom face
			-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
			 1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
			 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
			 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
			-1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
			-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
			// top face
			-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
			 1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
			 1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
			 1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
			-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
			-1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
		};

		mesh->setVertexBuffer(format, 36)->setVertexData(vertices);

		_renderProcedure = new MeshRenderProcedure(mesh, material, renderState, _stage);
		_renderProcedure->reference();

		_renderProcedure->createRenderData();

		return true;
	}

private:

	Mesh* mesh;
	Material* material;
	RenderState renderState;
	std::string _stage;
};

Actor* lightBox;
Actor* directionalLightActor;

void moveLightY(float value)
{
	if (lightBox)
	{
		lightBox->getTransform().translateY(value * 0.001);
	}
}

void moveLightX(float value)
{
	if (lightBox)
	{
		lightBox->getTransform().translateX(value * 0.001);
	}
}

void moveLightZ(float value)
{
	if (lightBox)
	{
		lightBox->getTransform().translateZ(value * 0.001);
	}
}

void moveDirectionalLightZ(float value)
{
	if (directionalLightActor)
	{
		directionalLightActor->getTransform().translateZ(value * 0.0001);
	}
}

void moveDirectionalLightX(float value)
{
	if (directionalLightActor)
	{
		directionalLightActor->getTransform().translateX(value * 0.0001);
	}
}

void moveDirectionalLightY(float value)
{
	if (directionalLightActor)
	{
		directionalLightActor->getTransform().translateY(value * 0.0001);
	}
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
	appDefinition.setAppName("Model Client");
	appDefinition.setWindowClass(L"MODEL_CLIENT");
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


	ResourceManager::instance().initialize("resources/resource_database.xml");


	World* world = new World("world");

	Actor* atrium = world->createActor("atrium", new Transform(Vector3d(0.0,0.0, 0.0), Vector3d::one(), Matrix4d::IDENTITY));
	atrium->setMobility(Actor::Mobility::Dynamic);
	Material* atriumMaterial = ResourceManager::instance().loadResource<Material>("atrium");
	atrium->addComponent(new Atrium("resources/atrium/sponza.obj", atriumMaterial, "OpaqueObjectStage", false));

	directionalLightActor = world->createActor("directionalLightActor", new Transform(Vector3d(0.0, 10.0, 0.0), Vector3d::one(), Matrix4d::IDENTITY));
	directionalLightActor->setMobility(Actor::Mobility::Dynamic);
	DirectionalLightComponent* directionalLight = new DirectionalLightComponent();
	directionalLightActor->addComponent(directionalLight);

	Actor* camera = world->createActor("camera", new Transform(Vector3d(0.0, 0.0, 0.0), Vector3d::one(), Matrix4d::IDENTITY));
	CameraComponent* cameraComponent = new CameraComponent(90.0f, GEngine->getApplicationInstance()->getActiveWindow().getAspectRatio(), 0.1, 1000.0);
	cameraComponent->setPosition(Vector3d(0.0, 0.0, 2.0));
	camera->addComponent(cameraComponent);
	
	/*Actor* lights = world->createActor("lights", new Transform(Vector3d(0.0, 0.0, 0.0), Vector3d::one(), Matrix4d::IDENTITY));

	PointLightComponent* pointLight = new PointLightComponent();
	pointLight->setPosition(Vector3d(0.0, 4.0, 0.0));
	lights->addComponent(pointLight);
	PointLightComponent* pointLight1 = new PointLightComponent();
	pointLight1->setPosition(Vector3d(10.0, 10.0, 10.0));
	lights->addComponent(pointLight1);
	PointLightComponent* pointLight2 = new PointLightComponent();
	pointLight2->setPosition(Vector3d(-10.0, -10.0, 10.0));
	lights->addComponent(pointLight2);
	PointLightComponent* pointLight3 = new PointLightComponent();
	pointLight3->setPosition(Vector3d(10.0, -10.0, 10.0));
	lights->addComponent(pointLight3);*/

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

	InputAxisBinding bindingMoveLightZ;
	bindingMoveLightZ.axis.addKeyMapping(Keys::Home, 1.0);
	bindingMoveLightZ.axis.addKeyMapping(Keys::End, -1.0);
	bindingMoveLightZ.axisCallback.bind(&moveLightZ);
	input->addAxisBinding(bindingMoveLightZ);

	InputAxisBinding bindingMoveLightX;
	bindingMoveLightX.axis.addKeyMapping(Keys::PageDown, 1.0);
	bindingMoveLightX.axis.addKeyMapping(Keys::Delete, -1.0);
	bindingMoveLightX.axisCallback.bind(&moveLightX);
	input->addAxisBinding(bindingMoveLightX);

	InputAxisBinding bindingMoveLightY;
	bindingMoveLightY.axis.addKeyMapping(Keys::Insert, 1.0);
	bindingMoveLightY.axis.addKeyMapping(Keys::PageUp, -1.0);
	bindingMoveLightY.axisCallback.bind(&moveLightY);
	input->addAxisBinding(bindingMoveLightY);

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

	DestroyEngine();

	return 1;
}