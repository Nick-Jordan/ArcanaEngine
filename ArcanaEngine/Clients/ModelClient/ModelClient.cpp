// ModelClient.cpp : Defines the entry point for the application.
//
#include "stdafx.h"
#include "ModelClient.h"

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

class Nanosuit : public GeometryComponent
{
public:

	Nanosuit(std::string file, Material* material, std::string stage, bool castsShadow) : _file(file), _stage(stage), material(material)
	{ 
		initialize();
		_lightProperties.CastsDynamicShadow = castsShadow;
	}

	virtual ~Nanosuit() {}

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

void createCornellBox(World* world);
Material* createNanosuitMaterial();

Actor* lightBox;
Actor* directionalLightActor;

void moveLightY(float value)
{
	if (lightBox)
	{
		lightBox->getLocalTransform().translateY(value * 0.001);
	}
}

void moveLightX(float value)
{
	if (lightBox)
	{
		lightBox->getLocalTransform().translateX(value * 0.001);
	}
}

void moveLightZ(float value)
{
	if (lightBox)
	{
		lightBox->getLocalTransform().translateZ(value * 0.001);
	}
}

void moveDirectionalLightZ(float value)
{
	if (directionalLightActor)
	{
		directionalLightActor->getLocalTransform().translateZ(value * 0.0001);
	}
}

void moveDirectionalLightX(float value)
{
	if (directionalLightActor)
	{
		directionalLightActor->getLocalTransform().translateX(value * 0.0001);
	}
}

void moveDirectionalLightY(float value)
{
	if (directionalLightActor)
	{
		directionalLightActor->getLocalTransform().translateY(value * 0.0001);
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

	createCornellBox(world);

	Actor* nanosuit = world->createActor("nanosuit", new Transform(Vector3d(0.0,-5.0, 0.0), Vector3d(0.5, 0.5, 0.5), Matrix4d::IDENTITY));
	nanosuit->setMobility(Actor::Mobility::Dynamic);
	Material* nanosuitMaterial = ResourceManager::instance().loadResource<Material>("nanosuit");//createNanosuitMaterial();
	nanosuit->addComponent(new Nanosuit("resources/nanosuit/nanosuit.obj", nanosuitMaterial, "OpaqueObjectStage", true));

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

void createCornellBox(World* world)
{
	Shader shader;
	shader.createProgram(Shader::Vertex, "resources/cube_vert.glsl");
	shader.createProgram(Shader::Fragment, "resources/cube_frag.glsl");

	Actor* greenWall = world->createActor("greenWall", new Transform(Vector3d(-5.1, 0.0, 0.0), Vector3d(0.1, 5.0, 5.0), Matrix4d::IDENTITY));
	greenWall->setMobility(Actor::Mobility::Dynamic);
	Material* greenWallMaterial = new Material("greenWall");
	Technique* greenWallTechnique = new Technique(shader);
	greenWallMaterial->addTechnique(greenWallTechnique);
	greenWallMaterial->addAttribute("baseColor", Vector3f(4.0f, 153.0f, 26.0f) / 255.0f);
	greenWallMaterial->addAttribute("roughness", 0.5f);
	greenWallMaterial->addAttribute("metallic", 0.5f);
	greenWall->addComponent(new CubeComponent(greenWallMaterial, "OpaqueObjectStage", true));

	Actor* redWall = world->createActor("redWall", new Transform(Vector3d(5.1, 0.0, 0.0), Vector3d(0.1, 5.0, 5.0), Matrix4d::IDENTITY));
	redWall->setMobility(Actor::Mobility::Dynamic);
	Material* redWallMaterial = new Material("redWall");
	Technique* redWallTechnique = new Technique(shader);
	redWallMaterial->addTechnique(redWallTechnique);
	redWallMaterial->addAttribute("baseColor", Vector3f(1.0f, 0.0f, 0.0f));
	redWallMaterial->addAttribute("roughness", 0.5f);
	redWallMaterial->addAttribute("metallic", 0.5f);
	redWall->addComponent(new CubeComponent(redWallMaterial, "OpaqueObjectStage", true));

	Actor* whiteWall = world->createActor("whiteWall", new Transform(Vector3d(0.0, 0.0, -5.1), Vector3d(5.0, 5.0, 0.1), Matrix4d::IDENTITY));
	whiteWall->setMobility(Actor::Mobility::Dynamic);
	Material* whiteWallMaterial = new Material("whiteWall");
	Technique* whiteWallTechnique = new Technique(shader);
	whiteWallMaterial->addTechnique(whiteWallTechnique);
	whiteWallMaterial->addAttribute("baseColor", Vector3f(0.9f, 0.9f, 0.9f));
	whiteWallMaterial->addAttribute("roughness", 0.5f);
	whiteWallMaterial->addAttribute("metallic", 0.5f);
	whiteWall->addComponent(new CubeComponent(whiteWallMaterial, "OpaqueObjectStage", true));

	Actor* roof = world->createActor("roof", new Transform(Vector3d(0.0, 5.1, 0.0), Vector3d(5.0, 0.1, 5.0), Matrix4d::IDENTITY));
	roof->setMobility(Actor::Mobility::Dynamic);
	Material* roofMaterial = new Material("roof");
	Technique* roofTechnique = new Technique(shader);
	roofMaterial->addTechnique(roofTechnique);
	roofMaterial->addAttribute("baseColor", Vector3f(0.9f, 0.9f, 0.9f));
	roofMaterial->addAttribute("roughness", 0.5f);
	roofMaterial->addAttribute("metallic", 0.5f);
	roof->addComponent(new CubeComponent(roofMaterial, "OpaqueObjectStage", true));

	Actor* floor = world->createActor("floor", new Transform(Vector3d(0.0, -5.1, 0.0), Vector3d(5.0, 0.1, 5.0), Matrix4d::IDENTITY));
	floor->setMobility(Actor::Mobility::Dynamic);
	Material* floorMaterial = new Material("floor");
	Technique* floorTechnique = new Technique(shader);
	floorMaterial->addTechnique(floorTechnique);
	floorMaterial->addAttribute("baseColor", Vector3f(0.9f, 0.9f, 0.9f));
	floorMaterial->addAttribute("roughness", 0.5f);
	floorMaterial->addAttribute("metallic", 0.5f);
	floor->addComponent(new CubeComponent(floorMaterial, "OpaqueObjectStage", true));

	Actor* leftBox = world->createActor("leftBox", new Transform(Vector3d(-2.0, -5.1 + 2.8, -1.5), Vector3d(1.4, 2.8, 1.4), Matrix4d::createRotation(Vector3d::unitY(), 30.0)));
	leftBox->setMobility(Actor::Mobility::Dynamic);
	Material* leftBoxMaterial = new Material("leftBox");
	Technique* leftBoxTechnique = new Technique(shader);
	leftBoxMaterial->addTechnique(leftBoxTechnique);
	leftBoxMaterial->addAttribute("baseColor", Vector3f(0.9f, 0.9f, 0.9f));
	leftBoxMaterial->addAttribute("roughness", 0.5f);
	leftBoxMaterial->addAttribute("metallic", 0.5f);
	leftBox->addComponent(new CubeComponent(leftBoxMaterial, "OpaqueObjectStage", true));

	Actor* rightBox = world->createActor("rightBox", new Transform(Vector3d(2.8, -5.1 + 1.4, 1.5), Vector3d(1.4, 1.4, 1.4), Matrix4d::IDENTITY));
	rightBox->setMobility(Actor::Mobility::Dynamic);
	Material* rightBoxMaterial = new Material("rightBox");
	Technique* rightBoxTechnique = new Technique(shader);
	rightBoxMaterial->addTechnique(rightBoxTechnique);
	rightBoxMaterial->addAttribute("baseColor", Vector3f(0.9f, 0.9f, 0.9f));
	rightBoxMaterial->addAttribute("roughness", 0.5f);
	rightBoxMaterial->addAttribute("metallic", 0.5f);
	rightBox->addComponent(new CubeComponent(rightBoxMaterial, "OpaqueObjectStage", true));

	lightBox = world->createActor("lightBox", new Transform(Vector3d(0.0, 4.0, 0.0), Vector3d(0.5, 0.5, 0.5), Matrix4d::IDENTITY));
	Material* lightBoxMaterial = new Material("lightBox");
	Shader lightBoxShader;
	lightBoxShader.createProgram(Shader::Vertex, "resources/cube_vert.glsl");
	lightBoxShader.createProgram(Shader::Fragment, "resources/light_box_frag.glsl");
	Technique* lightBoxTechnique = new Technique(lightBoxShader);
	lightBoxMaterial->addTechnique(lightBoxTechnique);
	lightBoxMaterial->addAttribute("baseColor", Vector3f::one());
	lightBoxMaterial->addAttribute("emissive", Vector3f::one());
	lightBox->addComponent(new CubeComponent(lightBoxMaterial, "TransparentObjectStage", false));

	PointLightComponent* pointLight = new PointLightComponent();
	lightBox->addComponent(pointLight);

	//directionalLightActor = world->createActor("directionalLightActor", new Transform(Vector3d(0.0, 0.0, 10.0), Vector3d::one(), Matrix4d::IDENTITY));
	//directionalLightActor->setMobility(Actor::Mobility::Dynamic);
	//DirectionalLightComponent* directionalLight = new DirectionalLightComponent();
	//directionalLightActor->addComponent(directionalLight);
}

/*Material* createNanosuitMaterial()
{
	Shader shader;
	shader.createProgram(Shader::Vertex, "resources/nanosuit_vert.glsl");
	shader.createProgram(Shader::Fragment, "resources/nanosuit_frag.glsl");

	Shader shaderLights;
	shaderLights.createProgram(Shader::Vertex, "resources/nanosuit_vert.glsl");
	shaderLights.createProgram(Shader::Fragment, "resources/nanosuit_lights_frag.glsl");

	Texture::Parameters params;
	params.setWrapS(TextureWrap::Repeat);
	params.setWrapT(TextureWrap::Repeat);

	Material* nanosuitMaterial = new Material("nanosuit");

	Technique* glassTechnique = new Technique(shader);
	nanosuitMaterial->addTechnique(glassTechnique);
	Image<uint8> imageGlass;
	imageGlass.init("resources/nanosuit/glass_dif.png");
	Texture* glass = Texture::create2D(Texture::RGBA, imageGlass.getWidth(), imageGlass.getHeight(), Texture::RGBA8, Texture::UnsignedByte, imageGlass.getPixelsPtr(), params);
	imageGlass.init("resources/nanosuit/glass_ddn.png");
	Texture* glassNormal = Texture::create2D(Texture::RGBA, imageGlass.getWidth(), imageGlass.getHeight(), Texture::RGBA8, Texture::UnsignedByte, imageGlass.getPixelsPtr(), params);
	nanosuitMaterial->addAttribute("baseColor", glass, 0);
	nanosuitMaterial->addAttribute("normals", glassNormal, 0);

	Technique* legsTechnique = new Technique(shader);
	nanosuitMaterial->addTechnique(legsTechnique);
	Image<uint8> imageLegs;
	imageLegs.init("resources/nanosuit/leg_dif.png");
	Texture* legs = Texture::create2D(Texture::RGBA, imageLegs.getWidth(), imageLegs.getHeight(), Texture::RGBA8, Texture::UnsignedByte, imageLegs.getPixelsPtr(), params);
	imageLegs.init("resources/nanosuit/leg_showroom_ddn.png");
	Texture* legsNormal = Texture::create2D(Texture::RGBA, imageLegs.getWidth(), imageLegs.getHeight(), Texture::RGBA8, Texture::UnsignedByte, imageLegs.getPixelsPtr(), params);
	nanosuitMaterial->addAttribute("baseColor", legs, 1);
	nanosuitMaterial->addAttribute("normals", legsNormal, 1);

	Technique* handsTechnique = new Technique(shader);
	nanosuitMaterial->addTechnique(handsTechnique);
	Image<uint8> imageHands;
	imageHands.init("resources/nanosuit/hand_dif.png");
	Texture* hands = Texture::create2D(Texture::RGBA, imageHands.getWidth(), imageHands.getHeight(), Texture::RGBA8, Texture::UnsignedByte, imageHands.getPixelsPtr(), params);
	imageHands.init("resources/nanosuit/hand_showroom_ddn.png");
	Texture* handsNormal = Texture::create2D(Texture::RGBA, imageHands.getWidth(), imageHands.getHeight(), Texture::RGBA8, Texture::UnsignedByte, imageHands.getPixelsPtr(), params);
	nanosuitMaterial->addAttribute("baseColor", hands, 2);
	nanosuitMaterial->addAttribute("normals", handsNormal, 2);

	Technique* lightsTechnique = new Technique(shaderLights);
	nanosuitMaterial->addTechnique(lightsTechnique);
	nanosuitMaterial->addAttribute("baseColor", Vector3f(255.0, 235.0, 61.0) / 255.0, 3);
	nanosuitMaterial->addAttribute("emissive", Vector3f(255.0, 235.0, 61.0) / 255.0 * 10.0, 3);

	Technique* armsTechnique = new Technique(shader);
	nanosuitMaterial->addTechnique(armsTechnique);
	Image<uint8> imageArms;
	imageArms.init("resources/nanosuit/arm_dif.png");
	Texture* arms = Texture::create2D(Texture::RGBA, imageArms.getWidth(), imageArms.getHeight(), Texture::RGBA8, Texture::UnsignedByte, imageArms.getPixelsPtr(), params);
	imageArms.init("resources/nanosuit/arm_showroom_ddn.png");
	Texture* armsNormal = Texture::create2D(Texture::RGBA, imageArms.getWidth(), imageArms.getHeight(), Texture::RGBA8, Texture::UnsignedByte, imageArms.getPixelsPtr(), params);
	nanosuitMaterial->addAttribute("baseColor", arms, 4);
	nanosuitMaterial->addAttribute("normals", armsNormal, 4);

	Technique* helmetTechnique = new Technique(shader);
	nanosuitMaterial->addTechnique(helmetTechnique);
	Image<uint8> imageHelmet;
	imageHelmet.init("resources/nanosuit/helmet_diff.png");
	Texture* helmet = Texture::create2D(Texture::RGBA, imageHelmet.getWidth(), imageHelmet.getHeight(), Texture::RGBA8, Texture::UnsignedByte, imageHelmet.getPixelsPtr(), params);
	imageHelmet.init("resources/nanosuit/helmet_showroom_ddn.png");
	Texture* helmetNormal = Texture::create2D(Texture::RGBA, imageHelmet.getWidth(), imageHelmet.getHeight(), Texture::RGBA8, Texture::UnsignedByte, imageHelmet.getPixelsPtr(), params);
	nanosuitMaterial->addAttribute("baseColor", helmet, 5);
	nanosuitMaterial->addAttribute("normals", helmetNormal, 5);

	Technique* bodyTechnique = new Technique(shader);
	nanosuitMaterial->addTechnique(bodyTechnique);
	Image<uint8> imageBody;
	imageBody.init("resources/nanosuit/body_dif.png");
	Texture* body = Texture::create2D(Texture::RGBA, imageBody.getWidth(), imageBody.getHeight(), Texture::RGBA8, Texture::UnsignedByte, imageBody.getPixelsPtr(), params);
	imageBody.init("resources/nanosuit/body_showroom_ddn.png");
	Texture* bodyNormal = Texture::create2D(Texture::RGBA, imageBody.getWidth(), imageBody.getHeight(), Texture::RGBA8, Texture::UnsignedByte, imageBody.getPixelsPtr(), params);
	nanosuitMaterial->addAttribute("baseColor", body, 6);
	nanosuitMaterial->addAttribute("normals", bodyNormal, 6);

	return nanosuitMaterial;
}*/