#include "Label.h"

namespace Arcana
{

	Label::Label(Widget* parent, const std::string& caption, Font::Justify justify, int32 fontSize)
		: Widget(parent), _caption(caption), _fontJustify(justify), _hasBackground(false), _backgroundColor(Color(0, 0, 0, 0))
	{
		if (getTheme())
		{
			setFontSize(getTheme()->StandardFontSize);
			_color = getTheme()->TextColor;
		}
		if (fontSize >= 0)
		{
			setFontSize(fontSize);
		}
	}

	const std::string& Label::getCaption() const
	{
		return _caption;
	}

	void Label::setCaption(const std::string& caption)
	{
		_caption = caption;
	}

	Font::Justify Label::getFontJustify() const
	{
		return _fontJustify;
	}

	void Label::setFontJustify(Font::Justify justify)
	{
		_fontJustify = justify;
	}

	bool Label::hasBackground() const
	{
		return _hasBackground;
	}

	void Label::hasBackground(bool hasBackground)
	{
		_hasBackground = hasBackground;
	}

	const Color& Label::getBackgroundColor() const
	{
		return _backgroundColor;
	}

	void Label::setBackgroundColor(const Color& backgroundColor)
	{
		_backgroundColor = backgroundColor;
	}

	Color Label::getColor() const
	{
		return _color;
	}

	void Label::setColor(const Color& color)
	{
		_color = color;
	}

	void Label::setTheme(Theme *theme) 
	{
		Widget::setTheme(theme);

		if (getTheme())
		{
			setFontSize(getTheme()->StandardFontSize);
			_color = getTheme()->TextColor;
		}
	}

	Vector2i Label::preferredSize(GUIRenderContext& renderContext) const
	{
		if (_caption == "")
		{
			return Vector2i::zero();
		}


		Font* font = getTheme()->NormalFont;

		uint32 width, height;
		font->measureText(_caption, getFontSize(), width, height);

		if (getFixedWidth() > 0)
		{
			return Vector2i(getFixedWidth(), height + 10);
		}

		return Vector2i(width + 20, height + 10);
	}

	void Label::render(GUIRenderContext& renderContext)
	{


		if (_hasBackground)
		{
			renderContext.start();

			renderContext.beginPath();
			renderContext.drawRoundedRect(getAbsolutePosition().x, getAbsolutePosition().y, getSize().x,
				getSize().y, 0.0);
			renderContext.setFillColor(_backgroundColor.A == 0 ? Color(20, 20, 20, 120) : _backgroundColor);
			renderContext.fill();

			renderContext.finish();
			renderContext.draw();
		}

		Rectf area = Rectf(getAbsolutePosition().cast<float>(), getSize().cast<float>());

		Font* font = getTheme()->NormalFont;
		font->start();
		font->drawText(_caption, area, _color, getFontSize(), _fontJustify, true, false, area);
		font->finish();

		Widget::render(renderContext);
	}

}