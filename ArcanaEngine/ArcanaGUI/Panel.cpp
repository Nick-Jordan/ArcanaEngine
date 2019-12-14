#include "Panel.h"

#include "GUIWindow.h"

namespace Arcana
{

	Panel::Panel(Widget *parent, const std::string& title)
		: Widget(parent), _title(title), _buttonPanel(nullptr), _modal(false), _drag(false), _dropShadow(true), _backgroundColor(0, 0, 0, 0)
	{
	}

	Panel::~Panel()
	{
		dispose();
	}

	const std::string& Panel::getTitle() const
	{
		return _title;
	}

	void Panel::setTitle(const std::string& title)
	{
		_title = title;
	}

	bool Panel::isModal() const
	{
		return _modal;
	}

	void Panel::setModal(bool modal)
	{
		_modal = modal;
	}

	bool Panel::hasDropShadow() const
	{
		return _dropShadow;
	}

	void Panel::setDropShadow(bool dropShadow)
	{
		_dropShadow = dropShadow;
	}

	const Color& Panel::getBackgroundColor() const
	{
		return _backgroundColor;
	}

	void Panel::setBackgroundColor(const Color& backgroundColor)
	{
		_backgroundColor = backgroundColor;
	}

	Vector2i Panel::preferredSize(GUIRenderContext& renderContext) const
	{
		if (_buttonPanel)
		{
			_buttonPanel->setVisible(false);
		}

		Vector2i result = Widget::preferredSize(renderContext);

		if (_buttonPanel)
		{
			_buttonPanel->setVisible(true);
		}

		return getSize();
	}

	Widget* Panel::getButtonPanel()
	{
		if (!_buttonPanel)
		{
			_buttonPanel = new Widget(this);
			_buttonPanel->setLayout(new BoxLayout(Layout::Orientation::Horizontal, Layout::Alignment::Middle, 0, 4));
		}
		return _buttonPanel;
	}

	void Panel::performLayout(GUIRenderContext& renderContext)
	{
		if (!_buttonPanel)
		{
			Widget::performLayout(renderContext);
		}
		else
		{
			_buttonPanel->setVisible(false);
			Widget::performLayout(renderContext);
			for (auto w : _buttonPanel->getChildren())
			{
				w->setFixedSize(Vector2i(22, 22));
				w->setFontSize(15);
			}
			_buttonPanel->setVisible(true);
			_buttonPanel->setSize(Vector2i(getWidth(), 22));
			_buttonPanel->setPosition(Vector2i(getWidth() - (_buttonPanel->preferredSize(renderContext).x + 5), 4));
			_buttonPanel->performLayout(renderContext);
		}
	}

	void Panel::render(GUIRenderContext& renderContext)
	{
		renderContext.reset();

		int32 ds = getTheme()->PanelDropShadowSize;
		float cr = getTheme()->PanelCornerRadius;
		int32 hh = getTheme()->PanelHeaderHeight;

		renderContext.setPrimaryColor(hasMouseFocus() ? getTheme()->PanelFillFocused : getTheme()->PanelFillUnfocused);
		renderContext.drawRoundedRect(getAbsolutePosition().x, getAbsolutePosition().y, getSize().x, getSize().y, cr);

		if (_backgroundColor.A != 0)
		{
			renderContext.setPrimaryColor(_backgroundColor);
			renderContext.drawRoundedRect(getAbsolutePosition().x, getAbsolutePosition().y, getSize().x, getSize().y, cr);
		}

		if (!_title.empty())
		{
			renderContext.setPrimaryColor(getTheme()->PanelHeaderGradientTop);
			renderContext.setSecondaryColor(getTheme()->PanelHeaderGradientBot);
			renderContext.setLinearGradient(0.0, 1.0, false, false);
			renderContext.drawRoundedRect(getAbsolutePosition().x, getAbsolutePosition().y, getSize().x, hh, cr);

			Font* font = getTheme()->NormalFont;

			Rectf area0 = Rectf(getAbsolutePosition().cast<float>(), Vector2f(getSize().x, hh));
			Rectf area1 = Rectf(Vector2f(area0.getLeft(), area0.getTop() + 2), area0.getSize());

			font->start();
			//somehow blur??
			font->drawText(_title, area1, getTheme()->DropShadow, 24, Font::Justify::AlignVerticalHorizontalCenter, false, false, area1, renderContext.getCurrentZ());
			font->drawText(_title, area0, isFocused() ? getTheme()->PanelTitleFocused
				: getTheme()->PanelTitleUnfocused, 24, Font::Justify::AlignVerticalHorizontalCenter, false, false, area0, renderContext.getCurrentZ());
			font->finish();

		}

		Widget::render(renderContext);
	}

	void Panel::center()
	{
		Widget* widget = this;
		while (widget->getParent())
		{
			widget = widget->getParent();
		}

		GUIWindow* window = dynamic_cast<GUIWindow*>(widget);

		if (window)
		{
			window->centerPanel(this);
		}
	}

	void Panel::dispose()
	{
		Widget* widget = this;
		while (widget->getParent())
		{
			widget = widget->getParent();
		}

		GUIWindow* window = dynamic_cast<GUIWindow*>(widget);

		if (window)
		{
			window->disposePanel(this);
		}
	}

	bool Panel::mouseDragEvent(const Vector2i &p, const Vector2i &rel, Key button, ModifierKeysState modifiers)
	{
		if (_drag && button.getKeyCode() == KeyCode::LeftMouseButton)
		{
			setPosition(getPosition() + rel);
			setPosition(Vector2i(Math::max(getPosition().x, 0), Math::max(getPosition().y, 0)));
			Widget* parent = getParent();
			if (parent)
			{
				setPosition(Vector2i(Math::min(getPosition().x, parent->getSize().x - getSize().x), Math::min(getPosition().y, parent->getSize().x - getSize().x)));
			}
			return true;
		}
		return false;
	}

	bool Panel::mouseButtonEvent(const Vector2i &p, Key button, bool down, ModifierKeysState modifiers)
	{
		if (Widget::mouseButtonEvent(p, button, down, modifiers))
		{
			return true;
		}

		if (button.getKeyCode() == KeyCode::LeftMouseButton)
		{
			_drag = down && (p.y - getPosition().y) < getTheme()->PanelHeaderHeight;
			return true;
		}
		return false;
	}

	bool Panel::scrollEvent(const Vector2i &p, float delta, MouseEvent::Wheel wheel)
	{
		Widget::scrollEvent(p, delta, wheel);
		return true;
	}

	void Panel::refreshRelativePlacement()
	{
	}

}