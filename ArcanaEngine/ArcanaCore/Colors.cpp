#include "Colors.h"

namespace Arcana
{
	INITIALIZE_CATEGORY(Arcana, ColorsListLog)

	const Color& Colors::getColor(const std::string& name) const
	{
		ColorsMap::const_iterator iter = _map.find(name);

		if (iter != _map.end())
		{
			return *(*iter).second;
		}

		return White;
	}

	const Color& Colors::getColor(int32 index) const
	{
		if (_lookup.isValidIndex(index))
		{
			return *_lookup[index];
		}

		return White;
	}

	const LinearColor Colors::getLinearColor(const std::string& name) const
	{
		ColorsMap::const_iterator iter = _map.find(name);

		if (iter != _map.end())
		{
			return *(*iter).second;
		}

		return LinearColor::White;
	}

	bool Colors::isValidColorName(const std::string& name) const
	{
		ColorsMap::const_iterator iter = _map.find(name);
		return iter != _map.end() ? true : false;
	}


	int32 Colors::getColorIndex(const std::string& name) const
	{
		const Color& color = getColor(name);
		int32 index = 0;
		_lookup.find(&color, index);
		return index;
	}

	const std::string& Colors::getName(int32 index) const
	{
		static const std::string invalidIndex("InvalidIndex");

		if (_lookup.isValidIndex(index))
		{
			const Color& color = *_lookup[index];

			ColorsMap::const_iterator it;
			for (it = _map.begin(); it != _map.end(); ++it)
				if (it->second == &color)
					return it->first;
		}

		return invalidIndex;
	}

	void Colors::createColorMap()
	{
		int32 index = 0;

		initializeColor("black", &Colors::Black, index);
		initializeColor("aquamarine", &Colors::Aquamarine, index);
		initializeColor("bakerchocolate", &Colors::BakerChocolate, index);
		initializeColor("blue", &Colors::Blue, index);
		initializeColor("blueviolet", &Colors::BlueViolet, index);
		initializeColor("brass", &Colors::Brass, index);
		initializeColor("brightgold", &Colors::BrightGold, index);
		initializeColor("brown", &Colors::Brown, index);
		initializeColor("bronze", &Colors::Bronze, index);
		initializeColor("bronzeii", &Colors::BronzeII, index);
		initializeColor("cadetblue", &Colors::CadetBlue, index);
		initializeColor("coolcopper", &Colors::CoolCopper, index);
		initializeColor("copper", &Colors::Copper, index);
		initializeColor("coral", &Colors::Coral, index);
		initializeColor("cornflowerblue", &Colors::CornFlowerBlue, index);
		initializeColor("cyan", &Colors::Cyan, index);
		initializeColor("darkbrown", &Colors::DarkBrown, index);
		initializeColor("darkgreen", &Colors::DarkGreen, index);
		initializeColor("darkgreencopper", &Colors::DarkGreenCopper, index);
		initializeColor("darkolivegreen", &Colors::DarkOliveGreen, index);
		initializeColor("darkorchid", &Colors::DarkOrchid, index);
		initializeColor("darkpurple", &Colors::DarkPurple, index);
		initializeColor("darkslateblue", &Colors::DarkSlateBlue, index);
		initializeColor("darkslategrey", &Colors::DarkSlateGrey, index);
		initializeColor("darktan", &Colors::DarkTan, index);
		initializeColor("darkturquoise", &Colors::DarkTurquoise, index);
		initializeColor("darkwood", &Colors::DarkWood, index);
		initializeColor("dimgrey", &Colors::DimGrey, index);
		initializeColor("dustyrose", &Colors::DustyRose, index);
		initializeColor("feldspar", &Colors::Feldspar, index);
		initializeColor("firebrick", &Colors::Firebrick, index);
		initializeColor("forestgreen", &Colors::ForestGreen, index);
		initializeColor("gold", &Colors::Gold, index);
		initializeColor("goldenrod", &Colors::Goldenrod, index);
		initializeColor("green", &Colors::Green, index);
		initializeColor("greencopper", &Colors::GreenCopper, index);
		initializeColor("greenyellow", &Colors::GreenYellow, index);
		initializeColor("grey", &Colors::Grey, index);
		initializeColor("huntergreen", &Colors::HunterGreen, index);
		initializeColor("indianred", &Colors::IndianRed, index);
		initializeColor("khaki", &Colors::Khaki, index);
		initializeColor("lightblue", &Colors::LightBlue, index);
		initializeColor("lightgrey", &Colors::LightGrey, index);
		initializeColor("lightsteelblue", &Colors::LightSteelBlue, index);
		initializeColor("lightwood", &Colors::LightWood, index);
		initializeColor("limegreen", &Colors::LimeGreen, index);
		initializeColor("magenta", &Colors::Magenta, index);
		initializeColor("mandarianorange", &Colors::MandarianOrange, index);
		initializeColor("maroon", &Colors::Maroon, index);
		initializeColor("mediumaquamarine", &Colors::MediumAquamarine, index);
		initializeColor("mediumblue", &Colors::MediumBlue, index);
		initializeColor("mediumforestgreen", &Colors::MediumForestGreen, index);
		initializeColor("mediumgoldenrod", &Colors::MediumGoldenrod, index);
		initializeColor("mediumorchid", &Colors::MediumOrchid, index);
		initializeColor("mediumseagreen", &Colors::MediumSeaGreen, index);
		initializeColor("mediumslateblue", &Colors::MediumSlateBlue, index);
		initializeColor("mediumspringgreen", &Colors::MediumSpringGreen, index);
		initializeColor("mediumturquoise", &Colors::MediumTurquoise, index);
		initializeColor("mediumvioletred", &Colors::MediumVioletRed, index);
		initializeColor("mediumwood", &Colors::MediumWood, index);
		initializeColor("midnightblue", &Colors::MidnightBlue, index);
		initializeColor("navyblue", &Colors::NavyBlue, index);
		initializeColor("neonblue", &Colors::NeonBlue, index);
		initializeColor("neonpink", &Colors::NeonPink, index);
		initializeColor("newmidnightblue", &Colors::NewMidnightBlue, index);
		initializeColor("newtan", &Colors::NewTan, index);
		initializeColor("oldgold", &Colors::OldGold, index);
		initializeColor("orange", &Colors::Orange, index);
		initializeColor("orangered", &Colors::OrangeRed, index);
		initializeColor("orchid", &Colors::Orchid, index);
		initializeColor("palegreen", &Colors::PaleGreen, index);
		initializeColor("pink", &Colors::Pink, index);
		initializeColor("plum", &Colors::Plum, index);
		initializeColor("quartz", &Colors::Quartz, index);
		initializeColor("red", &Colors::Red, index);
		initializeColor("richblue", &Colors::RichBlue, index);
		initializeColor("salmon", &Colors::Salmon, index);
		initializeColor("scarlet", &Colors::Scarlet, index);
		initializeColor("seagreen", &Colors::SeaGreen, index);
		initializeColor("semisweetchocolate", &Colors::SemiSweetChocolate, index);
		initializeColor("sienna", &Colors::Sienna, index);
		initializeColor("silver", &Colors::Silver, index);
		initializeColor("skyblue", &Colors::SkyBlue, index);
		initializeColor("slateblue", &Colors::SlateBlue, index);
		initializeColor("spicypink", &Colors::SpicyPink, index);
		initializeColor("springgreen", &Colors::SpringGreen, index);
		initializeColor("steelblue", &Colors::SteelBlue, index);
		initializeColor("summersky", &Colors::SummerSky, index);
		initializeColor("tan", &Colors::Tan, index);
		initializeColor("thistle", &Colors::Thistle, index);
		initializeColor("turquoise", &Colors::Turquoise, index);
		initializeColor("verydarkbrown", &Colors::VeryDarkBrown, index);
		initializeColor("verylightgrey", &Colors::VeryLightGrey, index);
		initializeColor("violet", &Colors::Violet, index);
		initializeColor("violetred", &Colors::VioletRed, index);
		initializeColor("wheat", &Colors::Wheat, index);
		initializeColor("white", &Colors::White, index);
		initializeColor("yellow", &Colors::Yellow, index);
		initializeColor("yellowgreen", &Colors::YellowGreen, index);

		_lookup.shrink();
	}

	void Colors::initializeColor(const std::string& name, const Color* color, int32& index)
	{
		_map.insert(std::make_pair(name, color));
		_lookup.add(color);

		index++;
	}

	void Colors::logColors()
	{
		for (ColorsMap::iterator it = _map.begin(); it != _map.end(); it++)
		{
			const Color* color = (*it).second;
			const std::string& name = (*it).first;

			int32 index = 0;
			_lookup.find(color, index);

			LOGF(Info, ColorsListLog, "%3i - %32s -> %s", index, name.c_str(), color->toString().c_str());
		}
	}

	const Color Colors::White(255, 255, 255, 255);
	const Color Colors::Red(255, 0, 0, 255);
	const Color Colors::Green(0, 255, 0, 255);
	const Color Colors::Blue(0, 0, 255, 255);
	const Color Colors::Magenta(255, 0, 255, 255);
	const Color Colors::Cyan(0, 255, 255, 255);
	const Color Colors::Yellow(255, 255, 0, 255);
	const Color Colors::Black(0, 0, 0, 255);
	const Color Colors::Aquamarine(112, 219, 147, 255);
	const Color Colors::BakerChocolate(92, 51, 23, 255);
	const Color Colors::BlueViolet(159, 95, 159, 255);
	const Color Colors::Brass(181, 166, 66, 255);
	const Color Colors::BrightGold(217, 217, 25, 255);
	const Color Colors::Brown(166, 42, 42, 255);
	const Color Colors::Bronze(140, 120, 83, 255);
	const Color Colors::BronzeII(166, 125, 61, 255);
	const Color Colors::CadetBlue(95, 159, 159, 255);
	const Color Colors::CoolCopper(217, 135, 25, 255);
	const Color Colors::Copper(184, 115, 51, 255);
	const Color Colors::Coral(255, 127, 0, 255);
	const Color Colors::CornFlowerBlue(66, 66, 111, 255);
	const Color Colors::DarkBrown(92, 64, 51, 255);
	const Color Colors::DarkGreen(47, 79, 47, 255);
	const Color Colors::DarkGreenCopper(74, 118, 110, 255);
	const Color Colors::DarkOliveGreen(79, 79, 47, 255);
	const Color Colors::DarkOrchid(153, 50, 205, 255);
	const Color Colors::DarkPurple(135, 31, 120, 255);
	const Color Colors::DarkSlateBlue(107, 35, 142, 255);
	const Color Colors::DarkSlateGrey(47, 79, 79, 255);
	const Color Colors::DarkTan(151, 105, 79, 255);
	const Color Colors::DarkTurquoise(112, 147, 219, 255);
	const Color Colors::DarkWood(133, 94, 66, 255);
	const Color Colors::DimGrey(84, 84, 84, 255);
	const Color Colors::DustyRose(133, 99, 99, 255);
	const Color Colors::Feldspar(209, 146, 117, 255);
	const Color Colors::Firebrick(142, 35, 35, 255);
	const Color Colors::ForestGreen(35, 142, 35, 255);
	const Color Colors::Gold(205, 127, 50, 255);
	const Color Colors::Goldenrod(219, 219, 112, 255);
	const Color Colors::Grey(192, 192, 192, 255);
	const Color Colors::GreenCopper(82, 127, 118, 255);
	const Color Colors::GreenYellow(147, 219, 112, 255);
	const Color Colors::HunterGreen(33, 94, 33, 255);
	const Color Colors::IndianRed(78, 47, 47, 255);
	const Color Colors::Khaki(159, 159, 95, 255);
	const Color Colors::LightBlue(192, 217, 217, 255);
	const Color Colors::LightGrey(168, 168, 168, 255);
	const Color Colors::LightSteelBlue(143, 143, 189, 255);
	const Color Colors::LightWood(233, 194, 166, 255);
	const Color Colors::LimeGreen(50, 205, 50, 255);
	const Color Colors::MandarianOrange(228, 120, 51, 255);
	const Color Colors::Maroon(142, 35, 107, 255);
	const Color Colors::MediumAquamarine(50, 205, 153, 255);
	const Color Colors::MediumBlue(50, 50, 205, 255);
	const Color Colors::MediumForestGreen(107, 142, 35, 255);
	const Color Colors::MediumGoldenrod(234, 234, 174, 255);
	const Color Colors::MediumOrchid(147, 112, 219, 255);
	const Color Colors::MediumSeaGreen(66, 111, 66, 255);
	const Color Colors::MediumSlateBlue(127, 0, 255, 255);
	const Color Colors::MediumSpringGreen(127, 255, 0, 255);
	const Color Colors::MediumTurquoise(112, 219, 219, 255);
	const Color Colors::MediumVioletRed(219, 112, 147, 255);
	const Color Colors::MediumWood(166, 128, 100, 255);
	const Color Colors::MidnightBlue(47, 47, 79, 255);
	const Color Colors::NavyBlue(35, 35, 142, 255);
	const Color Colors::NeonBlue(77, 77, 255, 255);
	const Color Colors::NeonPink(255, 110, 199, 255);
	const Color Colors::NewMidnightBlue(0, 0, 156, 255);
	const Color Colors::NewTan(235, 199, 158, 255);
	const Color Colors::OldGold(207, 181, 59, 255);
	const Color Colors::Orange(255, 127, 0, 255);
	const Color Colors::OrangeRed(255, 36, 0, 255);
	const Color Colors::Orchid(219, 112, 219, 255);
	const Color Colors::PaleGreen(143, 188, 143, 255);
	const Color Colors::Pink(188, 143, 143, 255);
	const Color Colors::Plum(234, 173, 234, 255);
	const Color Colors::Quartz(217, 217, 243, 255);
	const Color Colors::RichBlue(89, 89, 171, 255);
	const Color Colors::Salmon(111, 66, 66, 255);
	const Color Colors::Scarlet(140, 23, 23, 255);
	const Color Colors::SeaGreen(35, 142, 104, 255);
	const Color Colors::SemiSweetChocolate(107, 66, 38, 255);
	const Color Colors::Sienna(142, 107, 35, 255);
	const Color Colors::Silver(230, 232, 250, 255);
	const Color Colors::SkyBlue(50, 153, 204, 255);
	const Color Colors::SlateBlue(0, 127, 255, 255);
	const Color Colors::SpicyPink(255, 28, 174, 255);
	const Color Colors::SpringGreen(0, 255, 127, 255);
	const Color Colors::SteelBlue(35, 107, 142, 255);
	const Color Colors::SummerSky(56, 176, 222, 255);
	const Color Colors::Tan(219, 147, 112, 255);
	const Color Colors::Thistle(216, 191, 216, 255);
	const Color Colors::Turquoise(173, 234, 234, 255);
	const Color Colors::VeryDarkBrown(92, 64, 51, 255);
	const Color Colors::VeryLightGrey(205, 205, 205, 255);
	const Color Colors::Violet(79, 47, 79, 255);
	const Color Colors::VioletRed(204, 50, 153, 255);
	const Color Colors::Wheat(216, 216, 191, 255);
	const Color Colors::YellowGreen(153, 204, 50, 255);
}