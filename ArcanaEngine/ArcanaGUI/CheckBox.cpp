#include "CheckBox.h"

namespace Arcana
{
	CheckBox::CheckBox(Widget* parent, const std::string& caption) 
		: Widget(parent), _caption(caption), _pushed(false), _checked(false)
	{
		setIconExtraScale(1.2f);
	}

	CheckBox::~CheckBox()
	{

	}

	const std::string& CheckBox::getCaption() const
	{
		return _caption;
	}

	void CheckBox::setCaption(const std::string& caption)
	{
		_caption = caption;
	}

	const bool& CheckBox::isChecked() const
	{
		return _checked;
	}

	void CheckBox::setChecked(const bool& checked)
	{
		_checked = checked;
	}

	const bool& CheckBox::isPushed() const
	{
		return _pushed;
	}

	void CheckBox::setPushed(const bool& pushed)
	{
		_pushed = pushed;
	}

	OnCheckedCallback& CheckBox::getOnCheckedCallback()
	{
		return _onCheckedCallback;
	}



	Vector2i CheckBox::preferredSize(GUIRenderContext& renderContext) const
	{
		Vector2i fs = getFixedSize();

		if (fs != Vector2i::zero())
		{
			return fs;
		}

		uint32 w, h;

		int32 fontSize = getFontSize();
		getTheme()->NormalFont->measureText(_caption, fontSize, w, h);

		return Vector2i(w + 1.8f * fontSize, h * 1.3f);
	}

	bool CheckBox::mouseButtonEvent(const Vector2i &p, Key button, bool down, ModifierKeysState modifiers)
	{
		Widget::mouseButtonEvent(p, button, down, modifiers);

		if (!isEnabled())
			return false;

		if (button.getKeyCode() == KeyCode::LeftMouseButton)
		{
			if (down)
			{
				_pushed = true;
			}
			else if (_pushed)
			{
				if (contains(p))
				{
					_checked = !_checked;
					_onCheckedCallback.executeIfBound(_checked);
				}
				_pushed = false;
			}
			return true;
		}
		return false;
	}

	void CheckBox::render(GUIRenderContext& renderContext)
	{
		renderContext.reset();

		int32 fontSize = getFontSize() == -1 ? getTheme()->ButtonFontSize : getFontSize();

		Color textColor = isEnabled() ? getTheme()->TextColor : getTheme()->DisabledTextColor;

		Rectf textArea = Rectf(getAbsolutePosition().x + 1.6 * (float)fontSize, getAbsolutePosition().y, getSize().x - 1.6 * (float)fontSize, getSize().y);

		Font* font = getTheme()->NormalFont;
		font->start();
		font->drawText(_caption, textArea, textColor, fontSize, Font::Justify::AlignVerticalHorizontalCenter, false, false, textArea, renderContext.getCurrentZ());
		font->finish();

		//renderContext.setPrimaryColor(isChecked() ? getTheme()->TextColor : getTheme()->DisabledTextColor);
		renderContext.setPrimaryColor(Color(0, 0, 0, 25));
		renderContext.setSecondaryColor(_pushed ? Color(15, 15, 15, 255) : Color(30, 30, 30, 255));
		renderContext.setBoxGradient(0.0, 0.0, 1.0, 1.0, true);
		renderContext.drawRoundedRect(getAbsolutePosition().x + 1.0f, getAbsolutePosition().y + 1.0f, getSize().y - 2.0f, getSize().y - 2.0f, getTheme()->ButtonCornerRadius);

		if (_checked)
		{
			//draw check icon
			Rectf iconArea = Rectf(getAbsolutePosition().x + 1.0f, getAbsolutePosition().y + 1.0f, getSize().y - 2.0f, getSize().y - 2.0f);
			renderContext.drawIcon(getTheme()->CheckIcon, iconArea, isEnabled() ? getTheme()->IconColor : getTheme()->DisabledTextColor);
		}

		/*Color c = _pushed ? Color(255, 0, 0, 255) : _checked ? Color(0, 0, 255, 255) : Color(128, 128, 128, 255);

		renderContext.setPrimaryColor(c);

		renderContext.drawRoundedRect(getPosition().x, getPosition().y, getSize().y, getSize().y, 0.0);*/

		Widget::render(renderContext);
	}
}