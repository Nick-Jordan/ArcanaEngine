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
#include "Key.h"-
#include "NoDataEvents.h"
#include "Globals.h"
#include "GlobalShaders.h"
#include "Curve.h"

#include "MeshComponent.h"
#include "CameraComponent.h"
#include "ResourceManager.h"
#include "Input.h"
#include "MeshRenderProcedure.h"
#include "MeshParticleEmitterComponent.h"
#include "ParticleEmitterComponent.h"
#include "DynamicField.h"
#include "FPSCharacter.h"

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
		else if (event.getInt("keyCode") == KeyCode::Escape || event.getInt("keyCode") == KeyCode::ControllerSpecialRight)
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

Curve curve;

Vector3d gravity(double x, double y, double z)
{
	/*Vector3d vec = Vector3d(x, y, z);
	double mag = vec.magnitude();

	if (mag <= 1.0)
		return Vector3d::zero();

	return -vec / pow(mag, 3) * 1000;*/

	return Vector3d(0, -9.8, 0);
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
	GEngine->setStationaryCursor(true);

	World* world = new World("world");

	FPSCharacter* camera = world->createActor<FPSCharacter>("camera", Transform(Vector3d(0.0, 0.0, 0.0), Vector3d::one(), Matrix4d::IDENTITY));
	CameraComponent* cameraComponent = new CameraComponent(90.0f, GEngine->getApplicationInstance()->getActiveWindow().getAspectRatio(), 0.1, 1000.0);
	cameraComponent->setPosition(Vector3d(0.0, 0.0, 2.0));
	camera->addComponent(cameraComponent);

	Actor* particles = world->createActor("particles", Transform(Vector3d(0.0, 0.0, 0.0), Vector3d::one(), Matrix4d::IDENTITY));
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

	curve = Curve(Curve::InterpolationMethod::BSpline);
	curve.addPoint(Vector4d(0, 0, 0, 0));
	curve.addPoint(Vector4d(5, 0, 2, 0));
	curve.addPoint(Vector4d(5, 2, 4, 0));
	curve.addPoint(Vector4d(2, -2, 10, 0));
	curve.addPoint(Vector4d(-2, -1, 12, 0));
	curve.addPoint(Vector4d(-3, 6, 10, 0));
	curve.addPoint(Vector4d(3, 4, 4, 0));
	curve.addPoint(Vector4d(0, 0, 0, 0));

	ParticleEmitterProperties properties;
	properties.emissionRate = 100;
	properties.ellipsoid = false;
	properties.sizeStartMin = 0.2f;
	properties.sizeStartMax = 0.5f;
	properties.sizeEndMin = 0.1f;
	properties.sizeEndMax = 0.4f;
	properties.energyMin = 1000.0;
	properties.energyMax = 2000.0;
	properties.colorStart = Vector4f(0.0, 0.0, 1.0, 1.0);
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
	properties.velocityVar = Vector3f(3.2, 0.0, 3.2);
	properties.positionVar = Vector3f::zero();
	properties.frameRandomOffset = 0;
	properties.orbitPosition = false;
	properties.orbitVelocity = false;
	properties.orbitAcceleration = true;
	properties.useVelocityField = false;
	properties.useAccelerationField = false;

	//MeshParticleEmitterComponent* particleEmitter = new MeshParticleEmitterComponent(mesh, 20000, properties, *GlobalShaders::get(GlobalShaders::MeshParticles));
	ParticleEmitterComponent* particleEmitter = new ParticleEmitterComponent(20000, properties, *GlobalShaders::get(GlobalShaders::Particles));

	Image<uint8> image;
	image.init("resources/arcana/textures/particles/particle17.png");
	Texture* texture = Texture::create2D(Texture::RGBA, 64, 64, Texture::RGBA8, Texture::UnsignedByte, image.getPixelsPtr());
	particleEmitter->setTexture(texture);
	AE_RELEASE(texture);

	//DynamicVectorField* field = new DynamicVectorField(gravity);
	//particleEmitter->setAccelerationVectorField(field);
	//particleEmitter->setVelocityCurve(&curve);
	particleEmitter->setOrbitAcceleration(Vector3d(0, -10.0, 0));
	particleEmitter->setOrbitVelocity(Vector3d(0, 10.0, 0));
	particleEmitter->start();

	particles->addComponent(particleEmitter);

	GEngine->setWorld(world);

	//FASTER_THAN_LIGHT(world);

	GEngine->start();
	GEngine->exit();

	//AE_DELETE(field);

	DestroyEngine();

	return 1;
}