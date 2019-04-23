#include "Button.h"

namespace Arcana
{
	Button::Button(Widget* parent, const std::string& caption, Image<uint8> icon)
		: Widget(parent), _caption(caption), _icon(icon),
		_iconPosition(IconPosition::LeftCentered), _pushed(false),
		_flags(Normal), _backgroundColor(Color(0, 0, 0)),
		_textColor(Color(0, 0, 0))
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

	Image<uint8> Button::getIcon() const
	{
		return _icon;
	}

	void Button::setIcon(Image<uint8> icon)
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
		/*int32 fontSize = getFontSize() == -1 ? getTheme()->getButtonFontSize() : getFontSize();

		float tw = 0.0;//get text width;nvgTextBounds(ctx, 0, 0, mCaption.c_str(), nullptr, nullptr);
		float iw = 0.0f;
		float ih = fontSize;

		if (_icon)
		{
			if (nvgIsFontIcon(mIcon)) {
				ih *= icon_scale();
				nvgFontFace(ctx, "icons");
				nvgFontSize(ctx, ih);
				iw = nvgTextBounds(ctx, 0, 0, utf8(mIcon).data(), nullptr, nullptr)
					+ mSize.y() * 0.15f;
			}
			else {
				int w, h;
				ih *= 0.9f;
				nvgImageSize(ctx, mIcon, &w, &h);
				iw = w * ih / h;
			}
		}
		return Vector2i((int)(tw + iw) + 20, fontSize + 10);*/
		return Vector2i::zero();
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
		Widget::render(renderContext);

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

		renderContext.beginPath();
		renderContext.drawRoundedRect(getPosition().x + 1, getPosition().y + 1.0f, getSize().x - 2,
			getSize().y - 2, radius);

		if (getBackgroundColor().A != 0)
		{
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
		renderContext.drawRoundedRect(getPosition().x, getPosition().y, getSize().x,
			getSize().y, radius);

		renderContext.setLinearGradient(getPosition().x, getPosition().y, getPosition().x,
			getPosition().y + getSize().y, gradTop, gradBot);

		renderContext.fillGradient();


		renderContext.beginPath();
		renderContext.setStrokeWidth(1.0f);
		renderContext.drawRoundedRect(getPosition().x + 0.5f, getPosition().y + (_pushed ? 0.5f : 1.5f), getSize().x - 1,
			getSize().y - 1 - (_pushed ? 0.0f : 1.0f), radius);
		renderContext.setStrokeColor(getTheme()->BorderLight);
		renderContext.stroke();

		renderContext.beginPath();
		renderContext.drawRoundedRect(getPosition().x + 0.5f, getPosition().y + 0.5f, getSize().x - 1,
			getSize().y - 2, radius);
		renderContext.setStrokeColor(getTheme()->BorderDark);
		renderContext.stroke();

		/*int fontSize = mFontSize == -1 ? mTheme->mButtonFontSize : mFontSize;
		nvgFontSize(ctx, fontSize);
		nvgFontFace(ctx, "sans-bold");
		float tw = nvgTextBounds(ctx, 0, 0, mCaption.c_str(), nullptr, nullptr);

		Vector2f center = mPos.cast<float>() + mSize.cast<float>() * 0.5f;
		Vector2f textPos(center.x() - tw * 0.5f, center.y() - 1);
		NVGcolor textColor =
			mTextColor.w() == 0 ? mTheme->mTextColor : mTextColor;
		if (!mEnabled)
			textColor = mTheme->mDisabledTextColor;

		nvgFontSize(ctx, fontSize);
		nvgFontFace(ctx, "sans-bold");
		nvgTextAlign(ctx, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
		nvgFillColor(ctx, mTheme->mTextColorShadow);
		nvgText(ctx, textPos.x(), textPos.y(), mCaption.c_str(), nullptr);
		nvgFillColor(ctx, textColor);
		nvgText(ctx, textPos.x(), textPos.y() + 1, mCaption.c_str(), nullptr);*/
	}
}
