#include "GUIWindow.h"

namespace Arcana
{

	GUIWindow::GUIWindow(Application* application, const std::string& name, int32 width, int32 height) : Actor(), _firstMouseEvent(true)
	{
		if (application)
		{
			application->getEventHandler().addEventListener(std::shared_ptr<GUIWindow>(this));

			if (width == -1)
				width = application->getActiveWindow().getSize().x;
			if (height == -1)
				height = application->getActiveWindow().getSize().y;
		}

		initialize(name);
		setActive(true);

		listenForEvent(EventID::KeyEventID);
		listenForEvent(EventID::MouseEventID);
	}


	GUIWindow::~GUIWindow()
	{
	}

	void GUIWindow::initialize(std::string name, const Actor* templateActor)
	{
		Actor::initialize(name, templateActor);

		_renderContext.initialize();
	}

	void GUIWindow::update(double elapsedTime)
	{

	}

	void GUIWindow::render(ObjectRenderer& renderer, Matrix4d view, Matrix4d projection, Vector3d eyePosition)
	{
		_renderContext.start();

		for (auto it = _widgets.begin(); it != _widgets.end(); ++it)
		{
			Widget* widget = *it;
			widget->render(_renderContext);
		}

		_renderContext.finish();

		_renderContext.draw(renderer);
	}

	void GUIWindow::destroyed()
	{

	}


	bool GUIWindow::processEvent(Event& event, EventHandler& handler)
	{
		/*

		virtual bool mouseDragEvent(const Vector2i &p, const Vector2i &rel, Key button, ModifierKeysState modifiers);

		virtual bool mouseEnterEvent(const Vector2i &p, bool enter);

		virtual bool focusEvent(bool focused);

		*/

		if (_firstMouseEvent 
			&& event.getEventId() == EventID::MouseEventID 
			&& event.getInt("event") == MouseEvent::Moved)
		{
			_lastMouse = Vector2i(event.getInt("x"), event.getInt("y"));
			_firstMouseEvent = false;
		}

		if (event.getEventId() == EventID::MouseEventID)
		{
			int32 type = event.getInt("event");
			if (type == MouseEvent::Moved)
			{
				Vector2i newPosition = Vector2i(event.getInt("x"), event.getInt("y"));
				Vector2i rel = newPosition - _lastMouse;

				if (rel.magnitudeSq() > 0)
				{
					for (auto it = _widgets.begin(); it != _widgets.end(); ++it)
					{
						Widget* widget = *it;
						widget->mouseMotionEvent(newPosition, rel);
					}
				}

				_lastMouse = newPosition;
			}
			else if (type == MouseEvent::WheelScrolled)
			{
				Vector2i position = Vector2i(event.getInt("x"), event.getInt("y"));
				float delta = event.getFloat("delta");
				MouseEvent::Wheel wheel = (MouseEvent::Wheel) event.getInt("wheel");
				
				for (auto it = _widgets.begin(); it != _widgets.end(); ++it)
				{
					Widget* widget = *it;
					widget->scrollEvent(position, delta, wheel);
				}
			}
		}
		else if (event.getEventId() == EventID::KeyEventID)
		{
			KeyEvent::Type type = (KeyEvent::Type)event.getInt("event");
			Key key = Keys::get(event.getInt("keyCode"));
			bool shift = event.getBool("shift");
			bool control = event.getBool("control");
			bool alt = event.getBool("alt");
			bool system = event.getBool("system");

			ModifierKeysState modifierKeysState(shift,shift,control,control,alt,alt,system,system,false);

			if (key.isMouseButton())
			{
				Vector2i p = Vector2i(event.getInt("x"), event.getInt("y"));

				for (auto it = _widgets.begin(); it != _widgets.end(); ++it)
				{
					Widget* widget = *it;

					if (widget->isVisible(false) && widget->contains(p) &&
						widget->mouseButtonEvent(p, key, type == KeyEvent::Pressed, modifierKeysState))
					{
						return true;
					}
					else if (widget->isVisible(false) && type == KeyEvent::Released)
					{
						widget->mouseButtonEvent(p, key, false, modifierKeysState);
					}
				}
				//if (button == button1 && down && !_focused)
				//	requestFocus();
			}
			else
			{
				for (auto it = _widgets.begin(); it != _widgets.end(); ++it)
				{
					Widget* widget = *it;
					widget->keyEvent(key, type, modifierKeysState);
				}
			}
		}

		return true;
	}

	int32 GUIWindow::getWidgetCount() const
	{
		return _widgets.size();
	}

	const std::vector<Widget*>& GUIWindow::getWidgets() const
	{
		return _widgets;
	}

	void GUIWindow::addWidget(Widget* widget, int32 index)
	{
		if (index <= getWidgetCount())
		{
			if (widget)
			{
				_widgets.insert(_widgets.begin() + index, widget);
				widget->reference();
				widget->setWindow(this);
			}
		}
	}

	void GUIWindow::addWidget(Widget* widget)
	{
		if (widget)
		{
			_widgets.push_back(widget);
			widget->reference();
			widget->setWindow(this);
		}
	}

	void GUIWindow::removeWidget(int32 index)
	{
		if (index < getWidgetCount() && index >= 0)
		{
			Widget* widget = _widgets.at(index);
			_widgets.erase(_widgets.begin() + index);
			AE_RELEASE(widget);
		}
	}

	void GUIWindow::removeWidget(Widget* widget)
	{
		if (widget)
		{
			_widgets.erase(std::remove(_widgets.begin(), _widgets.end(), widget), _widgets.end());
			AE_RELEASE(widget);
		}
	}

	const Widget* GUIWindow::getWidget(int32 index) const
	{
		if (index < getWidgetCount() && index >= 0)
		{
			return _widgets.at(index);
		}

		return nullptr;
	}

	Widget* GUIWindow::getWidget(int32 index)
	{
		if (index < getWidgetCount() && index >= 0)
		{
			return _widgets.at(index);
		}

		return nullptr;
	}

	int32 GUIWindow::getWidgetIndex(Widget* widget) const
	{
		auto it = std::find(_widgets.begin(), _widgets.end(), widget);
		if (it == _widgets.end())
		{
			return -1;
		}

		return (int32)(it - _widgets.begin());
	}
}
