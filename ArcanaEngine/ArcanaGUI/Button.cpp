#include "Button.h"

namespace Arcana
{
	Button::Button(Widget* parent, const std::string& caption, GUIIcon* icon)
		: Widget(parent), _caption(caption), _icon(icon),
		_iconPosition(IconPosition::LeftCentered), _pushed(false),
		_flags(Normal), _backgroundColor(Color(0, 0, 0, 0)),
		_textColor(Color(0, 0, 0, 0)), _fontJustify(Font::Justify::AlignVerticalHorizontalCenter),
		_sidebarSize(0), _borderSize(0.0)
	{
	}

	Button::~Button()
	{

	}

	const std::string& Button::getCaption() const
	{
		return _caption;
	}

	void Button::setCaption(const std::string& caption)
	{
		_caption = caption;
	}

	const Color& Button::getBackgroundColor() const
	{
		return _backgroundColor;
	}

	void Button::setBackgroundColor(const Color& backgroundColor)
	{
		_backgroundColor = backgroundColor;
	}

	const Color& Button::getTextColor() const
	{
		return _textColor;
	}

	void Button::setTextColor(const Color& textColor)
	{
		_textColor = textColor;
	}

	GUIIcon* Button::getIcon() const
	{
		return _icon;
	}

	void Button::setIcon(GUIIcon* icon)
	{
		_icon = icon;
	}

	int32 Button::getFlags() const
	{
		return _flags;
	}

	void Button::setFlags(int32 buttonFlags)
	{
		_flags = buttonFlags;
	}

	Button::IconPosition Button::getIconPosition() const
	{
		return _iconPosition;
	}

	void Button::setIconPosition(IconPosition iconPosition)
	{
		_iconPosition = iconPosition;
	}

	void Button::setSidebar(int32 sizebar)
	{
		_sidebarSize = sizebar;
	}

	int32 Button::getSidebar() const
	{
		return _sidebarSize;
	}

	void Button::setBorder(float size)
	{
		_borderSize = size;
	}

	float Button::getBorder() const
	{
		return _borderSize;
	}

	bool Button::isPushed() const
	{
		return _pushed;
	}

	void Button::setPushed(bool pushed)
	{
		_pushed = pushed;
	}

	OnPressedCallback& Button::getOnPressedCallback()
	{
		return _onPressedCallback;
	}

	OnChangedCallback& Button::getOnChangedCallback()
	{
		return _onChangedCallback;
	}

	Font::Justify Button::getFontJustify() const
	{
		return _fontJustify;
	}

	void Button::setFontJustify(Font::Justify justify)
	{
		_fontJustify = justify;
	}

	void Button::setButtonGroup(const std::vector<Button*>& buttonGroup)
	{
		_buttonGroup = buttonGroup;
	}

	const std::vector<Button*>& Button::getButtonGroup() const
	{
		return _buttonGroup;
	}

	Vector2i Button::preferredSize(GUIRenderContext& renderContext) const
	{
		int32 fontSize = getFontSize() == -1 ? getTheme()->ButtonFontSize : getFontSize();

		Font* font = getTheme()->NormalFont;

		uint32 tw, th;
		font->measureText(_caption, fontSize, tw, th);

		float iw = 0.0, ih = 0.0;
		if (_icon)
		{
			float iconScale = getIconExtraScale();

			iw = (float)_icon->getWidth() * iconScale;
			ih = (float)_icon->getHeight() * iconScale;
		}

		return Vector2i((int32)(tw + iw) + 20 + _sidebarSize, (int32)Math::max((float)th, ih) + 10);
	}

	bool Button::mouseButtonEvent(const Vector2i &p, Key button, bool down, ModifierKeysState modifiers)
	{
		Widget::mouseButtonEvent(p, button, down, modifiers);

		Button* self = this;
		self->reference();

		if (button.getKeyCode() == KeyCode::LeftMouseButton && isEnabled())
		{
			bool pushedBackup = _pushed;

			if (down)
			{
				if (_flags & Radio)
				{
					if (_buttonGroup.empty())
					{
						for (auto widget : getParent()->getChildren())
						{
							Button* b = dynamic_cast<Button*>(widget);
							if (b != this && b && (b->getFlags() & Radio) && b->_pushed)
							{
								b->_pushed = false;
								b->getOnChangedCallback().executeIfBound(false);
							}
						}
					}
					else
					{
						for (auto b : _buttonGroup) 
						{
							if (b != this && (b->getFlags() & Radio) && b->_pushed)
							{
								b->_pushed = false;
								b->getOnChangedCallback().executeIfBound(false);
							}
						}
					}
				}

				if (_flags & Popup)
				{
					for (auto widget : getParent()->getChildren())
					{
						Button* b = dynamic_cast<Button*>(widget);
						if (b != this && b && (b->getFlags() & Popup) && b->_pushed)
						{
							b->_pushed = false;
							b->getOnChangedCallback().executeIfBound(false);
						}
					}
				}

				if (_flags & Toggle)
				{
					_pushed = !_pushed;
				}
				else
				{
					_pushed = true;
				}
			}
			else if (_pushed)
			{
				if (contains(p))
				{
					getOnPressedCallback().executeIfBound();
				}
				if (_flags & Normal)
				{
					_pushed = false;
				}
			}
			if (pushedBackup != _pushed)
			{
				getOnChangedCallback().executeIfBound(_pushed);
			}

			return true;
		}
		return false;
	}

	void Button::render(GUIRenderContext& renderContext)
	{
		Color gradTop = getTheme()->ButtonGradientTopUnfocused;
		Color gradBot = getTheme()->ButtonGradientBotUnfocused;

		if (_pushed)
		{
			gradTop = getTheme()->ButtonGradientTopPushed;
			gradBot = getTheme()->ButtonGradientBotPushed;
		}
		else if (hasMouseFocus() && isEnabled())
		{
			gradTop = getTheme()->ButtonGradientTopFocused;
			gradBot = getTheme()->ButtonGradientBotFocused;
		}

		float radius = getTheme()->ButtonCornerRadius;

		renderContext.start();


		renderContext.beginPath();
		renderContext.drawRoundedRect(getAbsolutePosition().x + 1, getAbsolutePosition().y + 1, getSize().x - 1,
			getSize().y - 1, radius);
		renderContext.setFillColor(getTheme()->BorderDark);
		renderContext.fill();

		float add = _pushed ? 1.0 : 0.0;


		if (getBackgroundColor().A != 0)
		{
			renderContext.beginPath();
			renderContext.drawRoundedRect(getAbsolutePosition().x + add, getAbsolutePosition().y + add, getSize().x - 1,
				getSize().y - 1, radius);
			renderContext.setFillColor(getBackgroundColor().withAlpha(255));
			renderContext.fill();

			if (_pushed)
			{
				gradTop.A = gradBot.A = 204;
			}
			else
			{
				uint8 v = 255 - getBackgroundColor().A;
				gradTop.A = gradBot.A = isEnabled() ? v : v / 2 + 128;
			}
		}

		renderContext.beginPath();
		renderContext.drawRoundedRect(getAbsolutePosition().x + add, getAbsolutePosition().y + add, getSize().x - 1,
			getSize().y - 1, radius);

		renderContext.setLinearGradient(getAbsolutePosition().x, getAbsolutePosition().y, getAbsolutePosition().x,
			getAbsolutePosition().y + getSize().y, gradTop, gradBot);

		renderContext.fillLinearGradient();


		if (_borderSize > 0.0)
		{
			renderContext.beginPath();
			renderContext.setStrokeWidth(_borderSize);
			renderContext.drawRoundedRect(getAbsolutePosition().x + add, getAbsolutePosition().y + add, getSize().x - add,
				getSize().y - add, radius);
			renderContext.setStrokeColor(_pushed ? getTheme()->ButtonBorderPushed : hasMouseFocus() ? getTheme()->ButtonBorderFocused : getTheme()->ButtonBorder);
			renderContext.stroke();
		}

		if (_sidebarSize > 0)
		{
			renderContext.beginPath();
			renderContext.setClipRect(getAbsolutePosition().x + add, getAbsolutePosition().y + add, (float)_sidebarSize, getSize().y - add);
			renderContext.drawRoundedRect(getAbsolutePosition().x + add, getAbsolutePosition().y + add, getSize().x - add,
				getSize().y - add, radius);
			renderContext.setFillColor(_pushed ? getTheme()->ButtonSidebarPushed : hasMouseFocus() ? getTheme()->ButtonSidebarFocused : getTheme()->ButtonSidebar);
			renderContext.fill();
		}

		/*renderContext.beginPath();
		renderContext.setStrokeWidth(1.0f);
		renderContext.drawRoundedRect(getAbsolutePosition().x + 0.5f, getAbsolutePosition().y + (_pushed ? 0.5f : 1.5f), getSize().x - 1,
			getSize().y - 1 - (_pushed ? 0.0f : 1.0f), radius);
		renderContext.setStrokeColor(getTheme()->BorderLight);
		renderContext.stroke();

		renderContext.beginPath();
		renderContext.drawRoundedRect(getAbsolutePosition().x + 0.5f, getAbsolutePosition().y + 0.5f, getSize().x - 1,
			getSize().y - 2, radius);
		renderContext.setStrokeColor(getTheme()->BorderDark);
		renderContext.stroke();*/

		renderContext.finish();
		renderContext.draw();

		Vector2i textPosition = getAbsolutePosition();
		textPosition.x += _sidebarSize;
		Vector2i textSize = getSize();
		textSize.x -= _sidebarSize;

		if (_icon)
		{
			Vector2i position = getAbsolutePosition();
			Vector2f size;

			float iconScale = getIconExtraScale();

			if (_iconPosition == IconPosition::Left)
			{
				size.x = Math::min((float)Math::min(getSize().x, getSize().y), (float)_icon->getWidth() * iconScale);
				size.y = Math::min((float)getSize().y, (float)_icon->getHeight() * iconScale);

				textPosition.x += size.x;
				textSize.x -= size.x;
			}
			else if (_iconPosition == IconPosition::Right)
			{
				size.x = Math::min((float)Math::min(getSize().x, getSize().y), (float)_icon->getWidth() * iconScale);
				size.y = Math::min((float)getSize().y, (float)_icon->getHeight() * iconScale);

				position.x += getSize().x - size.x;

				textSize.x -= size.x;
			}
			else if (_iconPosition == IconPosition::LeftCentered)
			{
				size.x = Math::min((float)Math::min(getSize().x, getSize().y), (float)_icon->getWidth() * iconScale);
				size.y = Math::min((float)getSize().y, (float)_icon->getHeight() * iconScale);

				position.y += (getSize().y - size.y) / 2;

				textPosition.x += size.x;
				textSize.x -= size.x;
			}
			else if (_iconPosition == IconPosition::RightCentered)
			{
				size.x = Math::min((float)Math::min(getSize().x, getSize().y), (float)_icon->getWidth() * iconScale);
				size.y = Math::min((float)getSize().y, (float)_icon->getHeight() * iconScale);

				position.x += getSize().x - size.x;
				position.y += (getSize().y - size.y) / 2;

				textSize.x -= size.x;
			}

			renderContext.drawIcon(getTheme()->CheckIcon, Rectf(position.cast<float>(), size.cast<float>()), isEnabled() ? getTheme()->IconColor
				: getTheme()->DisabledTextColor);
		}


		int32 fontSize = getFontSize() == -1 ? getTheme()->ButtonFontSize : getFontSize();

		Color textColor = getTextColor().A == 0 ? getTheme()->TextColor : getTextColor();
		if (!isEnabled())
		{
			textColor = getTheme()->DisabledTextColor;
		}

		Rectf area = Rectf(textPosition.cast<float>(), textSize.cast<float>());

		Font* font = getTheme()->NormalFont;
		font->start();
		font->drawText(_caption, Rectf(area.getLeft(), area.getTop() + 2, area.getSize().x, area.getSize().y), getTheme()->TextColorShadow, fontSize, _fontJustify, true, false, area);
		font->drawText(_caption, area, textColor, fontSize, _fontJustify, true, false, area);
		font->finish();

		Widget::render(renderContext);
	}
}
