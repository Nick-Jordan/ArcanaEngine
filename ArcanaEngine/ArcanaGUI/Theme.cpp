#include "Theme.h"

namespace Arcana
{
	Theme* Theme::Default = nullptr;

	Theme::Theme()
	{
		StandardFontSize = 16;
		ButtonFontSize = 20;
		TextBoxFontSize = 20;
		IconScale = 0.77f;

		WindowCornerRadius = 0.2;
		WindowHeaderHeight = 30;
		WindowDropShadowSize = 10;
		ButtonCornerRadius = 0.2;
		TabBorderWidth = 0.75f;
		TabInnerMargin = 5;
		TabMinButtonWidth = 20;
		TabMaxButtonWidth = 160;
		TabControlWidth = 20;
		TabButtonHorizontalPadding = 10;
		TabButtonVerticalPadding = 2;

		DropShadow = Color(0, 0, 0, 128);
		Transparent = Color(0, 0, 0, 0);
		BorderDark = Color(29, 29, 29, 255);
		BorderLight = Color(92, 92, 92, 255);
		BorderMedium = Color(35, 35, 35, 255);
		TextColor = Color(255, 255, 255, 160);
		DisabledTextColor = Color(255, 255, 255, 80);
		TextColorShadow = Color(0, 0, 0, 160);
		IconColor = TextColor;

		ButtonGradientTopFocused = Color(64, 64, 64, 255);
		ButtonGradientBotFocused = Color(48, 48, 48, 255);
		ButtonGradientTopUnfocused = Color(74, 74, 74, 255);
		ButtonGradientBotUnfocused = Color(58, 58, 58, 255);
		ButtonGradientTopPushed = Color(41, 41, 41, 255);
		ButtonGradientBotPushed = Color(29, 29, 29, 255);

		WindowFillUnfocused = Color(43, 43, 43, 230);
		WindowFillFocused = Color(45, 45, 45, 230);
		WindowTitleUnfocused = Color(220, 220, 220, 160);
		WindowTitleFocused = Color(255, 255, 255, 190);

		WindowHeaderGradientTop = ButtonGradientTopUnfocused;
		WindowHeaderGradientBot = ButtonGradientBotUnfocused;
		WindowHeaderSepTop = BorderLight;
		WindowHeaderSepBot = BorderDark;

		WindowPopup = Color(50, 50, 50, 255);
		WindowPopupTransparent = Color(50, 50, 50, 0);

		/*icons*/

		/*fonts*/
	}


	Theme::~Theme()
	{
	}

}
