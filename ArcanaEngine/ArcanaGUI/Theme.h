#ifndef THEME_H_
#define THEME_H_

#include "GUIDefines.h"

#include "Object.h"
#include "Color.h"

namespace Arcana
{

	class Theme : public Object
	{
	public:

		static Theme* Default;

	public:

		Theme();

		~Theme();

	public:

		int32 StandardFontSize;
		int32 ButtonFontSize;
		int32 TextBoxFontSize;
		float IconScale;

		float WindowCornerRadius;
		int32 WindowHeaderHeight;
		int32 WindowDropShadowSize;
		float ButtonCornerRadius;
		float TabBorderWidth;
		int32 TabInnerMargin;
		int32 TabMinButtonWidth;
		int32 TabMaxButtonWidth;
		int32 TabControlWidth;
		int32 TabButtonHorizontalPadding;
		int32 TabButtonVerticalPadding;

		Color DropShadow;
		Color Transparent;
		Color BorderDark;
		Color BorderLight;
		Color BorderMedium;
		Color TextColor;
		Color DisabledTextColor;
		Color TextColorShadow;
		Color IconColor;

		Color ButtonGradientTopFocused;
		Color ButtonGradientBotFocused;
		Color ButtonGradientTopUnfocused;
		Color ButtonGradientBotUnfocused;
		Color ButtonGradientTopPushed;
		Color ButtonGradientBotPushed;

		Color WindowFillUnfocused;
		Color WindowFillFocused;
		Color WindowTitleUnfocused;
		Color WindowTitleFocused;

		Color WindowHeaderGradientTop;
		Color WindowHeaderGradientBot;
		Color WindowHeaderSepTop;
		Color WindowHeaderSepBot;

		Color WindowPopup;
		Color WindowPopupTransparent;
	};

}

#endif // !THEME_H_

