#ifndef THEME_H_
#define THEME_H_

#include "GUIDefines.h"

#include "Object.h"
#include "Color.h"
#include "Font.h"
#include "Material.h"
#include "GUIIcon.h"

namespace Arcana
{

	class ARCANA_GUI_API Theme : public Object
	{
	public:

		static Theme* Default;

	public:

		Theme();

		Theme(const Theme& theme);

		~Theme();

	public:

		int32 StandardFontSize;
		int32 ButtonFontSize;
		int32 TextBoxFontSize;
		float IconScale;

		float PanelCornerRadius;
		int32 PanelHeaderHeight;
		int32 PanelDropShadowSize;
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
		Color ButtonSidebar;
		Color ButtonSidebarFocused;
		Color ButtonSidebarPushed;
		Color ButtonBorder;
		Color ButtonBorderFocused;
		Color ButtonBorderPushed;

		Color PanelFillUnfocused;
		Color PanelFillFocused;
		Color PanelTitleUnfocused;
		Color PanelTitleFocused;

		Color PanelHeaderGradientTop;
		Color PanelHeaderGradientBot;
		Color PanelHeaderSepTop;
		Color PanelHeaderSepBot;

		Color PanelPopup;
		Color PanelPopupTransparent;

		GUIIcon* CheckIcon;

		Font* NormalFont;

		Material* TextMaterial;
	};

}

#endif // !THEME_H_

