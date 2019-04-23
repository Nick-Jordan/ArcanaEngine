#include "Widget.h"

#include "GUIWindow.h"

namespace Arcana
{
	Widget::Widget(Widget* parent) :
		_parent(parent), _theme(nullptr),// _layout(nullptr),
		_position(Vector2i::zero()), _size(Vector2i::zero()),
		_fixedSize(Vector2i::zero()), _visible(true), _enabled(true),
		_focused(false), _mouseFocus(false), _tooltip(""), _fontSize(-1),
		_iconExtraScale(1.0f), _window(nullptr)
	{
		if (_parent)
		{
			if (_parent->getChildIndex(this) == -1)
			{
				_parent->addChild(this);
			}
		}

		if (Theme::Default == nullptr)
		{
			Theme::Default = new Theme();
			setTheme(Theme::Default);
		}
	}

	Widget::~Widget()
	{
		//release everything

		AE_RELEASE(_theme)

		for (auto child : _children)
		{
			AE_RELEASE(child);
		}
	}

	Widget* Widget::getParent()
	{
		return _parent;
	}

	const Widget * Widget::getParent() const
	{
		return _parent;
	}

	void Widget::setParent(Widget* parent)
	{
		if (_parent)
		{
			_parent->removeChild(this);
		}

		_parent = parent;

		if (_parent->getChildIndex(this) == -1)
		{
			_parent->addChild(this);
		}
	}

	/*Layout* Widget::getLayout()
	{
		return _layout;
	}

	const Layout* Widget::getLayout() const
	{
		return _layout;
	}

	void Widget::setLayout(Layout* layout)
	{
		AE_RELEASE(_layout);

		_layout = layout;

		if (_layout)
		{
			_layout->reference();
		}

	}*/

	Theme* Widget::getTheme()
	{
		return _theme;
	}

	const Theme* Widget::getTheme() const
	{
		return _theme;
	}

	void Widget::setTheme(Theme* theme)
	{
		if (_theme == theme)
		{
			return;
		}
		
		AE_RELEASE(_theme);

		_theme = theme;

		if (_theme)
		{
			_theme->reference();
		}

		for (auto child : _children)
		{
			child->setTheme(theme);
		}
	}

	const Vector2i& Widget::getPosition() const
	{
		return _position;
	}

	void Widget::setPosition(const Vector2i& position)
	{
		_position = position;
	}

	Vector2i Widget::getAbsolutePosition() const
	{
		return _parent ? (_parent->getAbsolutePosition() + _position) : _position;
	}

	const Vector2i& Widget::getSize() const
	{
		return _size;
	}

	void Widget::setSize(const Vector2i &size)
	{
		_size = size;
	}

	int32 Widget::getWidth() const
	{
		return _size.x;
	}

	void Widget::setWidth(int32 width)
	{
		_size.x = width;
	}

	int32 Widget::getHeight() const
	{
		return _size.y;
	}

	void Widget::setHeight(int32 height)
	{
		_size.y = height;
	}

	void Widget::setFixedSize(const Vector2i& fixedSize)
	{
		_fixedSize = fixedSize;
	}

	const Vector2i& Widget::getFixedSize() const
	{
		return _fixedSize;
	}

	int32 Widget::getFixedWidth() const
	{
		return _fixedSize.x;
	}

	int32 Widget::getFixedHeight() const
	{
		return _fixedSize.y;
	}

	void Widget::setFixedWidth(int32 width)
	{
		_fixedSize.x = width;
	}

	void Widget::setFixedHeight(int32 height)
	{
		_fixedSize.y = height;
	}

	bool Widget::isVisible(bool recursive) const
	{
		bool visible = _visible;

		if (recursive)
		{
			const Widget* widget = _parent;
			while (widget)
			{
				visible &= widget->isVisible(recursive);
				widget = widget->getParent();
			}
		}
		return visible;
	}

	void Widget::setVisible(bool visible)
	{
		_visible = visible;
	}

	int32 Widget::getChildCount() const
	{
		return _children.size();
	}

	const std::vector<Widget*>& Widget::getChildren() const
	{
		return _children;
	}

	void Widget::addChild(Widget* widget, int32 index)
	{
		if (index <= getChildCount())
		{
			if (widget)
			{
				_children.insert(_children.begin() + index, widget);
				widget->reference();
				widget->setParent(this);
				//widget->setTheme(_theme);
			}
		}
	}

	void Widget::addChild(Widget* widget)
	{
		if (widget)
		{
			_children.push_back(widget);
			widget->reference();
			widget->setParent(this);
			//widget->setTheme(_theme);
		}
	}

	void Widget::removeChild(int32 index)
	{
		if (index < getChildCount() && index >= 0)
		{
			Widget* widget = _children.at(index);
			_children.erase(_children.begin() + index);
			AE_RELEASE(widget);
		}
	}

	void Widget::removeChild(Widget* widget)
	{
		if (widget)
		{
			_children.erase(std::remove(_children.begin(), _children.end(), widget), _children.end());
			AE_RELEASE(widget);
		}
	}

	const Widget* Widget::getChild(int32 index) const
	{
		if (index < getChildCount() && index >= 0)
		{
			return _children.at(index);
		}

		return nullptr;
	}

	Widget* Widget::getChild(int32 index)
	{
		if (index < getChildCount() && index >= 0)
		{
			return _children.at(index);
		}

		return nullptr;
	}

	int32 Widget::getChildIndex(Widget* widget) const
	{
		auto it = std::find(_children.begin(), _children.end(), widget);
		if (it == _children.end())
		{
			return -1;
		}

		return (int32)(it - _children.begin());
	}

	GUIWindow* Widget::getWindow()
	{
		if (!_window)
		{
			Widget* widget = _parent;
			while (widget)
			{
				if (widget->getWindow())
				{
					return widget->getWindow();
				}
				widget = widget->getParent();
			}
		}

		return _window;
	}

	void Widget::setWindow(GUIWindow* window)
	{
		if (_window)
		{
			_window->removeWidget(this);
		}

		_window = window;
	}

	void Widget::setId(const std::string& id)
	{
		_id = GlobalObjectID(id);
	}

	const GlobalObjectID& Widget::getId() const
	{
		return _id;
	}

	bool Widget::isEnabled() const
	{
		return _enabled;
	}

	void Widget::setEnabled(bool enabled)
	{
		_enabled = enabled;
	}

	bool Widget::isFocused() const
	{
		return _focused;
	}

	void Widget::setFocused(bool focused)
	{
		_focused = focused;
	}

	void Widget::requestFocus()
	{
		
	}

	const std::string& Widget::getTooltip() const
	{
		return _tooltip;
	}

	void Widget::setTooltip(const std::string& tooltip)
	{
		_tooltip = tooltip;
	}

	int32 Widget::getFontSize() const
	{
		return  (_fontSize < 0 && _theme) ? _theme->StandardFontSize : _fontSize;
	}

	void Widget::setFontSize(int32 fontSize)
	{
		_fontSize = fontSize;
	}

	bool Widget::hasFontSize() const
	{
		return _fontSize > 0;
	}

	float Widget::getIconExtraScale() const
	{
		return _iconExtraScale;
	}

	void Widget::setIconExtraScale(float scale)
	{
		_iconExtraScale = scale;
	}

	bool Widget::hasMouseFocus() const
	{
		return _mouseFocus;
	}

	void Widget::setMouseFocus(bool mouseFocus)
	{
		_mouseFocus = mouseFocus;
	}

	//cursor????

	bool Widget::contains(const Vector2i& p) const
	{
		Recti bounds(_position, _size);

		return bounds.contains(p);
	}

	Widget* Widget::findWidget(const Vector2i& p)
	{
		for (auto it = _children.rbegin(); it != _children.rend(); ++it)
		{
			Widget* child = *it;

			if (child->isVisible(false) && child->contains(p - _position))
			{
				return child->findWidget(p - _position);
			}
		}
		return contains(p) ? this : nullptr;
	}

	bool Widget::mouseButtonEvent(const Vector2i &p, Key button, bool down, ModifierKeysState modifiers)
	{
		for (auto it = _children.rbegin(); it != _children.rend(); ++it)
		{
			Widget* child = *it;

			if (child->isVisible(false) && child->contains(p - _position) &&
				child->mouseButtonEvent(p - _position, button, down, modifiers))
			{
				return true;
			}
		}

		if (button.getKeyCode() == KeyCode::LeftMouseButton && down && !_focused)
		{
			requestFocus();
		}

		return false;
	}

	bool Widget::mouseMotionEvent(const Vector2i &p, const Vector2i &rel)
	{
		for (auto it = _children.rbegin(); it != _children.rend(); ++it)
		{
			Widget* child = *it;
			if (!child->isVisible(false))
			{
				continue;
			}

			bool contained = child->contains(p - _position);
			bool prevContained = child->contains(p - _position - rel);

			if (contained != prevContained)
			{
				child->mouseEnterEvent(p, contained);
			}

			if ((contained || prevContained) &&
				child->mouseMotionEvent(p - _position, rel))
			{
				return true;
			}
		}

		return false;
	}

	bool Widget::mouseDragEvent(const Vector2i &p, const Vector2i &rel, Key button, ModifierKeysState modifiers)
	{
		return false;
	}

	bool Widget::mouseEnterEvent(const Vector2i &p, bool enter)
	{
		_mouseFocus = enter;
		return false;
	}

	bool Widget::scrollEvent(const Vector2i &p, float delta, MouseEvent::Wheel wheel)
	{
		for (auto it = _children.rbegin(); it != _children.rend(); ++it)
		{
			Widget* child = *it;
			if (!child->isVisible(false))
			{
				continue;
			}

			if (child->contains(p - _position) && child->scrollEvent(p - _position, delta, wheel))
			{
				return true;
			}
		}

		return false;
	}

	bool Widget::focusEvent(bool focused)
	{
		_focused = focused;
		return false;
	}

	bool Widget::keyEvent(Key key, KeyEvent::Type action, ModifierKeysState modifiers)
	{
		return false;
	}

	Vector2i Widget::preferredSize(GUIRenderContext& renderContext) const
	{
		/*if (_layout)
		{
			return _layout->preferredSize(renderContext, this);
		}*/

		return _size;
	}

	void Widget::performLayout(GUIRenderContext& renderContext)
	{
		/*if (_layout)
		{
			_layout->performLayout(renderContext, this);
		}
		else
		{
			for (auto c : _children)
			{
				Vector2i pref = c->preferredSize(renderContext);
				Vector2i fix = c->getFixedSize();
				c->setSize(Vector2i(
					fix.x ? fix.x : pref.x,
					fix.y ? fix.y : pref.y
				));
				c->performLayout(renderContext);
			}
		}*/
	}

	void Widget::render(GUIRenderContext& renderContext)
	{
		if (_children.size() == 0)
		{
			return;
		}

		for (auto child : _children)
		{
			if (child->isVisible(false))
			{
				//set clip
				child->render(renderContext);
			}
		}
	}
}
