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
#include "Colors.h"

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

#include "PointLightComponent.h"
#include "DirectionalLightComponent.h"
#include "Color.h"
#include "SoundEngine.h"
#include "PostProcessor.h"
#include "PostProcessQueue.h"
#include "TextureUtils.h"
#include "FilmicTonemapEffect.h"

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

StaticMesh* CubeMesh = nullptr;
StaticMesh* TransparentCubeMesh = nullptr;
StaticMesh* Spaceship = nullptr;
StaticMesh* Monster = nullptr;

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
			else if (event.getInt("keyCode") == KeyCode::T)
			{
				effectQueue.toggleEffect("FilmicTonemap");
			}
			else if (event.getInt("keyCode") == KeyCode::Add)
			{
				ControllableActor::speed *= 1.2;
			}
			else if (event.getInt("keyCode") == KeyCode::Subtract)
			{
				ControllableActor::speed /= 1.2;
			}
			else if (event.getInt("keyCode") == KeyCode::Comma)
			{
				FilmicTonemapEffect::Exposure -= 0.01;
			}
			else if (event.getInt("keyCode") == KeyCode::Period)
			{
				FilmicTonemapEffect::Exposure += 0.01;
			}
		}


		return true;
	}
};

void createCornellBox(World* world);

void createLargeBox(World* world);

Vector3f test()
{
	Vector3f c = camera->getTimeline().getCurrentVector();
	return c;
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

	//Cornell Box with Decals
	createCornellBox(world);

	//Larger box
	//createLargeBox(world);

	camera = world->createActor<FPSCharacter>("camera", Transform(Vector3d(0.0, 0.0, 0.0), Vector3d::one(), Matrix4d::IDENTITY));
	CameraComponent* cameraComponent = new CameraComponent(90.0f, GEngine->getApplicationInstance()->getActiveWindow().getAspectRatio(), 0.1, 1000.0);
	cameraComponent->setPosition(Vector3d(0.0, 0.0, 2.0));
	camera->addComponent(cameraComponent);

	TimelineTrigger trigger;
	camera->getTimeline().setTimelineLengthMode(Timeline::LengthMode::TimelineLength);
	camera->getTimeline().setTimelineLength(100.0);
	camera->getTimeline().addVector(10.0, Vector3f::zero());
	camera->getTimeline().addVector(20.0, Vector3f::unitX());
	camera->getTimeline().addVector(30.0, Vector3f::unitZ());
	camera->getTimeline().addVector(40.0, Vector3f::unitY());
	camera->getTimeline().addVector(50.0, Vector3f::unitX());
	camera->getTimeline().addVector(60.0, Vector3f(0.5, 0.5, 0.5));

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

	/*FTL::NumLightBounces = 3;
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
	}*/

	GEngine->start();
	GEngine->exit();

	//FTL_CLEANUP();
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

	Actor* greenWall = world->createActor("greenWall", Transform(Vector3d(-5.1, 0.0, 0.0), Vector3d(0.1, 5.0, 5.0), Matrix4d::IDENTITY));
	greenWall->setMobility(Mobility::Static);
	Material* greenWallMaterial = new Material("greenWall");
	Technique* greenWallTechnique = new Technique(shader);
	greenWallMaterial->addTechnique(greenWallTechnique);
	greenWallMaterial->addAttribute("baseColor", Color(4, 153, 26), false);
	greenWallMaterial->addAttribute("roughness", 0.5f);
	greenWallMaterial->addAttribute("metallic", 0.5f); 
	greenWall->addComponent(new StaticMeshComponent(CubeMesh, greenWallMaterial));

	Actor* redWall = world->createActor("redWall", Transform(Vector3d(5.1, 0.0, 0.0), Vector3d(0.1, 5.0, 5.0), Matrix4d::IDENTITY));
	redWall->setMobility(Mobility::Static);
	Material* redWallMaterial = new Material("redWall");
	Technique* redWallTechnique = new Technique(shader);
	redWallMaterial->addTechnique(redWallTechnique);
	redWallMaterial->addAttribute("baseColor", LinearColor::Red, false);
	redWallMaterial->addAttribute("roughness", 0.5f);
	redWallMaterial->addAttribute("metallic", 0.5f);
	redWall->addComponent(new StaticMeshComponent(CubeMesh, redWallMaterial));

	Actor* whiteWall = world->createActor("whiteWall", Transform(Vector3d(0.0, 0.0, -5.1), Vector3d(5.0, 5.0, 0.1), Matrix4d::IDENTITY));
	whiteWall->setMobility(Mobility::Static);
	Material* whiteWallMaterial = new Material("whiteWall");
	Technique* whiteWallTechnique = new Technique(shader);
	whiteWallMaterial->addTechnique(whiteWallTechnique);
	//whiteWallMaterial->addAttribute("baseColor", Vector3f(0.9f, 0.9f, 0.9f));
	MaterialVector3AttributeBinding binding;
	binding.bind(test);
	whiteWallMaterial->bindAttribute("baseColor", binding);
	whiteWallMaterial->addAttribute("roughness", 0.5f);
	whiteWallMaterial->addAttribute("metallic", 0.5f);
	whiteWall->addComponent(new StaticMeshComponent(CubeMesh, whiteWallMaterial));

	Actor* roof = world->createActor("roof", Transform(Vector3d(0.0, 5.1, 0.0), Vector3d(5.0, 0.1, 5.0), Matrix4d::IDENTITY));
	roof->setMobility(Mobility::Static);
	Material* roofMaterial = new Material("roof");
	Technique* roofTechnique = new Technique(shader);
	roofMaterial->addTechnique(roofTechnique);
	roofMaterial->addAttribute("baseColor", Vector3f(0.9f, 0.9f, 0.9f));
	roofMaterial->addAttribute("roughness", 0.5f);
	roofMaterial->addAttribute("metallic", 0.5f);
	roof->addComponent(new StaticMeshComponent(CubeMesh, roofMaterial));

	Actor* floor = world->createActor("floor", Transform(Vector3d(0.0, -5.1, 0.0), Vector3d(5.0, 0.1, 5.0), Matrix4d::IDENTITY));
	floor->setMobility(Mobility::Static);
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

	Actor* leftBox = world->createActor("leftBox", Transform(Vector3d(-2.0, -5.1 + 2.8, -1.5), Vector3d(1.4, 2.8, 1.4), Matrix4d::createRotation(Vector3d::unitY(), 30.0)));
	leftBox->setMobility(Mobility::Static);
	Material* leftBoxMaterial = new Material("leftBox");
	Technique* leftBoxTechnique = new Technique(shader);
	leftBoxMaterial->addTechnique(leftBoxTechnique);
	leftBoxMaterial->addAttribute("baseColor", Vector3f(0.9f, 0.9f, 0.9f));
	leftBoxMaterial->addAttribute("roughness", 0.5f);
	leftBoxMaterial->addAttribute("metallic", 0.5f);
	leftBox->addComponent(new StaticMeshComponent(CubeMesh, leftBoxMaterial));

	Actor* rightBox = world->createActor("rightBox", Transform(Vector3d(2.8, -5.1 + 1.4, 1.5), Vector3d(1.4, 1.4, 1.4), Matrix4d::IDENTITY));
	rightBox->setMobility(Mobility::Static);
	Material* rightBoxMaterial = new Material("rightBox");
	Technique* rightBoxTechnique = new Technique(shader);
	rightBoxMaterial->addTechnique(rightBoxTechnique);
	rightBoxMaterial->addAttribute("baseColor", Vector3f(0.9f, 0.9f, 0.9f));
	rightBoxMaterial->addAttribute("roughness", 0.5f);
	rightBoxMaterial->addAttribute("metallic", 0.5f);
	rightBox->addComponent(new StaticMeshComponent(CubeMesh, rightBoxMaterial));

	Actor* transparentBox = world->createActor("transparentBox", Transform(Vector3d(2.0, 0.0, -2.0), Vector3d(1.0, 1.0, 1.0), Matrix4d::IDENTITY));
	transparentBox->setMobility(Mobility::Static);
	Material* transparentBoxMaterial = new Material("transparentBox");
	Shader transparentShader;
	transparentShader.createProgram(Shader::Vertex, "resources/cube_vert.glsl");
	transparentShader.createProgram(Shader::Fragment, "resources/transparent_cube_frag.glsl");
	Technique* transparentBoxTechnique = new Technique(transparentShader);
	transparentBoxMaterial->addTechnique(transparentBoxTechnique);
	transparentBoxMaterial->addAttribute("baseColor", Vector4f(0.1f, 0.1f, 0.7f, 0.3f));
	transparentBox->addComponent(new StaticMeshComponent(TransparentCubeMesh, transparentBoxMaterial));

	//lifetime destruction test
	//transparentBox->setLifetime(20.0);

	/*lightBox = world->createActor("lightBox", Transform(Vector3d(0.0, 4.0, 0.0), Vector3d(0.5, 0.5, 0.5), Matrix4d::IDENTITY));
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

	/*lightBox = world->createActor("lightBox", Transform(Vector3d(0.0, 4.0, 0.0), Vector3d(0.5, 0.5, 0.5), Matrix4d::IDENTITY));
	lightBox->setMobility(Mobility::Dynamic);
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

	Actor* staticlightBox = world->createActor("staticlightBox", Transform(Vector3d(0.0, 4.0, 0.0), Vector3d(0.5, 0.5, 0.5), Matrix4d::createRotation(Vector3d::unitX(), 90.0)));
	staticlightBox->setMobility(Mobility::Dynamic);
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
	staticPointLight->setIntensity(50.0);
	staticPointLight->setDynamicShadows(true);
	staticlightBox->addComponent(staticPointLight);

	SpotLightComponent* staticSpotLight = new SpotLightComponent();
	staticSpotLight->setIntensity(200.0);
	staticSpotLight->setLightColor(Color(255, 0, 0));
	staticlightBox->addComponent(staticSpotLight);

	Actor* staticlightBox2 = world->createActor("staticlightBox2", Transform(Vector3d(2.0, 4.0, 2.0), Vector3d(0.5, 0.5, 0.5), Matrix4d::createRotation(Vector3d::unitX(), 90.0)));
	staticlightBox2->setMobility(Mobility::Dynamic);
	Material* staticlightBoxMaterial2 = new Material("staticlightBox2");
	Technique* staticlightTechnique2 = new Technique(staticlightBoxShader);
	staticlightBoxMaterial2->addTechnique(staticlightTechnique2);
	staticlightBoxMaterial2->addAttribute("baseColor", Vector3f(0, 1, 0));
	staticlightBoxMaterial2->addAttribute("emissive", Vector3f(0, 1, 0));
	staticlightBox2->addComponent(new StaticMeshComponent(TransparentCubeMesh, staticlightBoxMaterial2));

	PointLightComponent* staticPointLight2 = new PointLightComponent();
	staticPointLight2->setIntensity(1.0);
	staticPointLight2->setSourceRadius(1.5);
	staticPointLight2->setDynamicShadows(true);
	staticPointLight2->setLightColor(Color(0, 255, 0));
	staticlightBox2->addComponent(staticPointLight2);


	/*Actor* areaLight = world->createActor("areaLight", Transform(Vector3d(-1.8, -5.1 + 1.4, 2.5), Vector3d(1.0, 0.01, 1.0), Matrix4d::IDENTITY));
	areaLight->setMobility(Mobility::Dynamic);
	Material* areaLightMaterial = new Material("areaLight");
	Technique* areaLightTechnique = new Technique(staticlightBoxShader);
	areaLightMaterial->addTechnique(areaLightTechnique);
	areaLightMaterial->addAttribute("baseColor", Vector3f(0, 0, 1));
	areaLightMaterial->addAttribute("emissive", Vector3f(0, 0, 1));
	areaLight->addComponent(new StaticMeshComponent(TransparentCubeMesh, areaLightMaterial));

	AreaLightComponent* areaLightComponent = new AreaLightComponent();
	areaLightComponent->setIntensity(100.0);
	areaLightComponent->setDynamicShadows(false);
	areaLightComponent->setLightColor(Color(0, 0, 255));
	areaLight->addComponent(areaLightComponent);*/

	//staticlightBox->addComponent(emitter);
	AE_DELETE(emitter);
	

	Actor* decal = world->createActor("decal", Transform(Vector3d(5.0, 0.0, 0.0), Vector3d(0.2, 4.0, 4.0), Matrix4d::IDENTITY));
	DecalProperties decalProperties;
	Texture::Parameters params;
	params.setMinFilter(TextureFilter::Linear);
	params.setMagFilter(TextureFilter::Linear);
	/*Image<uint8> decalImage;
	decalImage.init(ImageFormat::RGBA, 1, 1, Color(160, 160, 160, 255));
	//decalImage.init("resources/texture.png");
	Texture* decalTexture = Texture::create2D(Texture::RGBA, decalImage.getWidth(), decalImage.getHeight(), Texture::RGBA8, Texture::UnsignedByte, decalImage.getPixelsPtr(), params);
	decalProperties.setAlbedoTexture(decalTexture, 1.0);*/

	Image<uint8> decalNormals;
	decalNormals.init("resources/decalNormals.png");
	Texture* normalsTexture = Texture::create2D(Texture::RGBA, decalNormals.getWidth(), decalNormals.getHeight(), Texture::RGBA8, Texture::UnsignedByte, decalNormals.getPixelsPtr(), params);
	decalProperties.setNormalsTexture(normalsTexture, 1.0);

	/*Image<uint8> metallic;
	metallic.init(ImageFormat::RGBA, 1, 1, Color(200, 200, 200, 255));
	Texture* metallicTexture = Texture::create2D(Texture::RGBA, metallic.getWidth(), metallic.getHeight(), Texture::RGBA8, Texture::UnsignedByte, metallic.getPixelsPtr(), params);
	decalProperties.setMetallicTexture(metallicTexture, 1.0);
	*/
	//decalTexture->release();
	DecalComponent* decalComponent = new DecalComponent(decalProperties);
	decalComponent->addDecal(Transform(Vector3d(0.0, 0.0, -5.0), Vector3d(4.0, 4.0, 0.2), Matrix4d::IDENTITY), Color(255, 255, 255, 255), 0.5);
	decalComponent->addDecal(Transform(Vector3d(3.0, 0.0, -5.0), Vector3d(1.0, 1.0, 0.2), Matrix4d::IDENTITY));
	decalComponent->addDecal(Transform(Vector3d(3.0, 3.0, -5.0), Vector3d(1.0, 1.0, 0.2), Matrix4d::IDENTITY));
	decalComponent->addDecal(Transform(Vector3d(-3.0, 3.0, -5.0), Vector3d(1.0, 1.0, 0.2), Matrix4d::IDENTITY), Color(255, 255, 255, 255), 1.0, -1.0, Vector2f(0.0f, 0.0f), Vector2f(0.4296875f, 0.4296875f));
	//decalComponent->addDecal(Transform(Vector3d(-3.0, 0.0, -5.0), Vector3d(1.0, 1.0, 0.2), Matrix4d::IDENTITY), Color(255, 255, 255, 255), 1.0, 20.0, Vector2f(0.0f, 0.0f), Vector2f(0.4296875f, 0.4296875f));
	decalComponent->addDecal(Transform(Vector3d(-2.2, -5.1+2.8, -0.25), Vector3d(2.0, 2.0, 2.0), Matrix4d::IDENTITY), Color(255, 255, 255, 255), 1.0, -1.0);
	decalComponent->addDecal(Transform(Vector3d(1.4, -5.1 + 2.8, 1.5), Vector3d(0.5, 0.5, 0.5), Matrix4d::createRotation(Vector3d::unitX(), 45.0) * Matrix4d::createRotation(Vector3d::unitY(), 90.0)));

	decal->addComponent(decalComponent);

	//Skybox

	/*int num = 3;
	
	std::string files[6] =
	{
		"resources/skybox/skybox_" + std::to_string(num) + "_right1.png",
		"resources/skybox/skybox_" + std::to_string(num) + "_left2.png",
		"resources/skybox/skybox_" + std::to_string(num) + "_top3.png",
		"resources/skybox/skybox_" + std::to_string(num) + "_bottom4.png",
		"resources/skybox/skybox_" + std::to_string(num) + "_front5.png",
		"resources/skybox/skybox_" + std::to_string(num) + "_back6.png"
	};

	Texture* sky = TextureUtils::createImageCubeTexture(files, true);

	SkyboxActor* skybox = world->createActor<SkyboxActor>("skybox", Transform());
	skybox->setTexture(sky);
	skybox->setEmissiveThreshold(2.0f);

	sky->release();*/
}


void createLargeBox(World* world)
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

	if (!Spaceship)
	{
		StaticMesh::Properties properties;
		properties.isEnvironmentMesh = false;
		properties.isTransparent = false;
		properties.LightMapResolution = 0;
		properties.LightProperties.CastsDynamicShadow = false;
		properties.RenderState.setCullEnabled(true);
		properties.RenderState.setCullFaceSide(RenderState::Back);
		properties.RenderState.setDepthTestEnabled(true);
		properties.RenderState.setBlendEnabled(false);
		Spaceship = new StaticMesh("resources/ship.mesh", properties);
	}

	if (!Monster)
	{
		StaticMesh::Properties properties;
		properties.isEnvironmentMesh = false;
		properties.isTransparent = false;
		properties.LightMapResolution = 0;
		properties.LightProperties.CastsDynamicShadow = false;
		properties.RenderState.setCullEnabled(true);
		properties.RenderState.setCullFaceSide(RenderState::Back);
		properties.RenderState.setDepthTestEnabled(true);
		properties.RenderState.setBlendEnabled(false);
		Monster = new StaticMesh("resources/monster.mesh", properties);
	}

	Shader shader;
	shader.createProgram(Shader::Vertex, "resources/cube_vert.glsl");
	shader.createProgram(Shader::Fragment, "resources/ftl_cube_frag.glsl");

	Actor* leftWall = world->createActor("leftWall", Transform(Vector3d(-100.1, 0.0, 0.0), Vector3d(0.1, 10.0, 100.0), Matrix4d::IDENTITY));
	leftWall->setMobility(Mobility::Static);
	Material* leftWallMaterial = new Material("leftWall");
	Technique* leftWallTechnique = new Technique(shader);
	leftWallMaterial->addTechnique(leftWallTechnique);
	leftWallMaterial->addAttribute("baseColor", Color(128, 128, 128), false);
	leftWallMaterial->addAttribute("roughness", 0.5f);
	leftWallMaterial->addAttribute("metallic", 0.5f);
	leftWall->addComponent(new StaticMeshComponent(CubeMesh, leftWallMaterial));

	Actor* rightWall = world->createActor("rightWall", Transform(Vector3d(100.1, 0.0, 0.0), Vector3d(0.1, 10.0, 100.0), Matrix4d::IDENTITY));
	rightWall->setMobility(Mobility::Static);
	Material* rightWallMaterial = new Material("rightWall");
	Technique* righWallTechnique = new Technique(shader);
	rightWallMaterial->addTechnique(righWallTechnique);
	rightWallMaterial->addAttribute("baseColor", LinearColor::Gray, false);
	rightWallMaterial->addAttribute("roughness", 0.5f);
	rightWallMaterial->addAttribute("metallic", 0.5f);
	rightWall->addComponent(new StaticMeshComponent(CubeMesh, rightWallMaterial));

	Actor* frontWall = world->createActor("frontWall", Transform(Vector3d(0.0, 0.0, -100.1), Vector3d(100.0, 10.0, 0.1), Matrix4d::IDENTITY));
	frontWall->setMobility(Mobility::Static);
	Material* frontWallMaterial = new Material("frontWall");
	Technique* frontWallTechnique = new Technique(shader);
	frontWallMaterial->addTechnique(frontWallTechnique);
	//whiteWallMaterial->addAttribute("baseColor", Vector3f(0.9f, 0.9f, 0.9f));
	MaterialVector3AttributeBinding binding;
	binding.bind(test);
	frontWallMaterial->bindAttribute("baseColor", binding);
	frontWallMaterial->addAttribute("roughness", 0.5f);
	frontWallMaterial->addAttribute("metallic", 0.5f);
	frontWall->addComponent(new StaticMeshComponent(CubeMesh, frontWallMaterial));

	Actor* backWall = world->createActor("backWall", Transform(Vector3d(0.0, 0.0, 100.1), Vector3d(100.0, 10.0, 0.1), Matrix4d::IDENTITY));
	backWall->setMobility(Mobility::Static);
	Material* backWallMaterial = new Material("backWall");
	Technique* backWallTechnique = new Technique(shader);
	backWallMaterial->addTechnique(backWallTechnique);
	backWallMaterial->addAttribute("baseColor", LinearColor::Gray, false);
	backWallMaterial->addAttribute("roughness", 0.5f);
	backWallMaterial->addAttribute("metallic", 0.5f);
	backWall->addComponent(new StaticMeshComponent(CubeMesh, backWallMaterial));

	Actor* floor = world->createActor("floor", Transform(Vector3d(0.0, -10.1, 0.0), Vector3d(100.0, 0.1, 100.0), Matrix4d::IDENTITY));
	floor->setMobility(Mobility::Static);
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

	Actor* staticlightBox = world->createActor("staticlightBox", Transform(Vector3d(0.0, 120.0, 0.0), Vector3d(0.5, 0.5, 0.5), Matrix4d::IDENTITY));
	staticlightBox->setMobility(Mobility::Dynamic);
	Material* staticlightBoxMaterial = new Material("staticlightBox");
	Shader staticlightBoxShader;
	staticlightBoxShader.createProgram(Shader::Vertex, "resources/cube_vert.glsl");
	staticlightBoxShader.createProgram(Shader::Fragment, "resources/light_box_frag.glsl");
	Technique* staticlightTechnique = new Technique(staticlightBoxShader);
	staticlightBoxMaterial->addTechnique(staticlightTechnique);
	staticlightBoxMaterial->addAttribute("baseColor", Vector3f(1.0, 1.0, 1.0));
	staticlightBoxMaterial->addAttribute("emissive", Vector3f(1.0, 1.0, 1.0));
	staticlightBox->addComponent(new StaticMeshComponent(TransparentCubeMesh, staticlightBoxMaterial));

	//DirectionalLightComponent* staticDirLight = new DirectionalLightComponent();
	//staticDirLight->getLocalRelativeTransform().rotateX(-45);
	//staticDirLight->setIntensity(5.0);
	//staticlightBox->addComponent(staticDirLight);
	PointLightComponent* staticPointLight = new PointLightComponent();
	staticPointLight->setIntensity(100000.0);
	staticPointLight->setLightColor(Color(255, 255, 255));
	staticlightBox->addComponent(staticPointLight);

	//spaceship
	Actor* spaceship = world->createActor("spaceship", Transform(Vector3d(0.0, 100.0, 100.1), Vector3d::one(), Matrix4d::IDENTITY));
	spaceship->setMobility(Mobility::Static);
	Shader spaceshipShader;
	spaceshipShader.createProgram(Shader::Vertex, "resources/cube_vert.glsl");
	spaceshipShader.createProgram(Shader::Fragment, "resources/spaceship_frag.glsl");
	MaterialMap* spaceshipMaterial = new MaterialMap("spaceship");
	Technique* spaceshipTechnique1 = new Technique(spaceshipShader);
	spaceshipMaterial->addTechnique(spaceshipTechnique1);
	Technique* spaceshipTechnique2 = new Technique(spaceshipShader);
	spaceshipMaterial->addTechnique(spaceshipTechnique2);

	Image<uint8> image;
	Texture::Parameters params;
	params.setMinFilter(TextureFilter::Linear);
	params.setMagFilter(TextureFilter::Linear);
	params.setWrapS(TextureWrap::Repeat);
	params.setWrapT(TextureWrap::Repeat);

	image.init("resources/spaceship/metalplate/Metal_Plate_021_basecolor.png");
	Texture* baseColor = Texture::create2D(Texture::RGBA, image.getWidth(), image.getHeight(), Texture::RGBA8, Texture::UnsignedByte, image.getPixelsPtr(), params, true);
	image.init("resources/spaceship/metalplate/Metal_Plate_021_ao.png");
	Texture* ao = Texture::create2D(Texture::RGBA, image.getWidth(), image.getHeight(), Texture::RGBA8, Texture::UnsignedByte, image.getPixelsPtr(), params, true);
	image.init("resources/spaceship/metalplate/Metal_Plate_021_normal.png");
	Texture* normals = Texture::create2D(Texture::RGBA, image.getWidth(), image.getHeight(), Texture::RGBA8, Texture::UnsignedByte, image.getPixelsPtr(), params, true);
	image.init("resources/spaceship/metalplate/Metal_Plate_021_roughness.png");
	Texture* roughness = Texture::create2D(Texture::RGBA, image.getWidth(), image.getHeight(), Texture::RGBA8, Texture::UnsignedByte, image.getPixelsPtr(), params, true);
	image.init("resources/spaceship/metalplate/Metal_Plate_021_metallic.png");
	Texture* metallic = Texture::create2D(Texture::RGBA, image.getWidth(), image.getHeight(), Texture::RGBA8, Texture::UnsignedByte, image.getPixelsPtr(), params, true);

	image.init("resources/spaceship/bluemetalplate/Metal_Plate_022a_basecolor.png");
	Texture* baseColor1 = Texture::create2D(Texture::RGBA, image.getWidth(), image.getHeight(), Texture::RGBA8, Texture::UnsignedByte, image.getPixelsPtr(), params, true);
	image.init("resources/spaceship/bluemetalplate/Metal_Plate_022a_ao.png");
	Texture* ao1 = Texture::create2D(Texture::RGBA, image.getWidth(), image.getHeight(), Texture::RGBA8, Texture::UnsignedByte, image.getPixelsPtr(), params, true);
	image.init("resources/spaceship/bluemetalplate/Metal_Plate_022a_normal.png");
	Texture* normals1 = Texture::create2D(Texture::RGBA, image.getWidth(), image.getHeight(), Texture::RGBA8, Texture::UnsignedByte, image.getPixelsPtr(), params, true);
	image.init("resources/spaceship/bluemetalplate/Metal_Plate_022a_roughness.png");
	Texture* roughness1 = Texture::create2D(Texture::RGBA, image.getWidth(), image.getHeight(), Texture::RGBA8, Texture::UnsignedByte, image.getPixelsPtr(), params, true);
	image.init("resources/spaceship/bluemetalplate/Metal_Plate_022a_metallic.png");
	Texture* metallic1 = Texture::create2D(Texture::RGBA, image.getWidth(), image.getHeight(), Texture::RGBA8, Texture::UnsignedByte, image.getPixelsPtr(), params, true);

	spaceshipMaterial->addAttribute("baseColor", baseColor);
	spaceshipMaterial->addAttribute("roughness", roughness);
	spaceshipMaterial->addAttribute("metallic", metallic);
	spaceshipMaterial->addAttribute("metallicOffset", 0.2f);//0.1f
	spaceshipMaterial->addAttribute("normals", normals);
	spaceshipMaterial->addAttribute("ao", ao);
	spaceshipMaterial->addAttribute("normalScale", 1.0f);
	spaceshipMaterial->addAttribute("materialScale", 0.2f);
	spaceshipMaterial->addAttribute("offset", Vector2f(0.5, 0.5));

	spaceshipMaterial->addAttribute("baseColor", baseColor1, 1);
	spaceshipMaterial->addAttribute("roughness", roughness1, 1);
	spaceshipMaterial->addAttribute("metallic", metallic1, 1);
	spaceshipMaterial->addAttribute("metallicOffset", 0.0f, 1);
	spaceshipMaterial->addAttribute("normals", normals1, 1);
	spaceshipMaterial->addAttribute("ao", ao1, 1);
	spaceshipMaterial->addAttribute("normalScale", 1.0f, 1);
	spaceshipMaterial->addAttribute("materialScale", 0.06f, 1);
	spaceshipMaterial->addAttribute("offset", Vector2f::zero(), 1);

	spaceshipMaterial->addTechniqueMapping(0, 1);
	spaceshipMaterial->addTechniqueMapping(1, 1);
	spaceshipMaterial->addTechniqueMapping(2, 1);
	spaceshipMaterial->addTechniqueMapping(3, 1);
	spaceshipMaterial->addTechniqueMapping(4, 0);
	spaceshipMaterial->addTechniqueMapping(5, 0);
	spaceshipMaterial->addTechniqueMapping(6, 1);
	spaceshipMaterial->addTechniqueMapping(7, 1);
	spaceshipMaterial->addTechniqueMapping(8, 1);
	spaceshipMaterial->addTechniqueMapping(9, 1);
	spaceshipMaterial->addTechniqueMapping(10, 0);
	spaceshipMaterial->addTechniqueMapping(11, 0);
	spaceshipMaterial->addTechniqueMapping(12, 0);
	spaceshipMaterial->addTechniqueMapping(13, 0);
	spaceshipMaterial->addTechniqueMapping(14, 0);
	spaceshipMaterial->addTechniqueMapping(15, 0);
	spaceshipMaterial->addTechniqueMapping(16, 0);
	spaceshipMaterial->addTechniqueMapping(17, 0);
	spaceshipMaterial->addTechniqueMapping(18, 0);
	spaceshipMaterial->addTechniqueMapping(19, 0);
	spaceshipMaterial->addTechniqueMapping(20, 0);
	spaceshipMaterial->addTechniqueMapping(21, 0);
	spaceshipMaterial->addTechniqueMapping(22, 0);
	spaceshipMaterial->addTechniqueMapping(23, 0);
	spaceshipMaterial->addTechniqueMapping(24, 0);
	spaceshipMaterial->addTechniqueMapping(25, 0);
	spaceshipMaterial->addTechniqueMapping(26, 0);
	spaceshipMaterial->addTechniqueMapping(27, 0);
	spaceshipMaterial->addTechniqueMapping(28, 0);
	spaceshipMaterial->addTechniqueMapping(29, 0);
	spaceshipMaterial->addTechniqueMapping(30, 0);
	spaceshipMaterial->addTechniqueMapping(31, 0);
	spaceshipMaterial->addTechniqueMapping(32, 0);
	spaceshipMaterial->addTechniqueMapping(33, 0);
	spaceshipMaterial->addTechniqueMapping(34, 0);
	spaceshipMaterial->addTechniqueMapping(35, 1);
	spaceshipMaterial->addTechniqueMapping(36, 1);
	spaceshipMaterial->addTechniqueMapping(37, 1);
	spaceshipMaterial->addTechniqueMapping(38, 1);
	spaceship->addComponent(new StaticMeshComponent(Spaceship, spaceshipMaterial));

	//monster
	Actor* monster = world->createActor("monster", Transform(Vector3d(0.0, 10.0, -40.0), Vector3d::one(), Matrix4d::createRotation(Vector3d::unitY(), -90.0)));
	monster->setMobility(Mobility::Static);
	Shader monsterShader;
	monsterShader.createProgram(Shader::Vertex, "resources/monster_vert.glsl");
	monsterShader.createProgram(Shader::Fragment, "resources/monster_frag.glsl");
	MaterialMap* monsterMaterial = new MaterialMap("monster");
	Technique* monsterTechniqueBlack = new Technique(monsterShader);
	monsterMaterial->addTechnique(monsterTechniqueBlack);
	Technique* monsterTechniqueYellow = new Technique(monsterShader);
	monsterMaterial->addTechnique(monsterTechniqueYellow);
	Technique* monsterTechniqueRed = new Technique(monsterShader);
	monsterMaterial->addTechnique(monsterTechniqueRed);
	Technique* monsterTechniqueWhite = new Technique(monsterShader);
	monsterMaterial->addTechnique(monsterTechniqueWhite);
	Technique* monsterTechniqueIndicator = new Technique(monsterShader);
	monsterMaterial->addTechnique(monsterTechniqueIndicator);

	monsterMaterial->addAttribute("baseColor", Colors::Black, false);
	monsterMaterial->addAttribute("baseColor", Colors::Yellow, false, 1);
	monsterMaterial->addAttribute("baseColor", Colors::Red, false, 2);
	monsterMaterial->addAttribute("baseColor", Colors::White, false, 3);
	monsterMaterial->addAttribute("baseColor", Colors::DarkPurple, false, 4);

	monsterMaterial->addTechniqueMapping(0, 1);//body
	monsterMaterial->addTechniqueMapping(1, 3);//outer eye
	monsterMaterial->addTechniqueMapping(2, 0);//inner eye
	monsterMaterial->addTechniqueMapping(3, 0);//mouth
	monsterMaterial->addTechniqueMapping(4, 3);//tooth
	monsterMaterial->addTechniqueMapping(5, 2);//nose
	monsterMaterial->addTechniqueMapping(6, 2);//nose
	monsterMaterial->addTechniqueMapping(7, 0);//tophat
	monsterMaterial->addTechniqueMapping(8, 0);//tophat
	monsterMaterial->addTechniqueMapping(9, 0);//leg
	monsterMaterial->addTechniqueMapping(10, 0);//leg
	monsterMaterial->addTechniqueMapping(11, 1);
	monsterMaterial->addTechniqueMapping(12, 1);
	monsterMaterial->addTechniqueMapping(13, 0);
	monsterMaterial->addTechniqueMapping(14, 0);
	monsterMaterial->addTechniqueMapping(15, 1);
	monsterMaterial->addTechniqueMapping(16, 1);
	monsterMaterial->addTechniqueMapping(17, 3);

	monster->addComponent(new StaticMeshComponent(Monster, monsterMaterial));

	//Skybox

	int num = 3;

	std::string files[6] =
	{
		"resources/skybox/skybox_" + std::to_string(num) + "_right1.png",
		"resources/skybox/skybox_" + std::to_string(num) + "_left2.png",
		"resources/skybox/skybox_" + std::to_string(num) + "_top3.png",
		"resources/skybox/skybox_" + std::to_string(num) + "_bottom4.png",
		"resources/skybox/skybox_" + std::to_string(num) + "_front5.png",
		"resources/skybox/skybox_" + std::to_string(num) + "_back6.png"
	};

	Texture* sky = TextureUtils::createImageCubeTexture(files, true);

	SkyboxActor* skybox = world->createActor<SkyboxActor>("skybox", Transform());
	skybox->setTexture(sky);
	skybox->setEmissiveThreshold(2.0f);

	sky->release();
}