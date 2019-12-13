// PCGClient.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include "Renderer.h"

#include "Noise.h"
#include "CellularAutomata.h"
#include "MarkovChain.h"
#include "LSystem.h"

#include "ProceduralGenerator.h"
#include "RenderTextureGenerator.h"

using namespace Arcana;

struct Test
{

};

struct TestParams
{

};

class TestProceduralGenerator : public ProceduralGenerator <Test, TestParams>
{
public:

	virtual void generateObjectAsync(const TestParams& params, Test** test) override
	{
		(*test) = new Test();
	};
};

class TestTextureProceduralGenerator : public ProceduralGenerator <Texture, TestParams>
{
public:

	virtual void generateObjectAsync(const TestParams& params, Texture** texture) override
	{
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
		largeDetail.frequency = 0.003;
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

		uint32 width = 101;
		uint32 height = 101;

		double f = 1.0;

		image.init(ImageFormat::RGBA, width, height, Color(255, 255, 255, 255));

		double heightmap[101][101];

		for (uint32 i = 0; i < width; i++)
		{
			double x = i / (double)width * f;
			for (uint32 j = 0; j < height; j++)
			{
				double y = j / (double)height * f;

				double h;
				Noise::evaluateNoise(Vector3d(x, y, 0), base, h);

				heightmap[i][j] = h;

				//uint8 c = (uint8)(Math::range(h, -1990.0, 3150.0, 0.0, 255.0));

				//image.setPixel(i, j, Vector4<uint8>(c, c, c, 255));
			}
		}

		for (int32 i = 0; i < width; i++)
		{
			for (int32 j = 0; j < height; j++)
			{
				double const z0 = 0.0;

				double const Az = (i + 1 < width) ? (heightmap[i + 1][j]) : z0;
				double const Bz = (j + 1 < height) ? (heightmap[i][j + 1]) : z0;
				double const Cz = (i - 1 >= 0) ? (heightmap[i - 1][j]) : z0;
				double const Dz = (j - 1 >= 0) ? (heightmap[i][j - 1]) : z0;

				Vector3d normal = Vector3d(Cz - Az, Dz - Bz, 2.f);
				normal.normalize();

				Vector3<uint8> c = ((normal * 0.5 + Vector3d(0.5, 0.5, 0.5)) * 255.0f).cast<uint8>();
				uint8 h = (uint8)(Math::range(heightmap[i][j], -1990.0, 3150.0, 0.0, 255.0));

				image.setPixel(i, j, Vector4<uint8>(c.x, c.y, c.z, h));
			}
		}
	};

	virtual void generateObject(const TestParams& params, Texture** texture) override
	{
		std::thread::id this_id = std::this_thread::get_id();
		std::cout << "gen: " << this_id << std::endl;
		(*texture) = Texture::create2D(Texture::RGBA, 101, 101, Texture::RGBA8, Texture::UnsignedByte, image.getPixelsPtr());
	};

private:

	Image<uint8> image;
};

int main()
{
	TestProceduralGenerator* generator = new TestProceduralGenerator();
	TestParams params;
	generator->generate(params);

	Test* test = generator->get();
	std::cout << "TESTESTESTESTESTESET: " << test << std::endl;

	for (int i = 0; i < 10; i++)
	{
		generator->generate(params, i);
	}

	for (int i = 0; i < 10; i++)
	{
		Test* t = generator->get(i);
		std::cout << "test " << i << ": " << t << std::endl;
		AE_DELETE(t);
	}

	AE_DELETE(generator);
	AE_DELETE(test);


	//Texture test
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
	Renderer renderer(settings, 2048, 2048);
	//context

	/*RenderTextureGenerator* textureGenerator = new RenderTextureGenerator();
	TextureProceduralParameters textureParams;
	textureParams.FragmentShader = "resources/shader_frag.glsl";
	textureParams.Format = Texture::RGBA;
	textureParams.InternalFormat = Texture::RGBA8;
	textureParams.PixelType = Texture::UnsignedByte;
	textureParams.Width = 2048;
	textureParams.Height = 2048;
	textureGenerator->generate(textureParams);
	Texture* t = textureGenerator->get();

	glBindTexture(t->getType(), t->getId());
	uint8* renderTextureData = new uint8[t->getWidth() * t->getHeight() * t->getComponents()];
	glGetTexImage(t->getType(), 0, t->getFormat(), t->getPixelType(), renderTextureData);

	Image<uint8> image;
	image.init(ImageFormat::RGBA, t->getWidth(), t->getHeight(), renderTextureData);
	image.save("resources/procedural_texture.png");

	AE_DELETE_ARRAY(renderTextureData);
	AE_DELETE(t);
	AE_DELETE(textureGenerator);*/

	std::thread::id this_id = std::this_thread::get_id();
	std::cout << "main: " << this_id << std::endl;

	TestTextureProceduralGenerator* noiseGenerator = new TestTextureProceduralGenerator();
	noiseGenerator->generate(TestParams(), 1);

	Texture* noiseTexture = noiseGenerator->get(1, true);

	glBindTexture(noiseTexture->getType(), noiseTexture->getId());
	uint8* noiseTextureData = new uint8[noiseTexture->getWidth() * noiseTexture->getHeight() * noiseTexture->getComponents()];
	glGetTexImage(noiseTexture->getType(), 0, noiseTexture->getFormat(), noiseTexture->getPixelType(), noiseTextureData);

	Image<uint8> noiseImage;
	noiseImage.init(ImageFormat::RGBA, 101, 101, noiseTextureData);
	noiseImage.save("resources/noise_texture.png");

	AE_DELETE(noiseTexture);
	AE_DELETE_ARRAY(noiseTextureData);
	AE_DELETE(noiseGenerator);


	//Noise Test

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
	largeDetail.frequency = 0.003;
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

	uint32 width = 512;
	uint32 height = 512;

	double f = 1000.0;

	Image<uint8> proceduralImage;
	uint8* data = new uint8[width * height * 4];
	proceduralImage.init(ImageFormat::RGBA, width, height, data);
	AE_DELETE_ARRAY(data);

	for (uint32 i = 0; i < width; i++)
	{
		double x = i / (double)width * f;
		for (uint32 j = 0; j < height; j++)
		{
			double y = j / (double)height * f;

			double h;
			Noise::evaluateNoise(Vector3d(x, y, 0), base, h);

			//uint8 c = (uint8)(Math::range(h, -1990.0, 3150.0, 0.0, 255.0));
			uint8 c = (uint8)(Math::range(h, -40000.0, 40000.0, 0.0, 255.0));

			proceduralImage.setPixel(i, j, Vector4<uint8>(c, c, c, 255));
		}
	}

	proceduralImage.save("resources/procedural_texture.png");


	//cellular automata

	/*Array<CellularAutomata::Rule> ruleSet;
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
	LOGF(Info, CoreEngine, "l-system state: %s", system.getState().c_str());*/
}
