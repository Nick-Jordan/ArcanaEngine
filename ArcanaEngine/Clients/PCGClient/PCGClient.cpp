// PCGClient.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "ProceduralTexture.h"

#include "Renderer.h"

#include "Noise.h"
#include "CellularAutomata.h"
#include "MarkovChain.h"
#include "LSystem.h"

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

	/*float planetTerrain(vec3 position)
	{
		position = position / 1000.0;

		float tinyDetail = noise(position, 9, 0.15, 0.8, 0, 50.0);
		float smallDetail = noise(position, 6, 0.05, 0.8, 0, 100.0);
		float largeDetail = noise(position, 8, 0.003, 0.8, -2000, 3000.0);
		float n = 10 + tinyDetail + smallDetail + largeDetail;
		float mountains = clamp(noiseCubed(position, 7, 0.002, 0.7, -13, 13), 0, 1)
			* (cellularSquared(position.xyz, 3, 0.05, 0.6, -25000.0, 25000.0)
				+ ridgedNoise(position, 11, 0.03, 0.5, 0.0, 7500.0));
		float plateaus = clamp(noiseCubed(position, 6, 0.003, 0.6, -25, 25), 0, 1)
			* (clamp(noise(position, 5, 0.08, 0.55, -13000.0, 8000.0), 0.0, 1500.0)
				+ clamp(noise(position, 5, 0.1, 0.6, -7000.0, 5000.0), 0.0, 750.0));
		float oceans = clamp(noise(position, 6, 0.00015, 0.75, -5.0, 7.0), 0.0, 1.0)
			* noise(position, 6, 0.002, 0.9, 10000.0, 20000.0);

		n += mountains;
		n += plateaus;
		n -= oceans;

		return n;
	}*/


	///Noise Test

	Noise::FunctionProperties tinyDetail;
	tinyDetail.octaves = 9;
	tinyDetail.persistence = 0.8;
	tinyDetail.frequency = 0.15;
	tinyDetail.low = 0.0;
	tinyDetail.high = 50;

	Noise::FunctionProperties smallDetail;
	smallDetail.octaves = 6;
	smallDetail.persistence = 0.8;
	smallDetail.frequency = 0.05;
	smallDetail.low = 0.0;
	smallDetail.high = 100.0;

	Noise::FunctionProperties largeDetail;
	largeDetail.octaves = 8;
	largeDetail.persistence = 0.8;
	tinyDetail.frequency = 0.003;
	largeDetail.low = -2000.0;
	largeDetail.high = 3000.0;


	Noise::FunctionProperties mountainsCellSquared;
	mountainsCellSquared.function = Noise::CellularSquaredNoise;
	mountainsCellSquared.octaves = 3;
	mountainsCellSquared.persistence = 0.6;
	mountainsCellSquared.frequency = 0.05;
	mountainsCellSquared.low = -25000.0;
	mountainsCellSquared.high = 25000.0;

	Noise::FunctionProperties mountainsRidged;
	mountainsRidged.function = Noise::RidgedNoise;
	mountainsRidged.octaves = 11;
	mountainsRidged.persistence = 0.5;
	mountainsRidged.frequency = 0.03;
	mountainsRidged.low = 0.0;
	mountainsRidged.high = 7500.0;

	Noise::FunctionProperties mountains;
	mountains.function = Noise::CubedNoise;
	mountains.operation = Noise::Multiply;
	mountains.persistence = 0.7;
	mountains.frequency = 0.002;
	mountains.low = -13.0;
	mountains.high = 13.0;
	mountains.clamp = Vector2d(0.0, 1.0);
	mountains.children.add(mountainsCellSquared);
	mountains.children.add(mountainsRidged);

	Noise::FunctionProperties oceanNoise;
	oceanNoise.operation = Noise::Subtract;
	oceanNoise.octaves = 6;
	oceanNoise.persistence = 0.9;
	oceanNoise.frequency = 0.002;
	oceanNoise.low = 10000.0;
	oceanNoise.high = 20000.0;

	Noise::FunctionProperties oceans;
	oceans.operation = Noise::Multiply;
	oceans.octaves = 6;
	oceans.persistence = 0.75;
	oceans.frequency = 0.00015;
	oceans.low = -5.0;
	oceans.high = 7.0;
	oceans.clamp = Vector2d(0.0, 1.0);
	oceans.children.add(oceanNoise);

	Noise::Base base;
	base.base = 10.0;
	base.functions.add(tinyDetail);
	base.functions.add(smallDetail);
	base.functions.add(largeDetail);
	//base.functions.add(mountains);
	//base.functions.add(oceans);

	double n;
	Noise::evaluateNoise(Vector3d(1.0, 1.0, 0), base, n);
	LOGF(Info, CoreEngine, "h: %f", n);

	uint32 width = 101;
	uint32 height = 101;

	double f = 1.0;

	Image<uint8> noiseImage;
	uint8* data = new uint8[width * height * 4];
	noiseImage.init(ImageFormat::RGBA, width, height, data);
	AE_DELETE_ARRAY(data);

	for (uint32 i = 0; i < width; i++)
	{
		double x = i / (double)width * f;
		for (uint32 j = 0; j < height; j++)
		{
			double y = j / (double)height * f;

			double h;
			Noise::evaluateNoise(Vector3d(x, y, 0), base, h);

			uint8 c = (uint8)(Math::range(h, -1990.0, 3150.0, 0.0, 255.0));

			noiseImage.setPixel(i, j, Vector4<uint8>(c, c, c, 255));
		}
	}

	noiseImage.save("resources/noise_texture.png");


	//cellular automata

	Array<CellularAutomata::Rule> ruleSet;
	ruleSet.add(CellularAutomata::Rule(false, 4, CellularAutomata::Rule::LessThan, true));

	CellularAutomata cellularAutomata(100, 100, ruleSet);
	cellularAutomata.step();

	//markov chain
	MarkovChain<std::string> markovChain;
	markovChain.addState(0, "a");
	markovChain.addState(1, "b");
	markovChain.addState(2, "c");
	markovChain.addTransition(0, 1, 0.9);
	markovChain.addTransition(0, 2, 0.1);
	markovChain.addTransition(1, 0, 0.05);
	markovChain.addTransition(1, 2, 0.05);
	markovChain.addTransition(1, 1, 0.9);
	markovChain.addTransition(2, 0, 0.1);
	markovChain.addTransition(2, 1, 0.9);

	markovChain.setStartingState(0);

	std::vector<std::string> strings = markovChain.get(10, true);
	for (uint32 i = 0; i < strings.size(); i++)
	{
		LOGF(Info, CoreEngine, "strings: %s", strings[i].c_str());
	}

	//l-system

	LSystem<char, std::string> system;
	system.addRule('X', "F+[[X]-X]-F[-FX]+X");
	system.addRule('F', "FF");

	system.setState("X");
	
	LOGF(Info, CoreEngine, "l-system state: %s", system.getState().c_str());
	system.step();
	LOGF(Info, CoreEngine, "l-system state: %s", system.getState().c_str());
	system.step();
	LOGF(Info, CoreEngine, "l-system state: %s", system.getState().c_str());
	system.step();
	LOGF(Info, CoreEngine, "l-system state: %s", system.getState().c_str());
}
