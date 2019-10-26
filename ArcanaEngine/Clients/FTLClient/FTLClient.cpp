// FTLClient.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "FTLClient.h"

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
#include "Random.h"
#include "MeshRenderProcedure.h"
#include "ParticleEmitterComponent.h"
#include "StaticMeshComponent.h"
#include "SkyboxActor.h"
#include "DecalComponent.h"

#include "GUIWindow.h"
#include "Button.h"
#include "CheckBox.h"
#include "Panel.h"
#include "Label.h"

#include "PointLightComponent.h"
#include "DirectionalLightComponent.h"
#include "Color.h"
#include "SoundEngine.h"
#include "PostProcessor.h"
#include "PostProcessQueue.h"
#include "TextureUtils.h"

#include "FPSCharacter.h"


//FasterThanLight
#define BUILD_LIGHTING
#include "FasterThanLight.h"

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

FPSCharacter* camera;

PostProcessQueue effectQueue;

StaticMesh* CubeMesh = nullptr;
StaticMesh* TransparentCubeMesh = nullptr;

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
		static bool cursorStationary = true;

		if (event.getInt("event") == KeyEvent::Released)
		{
			if (event.getInt("keyCode") == KeyCode::Escape || event.getInt("keyCode") == KeyCode::ControllerSpecialRight)
			{
				handler.broadcast(WindowClosedEvent());
			}
			else if (event.getInt("keyCode") == KeyCode::Add)
			{
				ControllableActor::speed *= 1.2;
			}
			else if (event.getInt("keyCode") == KeyCode::Subtract)
			{
				ControllableActor::speed /= 1.2;
			}
		}


		return true;
	}
};

void createCornellBox(World* world);

Actor* lightBox;

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
	appDefinition.setAppName("FTL Client");
	appDefinition.setWindowClass(L"FTL_CLIENT");
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
	settings.splashScreenLength = 8.0f;
	settings.sRgb = false;

	GEngine->setRenderer(settings);

	GEngine->getApplicationInstance()->getActiveWindow().setVerticalSync(false);
	GEngine->setStationaryCursor(true);

	World* world = new World("world");

	createCornellBox(world);

	camera = world->createActor<FPSCharacter>("camera", Transform(Vector3d(0.0, 0.0, 0.0), Vector3d::one(), Matrix4d::IDENTITY));
	CameraComponent* cameraComponent = new CameraComponent(90.0f, GEngine->getApplicationInstance()->getActiveWindow().getAspectRatio(), 0.1, 1000.0);
	cameraComponent->setPosition(Vector3d(0.0, 0.0, 2.0));
	camera->addComponent(cameraComponent);

	//post processing
	effectQueue.setBaseEffect(EFFECT("EmissiveHDRComposite"));
	effectQueue.setEndEffect(EFFECT("FilmicTonemap"));
	effectQueue.addEffect(EFFECT("FXAA"));

	PostProcessor::buildQueue(effectQueue);
	//post processing

	GEngine->setWorld(world);


	FTL::NumLightBounces = 0;
	//FTL::NumPhotons = 10;
	//FTL::NumCausticsPhotons = 10;
	//FTL::MaxIterations = 1;
	//FTL::MaxCausticsIterations = 1;
	FASTER_THAN_LIGHT(world);

	Mesh* debugMesh = FTL::LightProcessor::DebugMesh;

	if (debugMesh)
	{
		LOG(Info, CoreEngine, "Adding debug mesh");

		Actor* debug = world->createActor("debug", Transform(Vector3d::zero(), Vector3d::one(), Matrix4d::IDENTITY));
		Material* debugMaterial = new Material("debug");
		Shader shader;
		shader.createProgram(Shader::Vertex, "resources/arcana/shaders/ftl/debug_rays_vert.glsl");
		shader.createProgram(Shader::Fragment, "resources/arcana/shaders/ftl/debug_rays_frag.glsl");
		Technique* technique = new Technique(shader);
		debugMaterial->addTechnique(technique);

		RenderProcedure::RenderProperties properties;
		properties.RendererStage = "TransparentObjectStage";
		properties.LightProperties.CastsDynamicShadow = false;
		properties.RenderState.setCullEnabled(false);
		properties.RenderState.setDepthTestEnabled(true);
		properties.RenderState.setBlendEnabled(false);
		debug->addComponent(new MeshComponent(debugMesh, debugMaterial, properties));
	}

	GEngine->start();
	GEngine->exit();

	FTL_CLEANUP();
	DestroyEngine();

	if (!CubeMesh)
	{
		AE_DELETE(CubeMesh);
		AE_DELETE(TransparentCubeMesh);
	}

	return 1;
}

void createCornellBox(World* world)
{
	if (!CubeMesh)
	{
		StaticMesh::Properties properties;
		properties.isEnvironmentMesh = false;
		properties.isTransparent = false;
		properties.LightMapResolution = 128;
		properties.LightProperties.CastsDynamicShadow = false;
		properties.RenderState.setCullEnabled(true);
		properties.RenderState.setCullFaceSide(RenderState::Back);
		properties.RenderState.setDepthTestEnabled(true);
		properties.RenderState.setBlendEnabled(false);
		CubeMesh = new StaticMesh("resources/cube.mesh", properties);

		StaticMesh::Properties propertiesTransparent;
		propertiesTransparent.isEnvironmentMesh = false;
		propertiesTransparent.isTransparent = true;
		propertiesTransparent.LightMapResolution = 0;
		propertiesTransparent.LightProperties.CastsDynamicShadow = false;
		propertiesTransparent.RenderState.setCullEnabled(true);
		propertiesTransparent.RenderState.setCullFaceSide(RenderState::Back);
		propertiesTransparent.RenderState.setDepthTestEnabled(true);
		propertiesTransparent.RenderState.setBlendEnabled(true);
		propertiesTransparent.RenderState.setBlendSrc(RenderState::SrcAlpha);
		propertiesTransparent.RenderState.setBlendDst(RenderState::OneMinusSrcAlpha);
		TransparentCubeMesh = new StaticMesh("resources/cube.mesh", propertiesTransparent);
	}

	Shader shader;
	shader.createProgram(Shader::Vertex, "resources/cube_vert.glsl");
	shader.createProgram(Shader::Fragment, "resources/ftl_cube_frag.glsl");

	Actor* redWall = world->createActor("redWall", Transform(Vector3d(5.1, 0.0, 0.0), Vector3d(0.1, 5.0, 5.0), Matrix4d::IDENTITY));
	redWall->setMobility(Actor::Mobility::Static);
	Material* redWallMaterial = new Material("redWall");
	Technique* redWallTechnique = new Technique(shader);
	redWallMaterial->addTechnique(redWallTechnique);
	redWallMaterial->addAttribute("baseColor", LinearColor::Red, false);
	redWallMaterial->addAttribute("roughness", 0.5f);
	redWallMaterial->addAttribute("metallic", 0.5f);
	redWall->addComponent(new StaticMeshComponent(CubeMesh, redWallMaterial));

	/*Actor* floor = world->createActor("floor", Transform(Vector3d(0.0, -5.1, 0.0), Vector3d(5.0, 0.1, 5.0), Matrix4d::IDENTITY));
	floor->setMobility(Actor::Mobility::Static);
	Material* floorMaterial = new Material("floor");
	Technique* floorTechnique = new Technique(shader);
	floorMaterial->addTechnique(floorTechnique);
	floorMaterial->addAttribute("baseColor", Vector3f(0.5f, 0.5f, 0.5f));
	floorMaterial->addAttribute("roughness", 0.5f);
	floorMaterial->addAttribute("metallic", 0.5f);
	//Technique* floorTechnique = ResourceManager::instance().loadResource<Technique>("resources/arcana/materials/bamboo-wood-semigloss.xml", "bamboo-wood-semigloss");
	//Shader floorShader;
	//floorShader.createProgram(Shader::Vertex, "resources/cube_vert.glsl");
	//floorShader.createProgram(Shader::Fragment, "resources/textured_cube_frag.glsl");
	//floorTechnique->setPass(0, floorShader);
	floorMaterial->addTechnique(floorTechnique);
	floor->addComponent(new StaticMeshComponent(CubeMesh, floorMaterial));*/

	Actor* rightBox = world->createActor("rightBox", Transform(Vector3d(2.8, -5.1 + 1.4, 1.5), Vector3d(1.4, 1.4, 1.4), Matrix4d::IDENTITY));
	rightBox->setMobility(Actor::Mobility::Static);
	Material* rightBoxMaterial = new Material("rightBox");
	Technique* rightBoxTechnique = new Technique(shader);
	rightBoxMaterial->addTechnique(rightBoxTechnique);
	rightBoxMaterial->addAttribute("baseColor", Vector3f(0.9f, 0.9f, 0.9f));
	rightBoxMaterial->addAttribute("roughness", 0.5f);
	rightBoxMaterial->addAttribute("metallic", 0.5f);
	rightBox->addComponent(new StaticMeshComponent(CubeMesh, rightBoxMaterial));

	/*Actor* transparentBox = world->createActor("transparentBox", Transform(Vector3d(2.0, 0.0, -2.0), Vector3d(1.0, 1.0, 1.0), Matrix4d::IDENTITY));
	transparentBox->setMobility(Actor::Mobility::Static);
	Material* transparentBoxMaterial = new Material("transparentBox");
	Shader transparentShader;
	transparentShader.createProgram(Shader::Vertex, "resources/cube_vert.glsl");
	transparentShader.createProgram(Shader::Fragment, "resources/transparent_cube_frag.glsl");
	Technique* transparentBoxTechnique = new Technique(transparentShader);
	transparentBoxMaterial->addTechnique(transparentBoxTechnique);
	transparentBoxMaterial->addAttribute("baseColor", Vector4f(0.1f, 0.1f, 0.7f, 0.3f));
	transparentBox->addComponent(new StaticMeshComponent(TransparentCubeMesh, transparentBoxMaterial));*/

	Actor* staticlightBox = world->createActor("staticlightBox", Transform(Vector3d(0.0, 4.0, 0.0), Vector3d(0.5, 0.5, 0.5), Matrix4d::IDENTITY));
	staticlightBox->setMobility(Actor::Mobility::Static);
	Material* staticlightBoxMaterial = new Material("staticlightBox");
	Shader staticlightBoxShader;
	staticlightBoxShader.createProgram(Shader::Vertex, "resources/cube_vert.glsl");
	staticlightBoxShader.createProgram(Shader::Fragment, "resources/light_box_frag.glsl");
	Technique* staticlightTechnique = new Technique(staticlightBoxShader);
	staticlightBoxMaterial->addTechnique(staticlightTechnique);
	staticlightBoxMaterial->addAttribute("baseColor", Vector3f::one());
	staticlightBoxMaterial->addAttribute("emissive", Vector3f::one());
	staticlightBox->addComponent(new StaticMeshComponent(TransparentCubeMesh, staticlightBoxMaterial));
	
	PointLightComponent* staticPointLight = new PointLightComponent();
	staticlightBox->addComponent(staticPointLight);
}