// FramebufferClient.cpp : Defines the entry point for the application.
//
#include "stdafx.h"
#include "FramebufferClient.h"

#include "Application.h"

#include "WindowsWindowDefinition.h"
#include "WindowsApplicationDefinition.h"

#include "Renderer.h"
#include "Globals.h"
#include "Texture.h"
#include "Image.h"
#include "Framebuffer.h"
#include "TextureAttachment.h"
#include "DepthStencilAttachment.h"

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

void drawQuad(Shader& shader)
{
	float quadVertices[] = {

		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		1.0f, -1.0f,  1.0f, 0.0f,
		1.0f,  1.0f,  1.0f, 1.0f
	};

	unsigned int quadVAO, quadVBO;
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	shader.bind();

	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	WindowsWindowDefinition windowDef;
	windowDef.setWidth(800);
	windowDef.setHeight(600);
	windowDef.setStyle(Style::Default);

	WindowsApplicationDefinition appDefinition;
	appDefinition.setAppName("Framebuffer Client");
	appDefinition.setWindowClass(L"FRAMEBUFFER_CLIENT");
	appDefinition.setInstance(hInstance);
	appDefinition.setCommandLineArgs(lpCmdLine);
	appDefinition.setShowCommand(nCmdShow);
	appDefinition.addWindowDefinition(windowDef);

	Application app = Application(appDefinition);

	RenderSettings settings;
	settings.bitsPerPixel = 32;
	settings.depthBits = 8;
	settings.stencilBits = 8;
	settings.antialiasingLevel = 4;
	settings.majorVersion = 4;
	settings.minorVersion = 5;
	settings.attributeFlags = RenderSettings::Default;
	settings.sRgb = false;
	Renderer renderer(settings, &app.getActiveWindow());

	//framebuffer test

	Shader shader;
	shader.createProgram(Shader::Vertex, "resources/test_shader_vert.glsl");
	shader.createProgram(Shader::Fragment, "resources/test_shader_frag.glsl");

	Texture* renderTexture = Texture::create2D(Texture::RGBA, 256, 256, Texture::RGBA8, Texture::UnsignedByte, nullptr);
	Framebuffer* framebuffer = new Framebuffer("framebuffer");
	framebuffer->addAttachment(new TextureAttachment("texture", renderTexture));
	framebuffer->addAttachment(new DepthStencilAttachment("depth_stencil", DepthStencilAttachment::DepthStencil, 256, 256));

	GLenum buffers[1] = {Framebuffer::Color0};
	framebuffer->setDrawBuffers(buffers, 1);

	Framebuffer* prev = framebuffer->bind();

	drawQuad(shader);

	Framebuffer::bind(prev);

	glBindTexture(renderTexture->getType(), renderTexture->getId());
	uint8* renderTextureData = new uint8[renderTexture->getWidth() * renderTexture->getHeight() * renderTexture->getComponents()];
	glGetTexImage(renderTexture->getType(), 0, renderTexture->getFormat(), renderTexture->getPixelType(), renderTextureData);

	Image<uint8> framebufferImage;
	framebufferImage.init(ImageFormat::RGBA, 256, 256, renderTextureData);
	framebufferImage.save("resources/framebuffer_test_texture.png");

	AE_DELETE(framebuffer);
	AE_DELETE_ARRAY(renderTextureData);
	AE_RELEASE(renderTexture);


	Framebuffer* framebuffer2 = new Framebuffer("framebuffer2");
	framebuffer2->addAttachment(new RenderbufferObjectAttachment("renderbuffer", Texture::RGBA8, 256, 256));
	framebuffer2->addAttachment(new RenderbufferObjectAttachment("renderbuffer2", Texture::RGBA8, 256, 256));

	framebuffer2->setDrawBuffers(buffers);

	prev = framebuffer2->bind();

	drawQuad(shader);

	Framebuffer::bind(prev);

	uint8* bufferData = new uint8[256 * 256 * 4];
	framebuffer2->readPixels(Framebuffer::Color0, 0, 0, 256, 256, GL_RGBA, GL_UNSIGNED_BYTE, bufferData);

	framebufferImage.init(ImageFormat::RGBA, 256, 256, bufferData);
	framebufferImage.save("resources/framebuffer_test_renderbuffer.png");

	Shader shader2;
	shader2.createProgram(Shader::Vertex, "resources/test_shader_vert.glsl");
	shader2.createProgram(Shader::Fragment, "resources/test_shader_frag2.glsl");

	framebuffer2->setDrawBuffer(Framebuffer::Color1);

	prev = framebuffer2->bind();

	drawQuad(shader2);

	Framebuffer::bind(prev);

	uint8* bufferData2 = new uint8[256 * 256 * 4];
	framebuffer2->readPixels(Framebuffer::Color1, 0, 0, 256, 256, GL_RGBA, GL_UNSIGNED_BYTE, bufferData2);

	framebufferImage.init(ImageFormat::RGBA, 256, 256, bufferData2);
	framebufferImage.save("resources/framebuffer_test_renderbuffer2.png");

	AE_DELETE(framebuffer2);
	AE_DELETE_ARRAY(bufferData);
	AE_DELETE_ARRAY(bufferData2);

	GLenum error = glGetError();
	LOGF(Error, CoreEngine, "Error: %d", error);

	return 1;
}