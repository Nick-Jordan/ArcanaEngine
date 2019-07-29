#ifndef COLORS_H_
#define COLORS_H_

#include "CoreDefines.h"

#include "ArcanaLog.h"
#include "Color.h"
#include "Array.h"
#include <map>

namespace Arcana
{
	REGISTER_CATEGORY_ENGINE(ARCANA_CORE_API, ColorsListLog, none)

	class ARCANA_CORE_API Colors
	{
	public:

		typedef std::map<std::string, const Color*> ColorsMap;
		typedef Array< const Color* > ColorsLookup;

	public:

		static const Color White;
		static const Color Red;
		static const Color Green;
		static const Color Blue;
		static const Color Magenta;
		static const Color Cyan;
		static const Color Yellow;
		static const Color Black;
		static const Color Aquamarine;
		static const Color BakerChocolate;
		static const Color BlueViolet;
		static const Color Brass;
		static const Color BrightGold;
		static const Color Brown;
		static const Color Bronze;
		static const Color BronzeII;
		static const Color CadetBlue;
		static const Color CoolCopper;
		static const Color Copper;
		static const Color Coral;
		static const Color CornFlowerBlue;
		static const Color DarkBrown;
		static const Color DarkGreen;
		static const Color DarkGreenCopper;
		static const Color DarkOliveGreen;
		static const Color DarkOrchid;
		static const Color DarkPurple;
		static const Color DarkSlateBlue;
		static const Color DarkSlateGrey;
		static const Color DarkTan;
		static const Color DarkTurquoise;
		static const Color DarkWood;
		static const Color DimGrey;
		static const Color DustyRose;
		static const Color Feldspar;
		static const Color Firebrick;
		static const Color ForestGreen;
		static const Color Gold;
		static const Color Goldenrod;
		static const Color Grey;
		static const Color GreenCopper;
		static const Color GreenYellow;
		static const Color HunterGreen;
		static const Color IndianRed;
		static const Color Khaki;
		static const Color LightBlue;
		static const Color LightGrey;
		static const Color LightSteelBlue;
		static const Color LightWood;
		static const Color LimeGreen;
		static const Color MandarianOrange;
		static const Color Maroon;
		static const Color MediumAquamarine;
		static const Color MediumBlue;
		static const Color MediumForestGreen;
		static const Color MediumGoldenrod;
		static const Color MediumOrchid;
		static const Color MediumSeaGreen;
		static const Color MediumSlateBlue;
		static const Color MediumSpringGreen;
		static const Color MediumTurquoise;
		static const Color MediumVioletRed;
		static const Color MediumWood;
		static const Color MidnightBlue;
		static const Color NavyBlue;
		static const Color NeonBlue;
		static const Color NeonPink;
		static const Color NewMidnightBlue;
		static const Color NewTan;
		static const Color OldGold;
		static const Color Orange;
		static const Color OrangeRed;
		static const Color Orchid;
		static const Color PaleGreen;
		static const Color Pink;
		static const Color Plum;
		static const Color Quartz;
		static const Color RichBlue;
		static const Color Salmon;
		static const Color Scarlet;
		static const Color SeaGreen;
		static const Color SemiSweetChocolate;
		static const Color Sienna;
		static const Color Silver;
		static const Color SkyBlue;
		static const Color SlateBlue;
		static const Color SpicyPink;
		static const Color SpringGreen;
		static const Color SteelBlue;
		static const Color SummerSky;
		static const Color Tan;
		static const Color Thistle;
		static const Color Turquoise;
		static const Color VeryDarkBrown;
		static const Color VeryLightGrey;
		static const Color Violet;
		static const Color VioletRed;
		static const Color Wheat;
		static const Color YellowGreen;

	public:

		void createColorMap();

		const Color& getColor(const std::string& name) const;

		const LinearColor getLinearColor(const std::string& name) const;

		bool isValidColorName(const std::string& name) const;

		int32 getColorIndex(const std::string& name) const;

		const Color& getColor(int32 index) const;

		const std::string& getName(int32 index) const;

		int32 getNumColors() const;

		void logColors();

	protected:

		void initializeColor(const std::string& name, const Color* color, int32& index);

		ColorsMap _map;

		ColorsLookup _lookup;
	};

}

#endif // !COLORS_H_

