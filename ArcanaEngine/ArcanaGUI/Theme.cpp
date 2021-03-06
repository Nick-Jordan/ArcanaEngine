#include "Theme.h"

namespace Arcana
{
	Theme* Theme::Default = nullptr;

	Theme::Theme()
	{
		StandardFontSize = 24;
		ButtonFontSize = 24;
		TextBoxFontSize = 24;
		IconScale = 0.77f;

		PanelCornerRadius = 0.05;
		PanelHeaderHeight = 30;
		PanelDropShadowSize = 10;
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
		TextColor = Color(255, 255, 255, 255);
		DisabledTextColor = Color(255, 255, 255, 80);
		TextColorShadow = Color(0, 0, 0, 160);
		IconColor = TextColor;

		ButtonGradientTopFocused = Color(34, 34, 34, 255); //Color(64, 64, 64, 255);
		ButtonGradientBotFocused = Color(14, 14, 14, 255); //Color(48, 48, 48, 255);
		ButtonGradientTopUnfocused = Color(24, 24, 24, 255);//Color(74, 74, 74, 255);
		ButtonGradientBotUnfocused = Color(4, 4, 4, 255);//Color(58, 58, 58, 255);
		ButtonGradientTopPushed = Color(41, 41, 41, 255);
		ButtonGradientBotPushed = Color(29, 29, 29, 255);
		ButtonSidebar = Color(34, 34, 34, 255);
		ButtonSidebarFocused = Color(43, 197, 239, 255);
		ButtonSidebarPushed = Color(18, 166, 206, 255);
		ButtonBorder = Color(0, 0, 0, 0);
		ButtonBorderFocused = Color(43, 197, 239, 255);
		ButtonBorderPushed = Color(18, 166, 206, 255);

		PanelFillUnfocused = Color(24, 24, 24, 100); //Color(43, 43, 43, 255);
		PanelFillFocused = Color(34, 34, 34, 100);//Color(45, 45, 45, 255);
		PanelTitleUnfocused = Color(220, 220, 220, 200);
		PanelTitleFocused = Color(255, 255, 255, 255);

		PanelHeaderGradientTop = ButtonGradientTopUnfocused;
		PanelHeaderGradientBot = ButtonGradientBotUnfocused;
		PanelHeaderSepTop = BorderLight;
		PanelHeaderSepBot = BorderDark;

		PanelPopup = Color(50, 50, 50, 255);
		PanelPopupTransparent = Color(50, 50, 50, 0);

		
		CheckIcon = new GUIIcon("resources/arcana/textures/gui/check1.png");
		CheckIcon->reference();

		NormalFont = Font::load(StandardFontSize, "resources/arcana/fonts/Orbitron.ttf", "orbitron");// , std::vector<uint32>{(uint32)StandardFontSize, (uint32)ButtonFontSize, (uint32)TextBoxFontSize});
		NormalFont->reference();
		LOGF(Info, CoreEngine, "%p", NormalFont);

		Shader shader;
		shader.createProgram(Shader::Vertex, "resources/arcana/shaders/gui/text_vert.glsl");
		Shader::Defines defines;
		defines.addDefine("DISTANCE_FIELD");
		shader.createProgram(Shader::Fragment, "resources/arcana/shaders/gui/text_frag.glsl", defines);

		TextMaterial = new Material("text");
		TextMaterial->reference();
		Technique* technique = new Technique(shader);
		technique->addAttribute("u_Texture0", NormalFont->getGlyphTexture());
		TextMaterial->addTechnique(technique);

		Matrix4f orthographicProjection = Matrix4f::createOrthographicOffCenter(0.0, 1920.0, 1061.0, -19, -1.0, 1.0);
		technique->getPass(0)->getUniform("u_OrthographicProjection").setValue(orthographicProjection);

		NormalFont->getSpriteImmediateRenderer()->setMaterial(TextMaterial);
	}

	Theme::Theme(const Theme& theme)
		: StandardFontSize(theme.StandardFontSize),
		ButtonFontSize(theme.ButtonFontSize),
		TextBoxFontSize(theme.TextBoxFontSize),
		IconScale(theme.IconScale),
		PanelCornerRadius(theme.PanelCornerRadius),
		PanelHeaderHeight(theme.PanelHeaderHeight),
		PanelDropShadowSize(theme.PanelDropShadowSize),
		ButtonCornerRadius(theme.ButtonCornerRadius),
		TabBorderWidth(theme.TabBorderWidth),
		TabInnerMargin(theme.TabInnerMargin),
		TabMinButtonWidth(theme.TabMinButtonWidth),
		TabMaxButtonWidth(theme.TabMaxButtonWidth),
		TabControlWidth(theme.TabControlWidth),
		TabButtonHorizontalPadding(theme.TabButtonHorizontalPadding),
		TabButtonVerticalPadding(theme.TabButtonVerticalPadding),
		DropShadow(theme.DropShadow),
		Transparent(theme.Transparent),
		BorderDark(theme.BorderDark),
		BorderLight(theme.BorderLight),
		BorderMedium(theme.BorderMedium),
		TextColor(theme.TextColor),
		DisabledTextColor(theme.DisabledTextColor),
		TextColorShadow(theme.TextColorShadow),
		IconColor(theme.IconColor),
		ButtonGradientTopFocused(theme.ButtonGradientTopFocused),
		ButtonGradientBotFocused(theme.ButtonGradientBotFocused),
		ButtonGradientTopUnfocused(theme.ButtonGradientTopUnfocused),
		ButtonGradientBotUnfocused(theme.ButtonGradientBotUnfocused),
		ButtonGradientTopPushed(theme.ButtonGradientTopPushed),
		ButtonGradientBotPushed(theme.ButtonGradientBotPushed),
		ButtonSidebar(theme.ButtonSidebar),
		ButtonSidebarFocused(theme.ButtonSidebarFocused),
		ButtonSidebarPushed(theme.ButtonSidebarPushed),
		ButtonBorder(theme.ButtonBorder),
		ButtonBorderFocused(theme.ButtonBorderFocused),
		ButtonBorderPushed(theme.ButtonBorderPushed),
		PanelFillUnfocused(theme.PanelFillUnfocused),
		PanelFillFocused(theme.PanelFillFocused),
		PanelTitleUnfocused(theme.PanelTitleUnfocused),
		PanelTitleFocused(theme.PanelTitleFocused),
		PanelHeaderGradientTop(theme.PanelHeaderGradientTop),
		PanelHeaderGradientBot(theme.PanelHeaderGradientBot),
		PanelHeaderSepTop(theme.PanelHeaderSepTop),
		PanelHeaderSepBot(theme.PanelHeaderSepBot),
		PanelPopup(theme.PanelPopup),
		PanelPopupTransparent(theme.PanelPopupTransparent),
		CheckIcon(theme.CheckIcon),
		NormalFont(theme.NormalFont),
		TextMaterial(theme.TextMaterial)
	{
		AE_REFERENCE(CheckIcon);
		AE_REFERENCE(NormalFont);
		AE_REFERENCE(TextMaterial);
	}


	Theme::~Theme()
	{
		AE_RELEASE(CheckIcon);
		AE_RELEASE(NormalFont);
		AE_RELEASE(TextMaterial);
	}

}
