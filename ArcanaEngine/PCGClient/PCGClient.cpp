// PCGClient.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "ProceduralTexture.h"

#include "Renderer.h"

using namespace Arcana;

class TestProceduralObject : public ProceduralObject
{
public:

	TestProceduralObject() : ProceduralObject("TestProceduralObject", "test")
	{

	}
};

int main()
{
	//context
	RenderSettings settings;
	settings.bitsPerPixel = 32;
	settings.depthBits = 8;
	settings.stencilBits = 8;
	settings.antialiasingLevel = 4;
	settings.majorVersion = 4;
	settings.minorVersion = 5;
	settings.attributeFlags = RenderSettings::Default;
	settings.sRgb = false;
	Renderer renderer(settings, 256, 256);
	//context


	TestProceduralObject* test = new TestProceduralObject();
	test->generate(Seed());
	delete test;

	Shader shader;
	shader.createProgram(Shader::Vertex, "resources/shader_vert.glsl");
	shader.createProgram(Shader::Fragment, "resources/shader_frag.glsl");
	ProceduralTexture* texture = new ProceduralTexture(shader, Texture::RGBA, 128, 128, Texture::RGBA8, Texture::UnsignedByte);
	texture->generate(ProceduralParameters(), Seed());
	Texture* t = texture->get();

	glBindTexture(t->getType(), t->getId());
	uint8* renderTextureData = new uint8[t->getWidth() * t->getHeight() * t->getComponents()];
	glGetTexImage(t->getType(), 0, t->getFormat(), t->getPixelType(), renderTextureData);

	Image<uint8> image;
	image.init(ImageFormat::RGBA, t->getWidth(), t->getHeight(), renderTextureData);
	image.save("resources/procedural_texture.png");

	AE_DELETE_ARRAY(renderTextureData);

	delete texture;
}
