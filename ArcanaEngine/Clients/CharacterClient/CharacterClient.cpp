// CharacterClient.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "CharacterClient.h"

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
#include "ParticleEmitterComponent.h"
#include "StaticMeshComponent.h"

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

#include "FPSCharacter.h"


//FasterThanLight
//#define BUILD_LIGHTING
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
			else if (event.getInt("keyCode") == KeyCode::V)
			{
				effectQueue.toggleEffect("Vignette");
			}
			else if(event.getInt("keyCode") == KeyCode::N)
			{
				effectQueue.toggleEffect("NightVision");
			}
			else if (event.getInt("keyCode") == KeyCode::C)
			{
				effectQueue.toggleEffect("ColorInversion");
			}
			else if (event.getInt("keyCode") == KeyCode::B)
			{
				effectQueue.toggleEffect("Blur");
			}
			else if (event.getInt("keyCode") == KeyCode::G)
			{
				effectQueue.toggleEffect("Grayscale");
			}
			else if (event.getInt("keyCode") == KeyCode::L)
			{
				effectQueue.toggleEffect("ColorGrading");
			}
			else if (event.getInt("keyCode") == KeyCode::K)
			{
				effectQueue.toggleEffect("EdgeDetection");
			}
			else if (event.getInt("keyCode") == KeyCode::J)
			{
				effectQueue.toggleEffect("Sharpen");
			}
			else if (event.getInt("keyCode") == KeyCode::F)
			{
				effectQueue.toggleEffect("FXAA");
			}
		}


		return true;
	}
};

void createCornellBox(World* world);

Actor* lightBox;

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

void button()
{
	LOG(Info, CoreEngine, "BUTTON PRESSED");
}

void actorUpdate(double x)
{
	LOGF(Info, CoreEngine, "actor: %f", x);
}
void actorCompUpdate(double x)
{
	LOGF(Info, CoreEngine, "actor comp: %f", x);
}
void timelineTrigger()
{
	LOG(Info, CoreEngine, "timeline trigger");
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
	appDefinition.setAppName("Character Client");
	appDefinition.setWindowClass(L"CHARACTER_CLIENT");
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

	camera = world->createActor<FPSCharacter>("camera", new Transform(Vector3d(0.0, 0.0, 0.0), Vector3d::one(), Matrix4d::IDENTITY));
	CameraComponent* cameraComponent = new CameraComponent(90.0f, GEngine->getApplicationInstance()->getActiveWindow().getAspectRatio(), 0.1, 1000.0);
	cameraComponent->setPosition(Vector3d(0.0, 0.0, 2.0));
	camera->addComponent(cameraComponent);

	TimelineTrigger trigger;
	trigger.bind(timelineTrigger);
	camera->getSceneComponent()->getTimeline().setTimelineLengthMode(Timeline::LengthMode::LastKeyFrame);
	camera->getSceneComponent()->getTimeline().addTrigger(2.0, trigger);
	camera->getSceneComponent()->updateFunction().bind(actorCompUpdate);

	//post processing
	effectQueue.setBaseEffect(EFFECT("EmissiveHDRComposite"));
	effectQueue.setEndEffect(EFFECT("Vignette"), false);
	effectQueue.addEffect(EFFECT("FXAA"));
	effectQueue.addEffect(EFFECT("NightVision"), false);
	effectQueue.addEffect(EFFECT("Grayscale"), false);
	effectQueue.addEffect(EFFECT("Blur"), false);
	effectQueue.addEffect(EFFECT("ColorInversion"), false);
	effectQueue.addEffect(EFFECT("EdgeDetection"), false);
	effectQueue.addEffect(EFFECT("Sharpen"), false);
	effectQueue.addEffect(EFFECT("FilmicTonemap"));
	effectQueue.addEffect(EFFECT("ColorGrading"), false);

	PostProcessor::buildQueue(effectQueue);
	//post processing

	GEngine->setWorld(world);

	/*SoundEngine* s = new SoundEngine();

	s->_engine->play2D("resources/arcana/sounds/op48no1.mp3", true);*/

	FTL::NumLightBounces = 3;
	FASTER_THAN_LIGHT(world);

	Mesh* debugMesh = FTL::LightProcessor::DebugMesh;

	if (debugMesh)
	{
		LOG(Info, CoreEngine, "Adding debug mesh");

		Actor* debug = world->createActor("debug", new Transform(Vector3d::zero(), Vector3d::one(), Matrix4d::IDENTITY));
		Material* debugMaterial = new Material("debug");
		Shader shader;
		shader.createProgram(Shader::Vertex, "resources/arcana/shaders/ftl/debug_rays_vert.glsl");
		shader.createProgram(Shader::Fragment, "resources/arcana/shaders/ftl/debug_rays_frag.glsl");
		Technique* technique = new Technique(shader);
		debugMaterial->addTechnique(technique);

		MeshRenderProperties properties;
		properties.rendererStage = "TransparentObjectStage";
		properties.lightProperties.CastsDynamicShadow = false;
		properties.renderState.setCullEnabled(false);
		properties.renderState.setDepthTestEnabled(true);
		properties.renderState.setBlendEnabled(false);
		debug->addComponent(new MeshComponent(debugMesh, debugMaterial, properties));
	}

	GEngine->start();
	GEngine->exit();

	DestroyEngine();

	return 1;
}

StaticMesh* CubeMesh = nullptr;
StaticMesh* TransparentCubeMesh = nullptr;

void createCornellBox(World* world)
{
	if (!CubeMesh)
	{
		StaticMesh::Properties properties;
		properties.isEnvironmentMesh = false;
		properties.isTransparent = false;
		properties.LightMapResolution = 0;
		properties.LightProperties.CastsDynamicShadow = true;
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

	Actor* greenWall = world->createActor("greenWall", new Transform(Vector3d(-5.1, 0.0, 0.0), Vector3d(0.1, 5.0, 5.0), Matrix4d::IDENTITY));
	greenWall->setMobility(Actor::Mobility::Static);
	Material* greenWallMaterial = new Material("greenWall");
	Technique* greenWallTechnique = new Technique(shader);
	greenWallMaterial->addTechnique(greenWallTechnique);
	greenWallMaterial->addAttribute("baseColor", Vector3f(4.0f, 153.0f, 26.0f) / 255.0f);
	greenWallMaterial->addAttribute("roughness", 0.5f);
	greenWallMaterial->addAttribute("metallic", 0.5f);
	greenWall->addComponent(new StaticMeshComponent(CubeMesh, greenWallMaterial));

	Actor* redWall = world->createActor("redWall", new Transform(Vector3d(5.1, 0.0, 0.0), Vector3d(0.1, 5.0, 5.0), Matrix4d::IDENTITY));
	redWall->setMobility(Actor::Mobility::Static);
	Material* redWallMaterial = new Material("redWall");
	Technique* redWallTechnique = new Technique(shader);
	redWallMaterial->addTechnique(redWallTechnique);
	redWallMaterial->addAttribute("baseColor", Vector3f(1.0f, 0.0f, 0.0f));
	redWallMaterial->addAttribute("roughness", 0.5f);
	redWallMaterial->addAttribute("metallic", 0.5f);
	redWall->addComponent(new StaticMeshComponent(CubeMesh, redWallMaterial));

	Actor* whiteWall = world->createActor("whiteWall", new Transform(Vector3d(0.0, 0.0, -5.1), Vector3d(5.0, 5.0, 0.1), Matrix4d::IDENTITY));
	whiteWall->setMobility(Actor::Mobility::Static);
	Material* whiteWallMaterial = new Material("whiteWall");
	Technique* whiteWallTechnique = new Technique(shader);
	whiteWallMaterial->addTechnique(whiteWallTechnique);
	whiteWallMaterial->addAttribute("baseColor", Vector3f(0.9f, 0.9f, 0.9f));
	whiteWallMaterial->addAttribute("roughness", 0.5f);
	whiteWallMaterial->addAttribute("metallic", 0.5f);
	whiteWall->addComponent(new StaticMeshComponent(CubeMesh, whiteWallMaterial));

	Actor* roof = world->createActor("roof", new Transform(Vector3d(0.0, 5.1, 0.0), Vector3d(5.0, 0.1, 5.0), Matrix4d::IDENTITY));
	roof->setMobility(Actor::Mobility::Static);
	Material* roofMaterial = new Material("roof");
	Technique* roofTechnique = new Technique(shader);
	roofMaterial->addTechnique(roofTechnique);
	roofMaterial->addAttribute("baseColor", Vector3f(0.9f, 0.9f, 0.9f));
	roofMaterial->addAttribute("roughness", 0.5f);
	roofMaterial->addAttribute("metallic", 0.5f);
	roof->addComponent(new StaticMeshComponent(CubeMesh, roofMaterial));

	Actor* floor = world->createActor("floor", new Transform(Vector3d(0.0, -5.1, 0.0), Vector3d(5.0, 0.1, 5.0), Matrix4d::IDENTITY));
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
	floor->addComponent(new StaticMeshComponent(CubeMesh, floorMaterial));

	Actor* leftBox = world->createActor("leftBox", new Transform(Vector3d(-2.0, -5.1 + 2.8, -1.5), Vector3d(1.4, 2.8, 1.4), Matrix4d::createRotation(Vector3d::unitY(), 30.0)));
	leftBox->setMobility(Actor::Mobility::Static);
	Material* leftBoxMaterial = new Material("leftBox");
	Technique* leftBoxTechnique = new Technique(shader);
	leftBoxMaterial->addTechnique(leftBoxTechnique);
	leftBoxMaterial->addAttribute("baseColor", Vector3f(0.9f, 0.9f, 0.9f));
	leftBoxMaterial->addAttribute("roughness", 0.5f);
	leftBoxMaterial->addAttribute("metallic", 0.5f);
	leftBox->addComponent(new StaticMeshComponent(CubeMesh, leftBoxMaterial));

	Actor* rightBox = world->createActor("rightBox", new Transform(Vector3d(2.8, -5.1 + 1.4, 1.5), Vector3d(1.4, 1.4, 1.4), Matrix4d::IDENTITY));
	rightBox->setMobility(Actor::Mobility::Static);
	Material* rightBoxMaterial = new Material("rightBox");
	Technique* rightBoxTechnique = new Technique(shader);
	rightBoxMaterial->addTechnique(rightBoxTechnique);
	rightBoxMaterial->addAttribute("baseColor", Vector3f(0.9f, 0.9f, 0.9f));
	rightBoxMaterial->addAttribute("roughness", 0.5f);
	rightBoxMaterial->addAttribute("metallic", 0.5f);
	rightBox->addComponent(new StaticMeshComponent(CubeMesh, rightBoxMaterial));

	Actor* transparentBox = world->createActor("transparentBox", new Transform(Vector3d(2.0, 0.0, -2.0), Vector3d(1.0, 1.0, 1.0), Matrix4d::IDENTITY));
	transparentBox->setMobility(Actor::Mobility::Static);
	Material* transparentBoxMaterial = new Material("transparentBox");
	Shader transparentShader;
	transparentShader.createProgram(Shader::Vertex, "resources/cube_vert.glsl");
	transparentShader.createProgram(Shader::Fragment, "resources/transparent_cube_frag.glsl");
	Technique* transparentBoxTechnique = new Technique(transparentShader);
	transparentBoxMaterial->addTechnique(transparentBoxTechnique);
	transparentBoxMaterial->addAttribute("baseColor", Vector4f(0.1f, 0.1f, 0.7f, 0.3f));
	transparentBox->addComponent(new StaticMeshComponent(TransparentCubeMesh, transparentBoxMaterial));

	/*lightBox = world->createActor("lightBox", new Transform(Vector3d(0.0, 4.0, 0.0), Vector3d(0.5, 0.5, 0.5), Matrix4d::IDENTITY));
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
	lightBox->addComponent(pointLight);*/

	/*lightBox = world->createActor("lightBox", new Transform(Vector3d(0.0, 4.0, 0.0), Vector3d(0.5, 0.5, 0.5), Matrix4d::IDENTITY));
	lightBox->setMobility(Actor::Mobility::Dynamic);
	Material* lightBoxMaterial = new Material("lightBox");
	Technique* lightBoxTechnique = new Technique(shader);
	lightBoxMaterial->addTechnique(lightBoxTechnique);
	lightBoxMaterial->addAttribute("baseColor", Vector3f(0.5f, 0.5f, 0.5f));
	lightBoxMaterial->addAttribute("roughness", 0.5f);
	lightBoxMaterial->addAttribute("metallic", 0.5f);
	lightBox->addComponent(new StaticMeshComponent(CubeMesh, lightBoxMaterial));*/


	ParticleEmitterProperties properties;
	properties.emissionRate = 100;
	properties.ellipsoid = false;
	properties.sizeStartMin = 0.2f;
	properties.sizeStartMax = 0.5f;
	properties.sizeEndMin = 0.1f;
	properties.sizeEndMax = 0.4f;
	properties.energyMin = 1000.0;
	properties.energyMax = 4000.0;
	properties.colorStart = Vector4f::one();
	properties.colorStartVar = Vector4f::zero();
	properties.colorEnd = Vector4f(1.0, 0.0, 0.0, 0.0);
	properties.colorEndVar = Vector4f::zero();
	properties.rotationPerParticleSpeedMin = 0.0f;
	properties.rotationPerParticleSpeedMax = 0.0f;
	properties.rotationSpeedMin = 0.0f;
	properties.rotationSpeedMax = 0.0f;
	properties.accelerationVar = Vector3f::one();
	properties.velocityVar = Vector3f::one();
	properties.positionVar = Vector3f(0.5, 0.5, 0.5);
	properties.frameRandomOffset = 0;
	properties.orbitPosition = false;
	properties.orbitVelocity = false;
	properties.orbitAcceleration = false;
	properties.useVelocityField = false;
	properties.useAccelerationField = false;

	ParticleEmitterComponent* emitter = new ParticleEmitterComponent(20000, properties, *GlobalShaders::get(GlobalShaders::Particles));

	Image<uint8> image;
	image.init("resources/arcana/textures/particles/particle17.png");
	Texture* texture = Texture::create2D(Texture::RGBA, 64, 64, Texture::RGBA8, Texture::UnsignedByte, image.getPixelsPtr());
	emitter->setTexture(texture);
	AE_RELEASE(texture);

	emitter->start();

	Actor* staticlightBox = world->createActor("staticlightBox", new Transform(Vector3d(0.0, 4.0, 0.0), Vector3d(0.5, 0.5, 0.5), Matrix4d::IDENTITY));
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

	staticlightBox->addComponent(emitter);

}