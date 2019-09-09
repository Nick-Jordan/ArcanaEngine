// TerrainClient.cpp : Defines the entry point for the application.
//
#include "stdafx.h"
#include "TerrainClient.h"

#include "Application.h"

#include "WindowsWindowDefinition.h"
#include "WindowsApplicationDefinition.h"

#include "Renderer.h"
#include "EventListener.h"
#include "KeyEvent.h"
#include "Key.h"
#include "NoDataEvents.h"
#include "Globals.h"
#include "FPSCharacter.h"

#include "HeightField.h"
//#include "TerrainBumpMapGenerator.h"

#include "GeometryComponent.h"
#include "CameraComponent.h"
#include "ResourceManager.h"
#include "Input.h"
#include "TerrainComponent.h"
#include "AtmosphereComponent.h"
#include "CloudsComponent.h"
#include "Profiler.h"

#include "PostProcessor.h"
#include "PostProcessQueue.h"
#include "FilmicTonemapEffect.h"

#define PROFILE_FRAMES

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

PostProcessQueue effectQueue;

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
			handler.broadcast(WindowClosedEvent());
		}
		if (event.getInt("keyCode") == KeyCode::Add)
		{
			ControllableActor::speed *= 1.2;
		}
		if (event.getInt("keyCode") == KeyCode::Subtract)
		{
			ControllableActor::speed /= 1.2;
		}
		if (event.getInt("keyCode") == KeyCode::N)
		{
			TerrainNode::updateQuad = false;// !TerrainNode::updateQuad;
		}
		if (event.getInt("keyCode") == KeyCode::M)
		{
			TerrainNode::updateQuad = true;// !TerrainNode::updateQuad;
		}

		if (event.getInt("keyCode") == KeyCode::Comma)
		{
			FilmicTonemapEffect::Exposure -= 0.05f;
		}
		if (event.getInt("keyCode") == KeyCode::Period)
		{
			FilmicTonemapEffect::Exposure += 0.05f;
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
	appDefinition.setAppName("Terrain Client");
	appDefinition.setWindowClass(L"TERRAIN_CLIENT");
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

	GEngine->setStationaryCursor(true);
	GEngine->getApplicationInstance()->getActiveWindow().setVerticalSync(false);

	World* world = new World("world");

	Terrain::Parameters params;
	params.maxLevel = 16;
	params.radius = 6367000.0;
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

	Actor* actor = world->createActor("actor", Transform());

	TerrainComponent* front = new TerrainComponent(params, Transform());
	actor->addComponent(front);
	TerrainComponent* back = new TerrainComponent(params, Transform(Vector3d(0.0, 0.0, 0.0), Vector3d::one(), Matrix4d::createRotation(Vector3d::unitX(), 180.0)));
	actor->addComponent(back);
	TerrainComponent* bottom = new TerrainComponent(params, Transform(Vector3d(0.0, 0.0, 0.0), Vector3d::one(), Matrix4d::createRotation(Vector3d::unitX(), 90.0)));
	actor->addComponent(bottom);
	TerrainComponent* top = new TerrainComponent(params, Transform(Vector3d(0.0, 0.0, 0.0), Vector3d::one(), Matrix4d::createRotation(Vector3d::unitX(), -90.0)));
	actor->addComponent(top);
	TerrainComponent* right = new TerrainComponent(params, Transform(Vector3d(0.0, 0.0, 0.0), Vector3d::one(), Matrix4d::createRotation(Vector3d::unitY(), 90.0)));
	actor->addComponent(right);
	TerrainComponent* left = new TerrainComponent(params, Transform(Vector3d(0.0, 0.0, 0.0), Vector3d::one(), Matrix4d::createRotation(Vector3d::unitY(), -90.0)));
	actor->addComponent(left);

	/*actor->addComponent(new AtmosphereComponent());

	CloudsComponent* frontClouds = new CloudsComponent(cloudsParams, new Transform());
	actor->addComponent(frontClouds);
	CloudsComponent* backClouds = new CloudsComponent(cloudsParams, new Transform(Vector3d(0.0, 0.0, 0.0), Vector3d::one(), Matrix4d::createRotation(Vector3d::unitX(), 180.0)));
	actor->addComponent(backClouds);
	CloudsComponent* bottomClouds = new CloudsComponent(cloudsParams, new Transform(Vector3d(0.0, 0.0, 0.0), Vector3d::one(), Matrix4d::createRotation(Vector3d::unitX(), 90.0)));
	actor->addComponent(bottomClouds);
	CloudsComponent* topClouds = new CloudsComponent(cloudsParams, new Transform(Vector3d(0.0, 0.0, 0.0), Vector3d::one(), Matrix4d::createRotation(Vector3d::unitX(), -90.0)));
	actor->addComponent(topClouds);
	CloudsComponent* rightClouds = new CloudsComponent(cloudsParams, new Transform(Vector3d(0.0, 0.0, 0.0), Vector3d::one(), Matrix4d::createRotation(Vector3d::unitY(), 90.0)));
	actor->addComponent(rightClouds);
	CloudsComponent* leftClouds = new CloudsComponent(cloudsParams, new Transform(Vector3d(0.0, 0.0, 0.0), Vector3d::one(), Matrix4d::createRotation(Vector3d::unitY(), -90.0)));
	actor->addComponent(leftClouds);*/

	FPSCharacter* camera = world->createActor<FPSCharacter>("camera", Transform(Vector3d(0.0, 0.0, 0.0), Vector3d::one(), Matrix4d::IDENTITY));
	CameraComponent* cameraComponent = new CameraComponent(90.0f, GEngine->getApplicationInstance()->getActiveWindow().getAspectRatio(), 0.000001, pow(10.0, 10.0));
	cameraComponent->setPosition(Vector3d(0.0, 0.0, 2 * 6370000.0));
	camera->addComponent(cameraComponent);


	effectQueue.setBaseEffect(EFFECT("EmissiveHDRComposite"));
	effectQueue.addEffect(EFFECT("FXAA"));
	effectQueue.setEndEffect(EFFECT("FilmicTonemap"));
	PostProcessor::buildQueue(effectQueue);


	GEngine->setWorld(world);


	GEngine->start();
	GEngine->exit();

	DestroyEngine();


	
	//test terrain generator
	/*TerrainBumpMapGenerator* generator = new TerrainBumpMapGenerator();

	generator->generate(TerrainProceduralParams());

	HeightField<Vector4d>* bump = generator->get();

	Image<uint8> image;
	image.init(ImageFormat::RGBA, bump->getNumRows(), bump->getNumColumns(), Color());

	for (int i = 0; i < bump->getNumRows(); i++)
	{
		for (int j = 0; j < bump->getNumColumns(); j++)
		{
			Vector4d c = bump->getHeight(i, j);
			uint8 r = (c.x * 0.5 + 0.5) * 255;
			uint8 g = (c.y * 0.5 + 0.5) * 255;
			uint8 b = (c.z * 0.5 + 0.5) * 255;
			image.setPixel(i, j, Color(r, g, b, 255));
		}
	}

	image.save("resources/normal_map.png");

	AE_RELEASE(bump);
	AE_DELETE(generator);*/

	return 1;
}