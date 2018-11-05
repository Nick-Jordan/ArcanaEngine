// TextureClient.cpp : Defines the entry point for the application.
//
#include "stdafx.h"
#include "TextureClient.h"

#include "Application.h"

#include "WindowsWindowDefinition.h"
#include "WindowsApplicationDefinition.h"

#include "Renderer.h"
#include "Globals.h"
#include "Texture.h"
#include "Image.h"

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
	appDefinition.setAppName("Texture Client");
	appDefinition.setWindowClass(L"TEXTURE_CLIENT");
	appDefinition.setInstance(hInstance);
	appDefinition.setCommandLineArgs(lpCmdLine);
	appDefinition.setShowCommand(nCmdShow);
	appDefinition.addWindowDefinition(windowDef);

	GEngine->createApplication(appDefinition);

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

	Image<uint8> image;
	//image.init("resources/texture.png");
	image.init(ImageFormat::RGBA, 256, 256, Vector4<uint8>(255, 0, 0, 255));
	image.save("resources/saved_texture.png");

	Texture* texture = Texture::createDefault();
	//Texture* texture = Texture::create1D(Texture::RGBA, 255, Texture::RGBA8, Texture::UnsignedByte, pixels);

	glBindTexture(texture->getType(), texture->getId());
	uint8* data = new uint8[texture->getWidth() * texture->getHeight() * texture->getComponents()];
	glGetTexImage(texture->getType(), 0, texture->getFormat(), texture->getPixelType(), data);

	Image<uint8> imageFromTexture;
	imageFromTexture.init(ImageFormat::RGBA, texture->getWidth(), texture->getHeight(), data);
	imageFromTexture.save("resources/image_from_texture.png");

	texture->release();

	AE_DELETE_ARRAY(data);

	GEngine->setWorld(world);

	GEngine->start();
	GEngine->exit();

	DestroyEngine();

	return 1;
}