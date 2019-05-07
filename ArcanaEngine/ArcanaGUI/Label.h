#ifndef LABEL_H_
#define LABEL_H_

#include "GUIDefines.h"

#include "Widget.h"

namespace Arcana
{


	class ARCANA_GUI_API Label : public Widget
	{
	public:

		Label(Widget* parent, const std::string& caption, Font::Justify justify = Font::AlignVerticalHorizontalCenter, int32 fontSize = -1);

		const std::string& getCaption() const;

		void setCaption(const std::string& caption);

		Font::Justify getFontJustify() const;

		void setFontJustify(Font::Justify justify);

		bool hasBackground() const;

		void hasBackground(bool hasBackground);

		const Color& getBackgroundColor() const;

		void setBackgroundColor(const Color& backgroundColor);

		//void setFont(Font* font);

		//const Font* getFont() const;

		Color getColor() const;

		void setColor(const Color& color);

		virtual void setTheme(Theme *theme) override;

		virtual Vector2i preferredSize(GUIRenderContext& renderContext) const override;

		virtual void render(GUIRenderContext& renderContext) override;

	private:

		std::string _caption;

		Color _color;

		Color _backgroundColor;

		Font::Justify _fontJustify;

		bool _hasBackground;
};

}

#endif // !LABEL_H_