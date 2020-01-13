#include "GUIWindow.h"

#include "Input.h"
#include "Panel.h"

namespace Arcana
{
	GUIWindow::GUIWindow(Application* application, const std::string& name, int32 width, int32 height) : Actor(), Widget(nullptr), _firstMouseEvent(true)
	{
		_eventListener = new GUIEventListener(this);

		if (application)
		{
			application->getEventHandler().addEventListener(std::shared_ptr<GUIEventListener>(_eventListener));

			if (width == -1)
				width = application->getActiveWindow().getSize().x;
			if (height == -1)
				height = application->getActiveWindow().getSize().y;

			setSize(Vector2i(width, height));
		}

		initialize(name);
		setActive(true);

		_eventListener->listenForEvent(EventID::KeyEventID);
		_eventListener->listenForEvent(EventID::MouseEventID);
	}


	GUIWindow::~GUIWindow()
	{
	}

	void GUIWindow::initialize(std::string name, const Actor* templateActor)
	{
		Actor::initialize(name, templateActor);

		_renderContext.initialize(getSize());

		_renderProcedure = new GUIRenderProcedure(this);
		_renderProcedure->Properties.RendererStage = "GraphicalUserInterfaceStage";
		_renderProcedure->Properties.RenderState.setCullEnabled(false);
		_renderProcedure->Properties.RenderState.setCullFaceSide(RenderState::Back);
		_renderProcedure->Properties.RenderState.setDepthTestEnabled(true);
		_renderProcedure->Properties.RenderState.setBlendEnabled(true);
		_renderProcedure->Properties.RenderState.setBlendSrc(RenderState::SrcAlpha);
		_renderProcedure->Properties.RenderState.setBlendDst(RenderState::OneMinusSrcAlpha);
		_renderProcedure->Properties.LightProperties.CastsDynamicShadow = false;
	}

	void GUIWindow::update(double elapsedTime)
	{

	}

	void GUIWindow::render(ObjectRenderer& renderer)
	{
		/*MeshRenderContext context;

		context.mesh = nullptr;
		context.material = nullptr;
		context.renderProperties.lightProperties.CastsDynamicShadow = false;
		context.renderProperties.rendererStage = "GraphicalUserInterfaceStage";
		context.renderProperties.renderState.setCullEnabled(true);
		context.renderProperties.renderState.setCullFaceSide(RenderState::Back);
		context.renderProperties.renderState.setDepthTestEnabled(false);
		context.renderProperties.renderState.setBlendEnabled(true);
		context.renderProperties.renderState.setBlendSrc(RenderState::SrcAlpha);
		context.renderProperties.renderState.setBlendDst(RenderState::OneMinusSrcAlpha);

		context.callback.bind(&_renderer, &GUIWindowRenderer::draw);

		renderer.addMesh(context);*/

		renderer.addProcedure(_renderProcedure);
	}
	
	GUIEventListener::GUIEventListener(GUIWindow* window) : _window(window)
	{

	}

	GUIRenderProcedure::GUIRenderProcedure(GUIWindow* window) : _window(window)
	{

	}

	GUIRenderProcedure::~GUIRenderProcedure()
	{

	}

	void GUIRenderProcedure::render()
	{
		Properties.RenderState.bind();
		for (auto it = _window->getWidgets().begin(); it != _window->getWidgets().end(); ++it)
		{
			Widget* widget = *it;
			widget->render(_window->_renderContext);
		}

		_window->_renderContext.render();
		Properties.RenderState.unbind();
	}

	bool GUIRenderProcedure::isValidProcedure()
	{
		return _window != nullptr;
	}

	void GUIWindow::destroyed()
	{

	}


	bool GUIEventListener::processEvent(Event& event, EventHandler& handler)
	{
		/*

		virtual bool mouseDragEvent(const Vector2i &p, const Vector2i &rel, Key button, ModifierKeysState modifiers);

		virtual bool mouseEnterEvent(const Vector2i &p, bool enter);

		virtual bool focusEvent(bool focused);

		*/

		if (_window->_firstMouseEvent
			&& event.getEventId() == EventID::MouseEventID
			&& event.getInt("event") == MouseEvent::Moved)
		{
			_window->_lastMouse = Vector2i(event.getInt("x"), event.getInt("y"));
			_window->_firstMouseEvent = false;
		}

		if (event.getEventId() == EventID::MouseEventID)
		{
			int32 type = event.getInt("event");
			if (type == MouseEvent::Moved)
			{
				Vector2i newPosition = Vector2i(event.getInt("x"), event.getInt("y"));
				Vector2i rel = newPosition - _window->_lastMouse;

				bool drag = Input::isKeyPressed(Keys::LeftMouseButton);

				if (rel.magnitudeSq() > 0)
				{
					for (auto it = _window->Widget::getChildren().rbegin(); it != _window->Widget::getChildren().rend(); ++it)
					{
						Widget* widget = *it;
						widget->mouseMotionEvent(newPosition, rel);

						if (drag)
						{
							if (_window->_focusPath.size() > 1)
							{
								const Panel* panel = dynamic_cast<Panel*>(_window->_focusPath[_window->_focusPath.size() - 2]);
								if (panel && panel->isModal())
								{
									if (!panel->contains(newPosition))
									{
										return false;
									}
								}
							}

							widget->mouseDragEvent(newPosition, rel, Keys::LeftMouseButton, ModifierKeysState());
						}

						if (widget->contains(newPosition))
						{
							if (!widget->hasMouseFocus())
							{
								widget->mouseEnterEvent(newPosition, true);
							}
						}
						else
						{
							if (widget->hasMouseFocus())
							{
								widget->mouseEnterEvent(newPosition, false);
							}
						}
					}
				}

				_window->_lastMouse = newPosition;
			}
			else if (type == MouseEvent::WheelScrolled)
			{
				Vector2i position = Vector2i(event.getInt("x"), event.getInt("y"));
				float delta = event.getFloat("delta");
				MouseEvent::Wheel wheel = (MouseEvent::Wheel) event.getInt("wheel");

				for (auto it = _window->Widget::getChildren().rbegin(); it != _window->Widget::getChildren().rend(); ++it)
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

			ModifierKeysState modifierKeysState(shift, shift, control, control, alt, alt, system, system, false);

			if (key.isMouseButton())
			{
				Vector2i p = Vector2i(event.getInt("x"), event.getInt("y"));

				if (_window->_focusPath.size() > 1)
				{
					const Panel* panel = dynamic_cast<Panel*>(_window->_focusPath[_window->_focusPath.size() - 2]);
					if (panel && panel->isModal())
					{
						if (!panel->contains(p))
						{
							return false;
						}
					}
				}


				for (auto it = _window->Widget::getChildren().rbegin(); it != _window->Widget::getChildren().rend(); ++it)
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
			}
			else
			{
				if (_window->_focusPath.size() > 0)
				{
					for (auto it = _window->_focusPath.rbegin() + 1; it != _window->_focusPath.rend(); ++it)
					{
						if ((*it)->isFocused() && (*it)->keyEvent(key, type, modifierKeysState))
						{
							return true;
						}
					}
				}
			}
		}

		return true;
	}

	int32 GUIWindow::getWidgetCount() const
	{
		return getChildCount();
	}

	const std::vector<Widget*>& GUIWindow::getWidgets() const
	{
		return Widget::getChildren();
	}

	void GUIWindow::addWidget(Widget* widget, int32 index)
	{
		return Widget::addChild(widget, index);
	}

	void GUIWindow::addWidget(Widget* widget)
	{
		return Widget::addChild(widget);
	}

	void GUIWindow::removeWidget(int32 index)
	{
		return Widget::removeChild(index);
	}

	void GUIWindow::removeWidget(Widget* widget)
	{
		return Widget::removeChild(widget);
	}

	const Widget* GUIWindow::getWidget(int32 index) const
	{
		return Widget::getChild(index);
	}

	Widget* GUIWindow::getWidget(int32 index)
	{
		return Widget::getChild(index);
	}

	int32 GUIWindow::getWidgetIndex(Widget* widget) const
	{
		return getChildIndex(widget);
	}

	void GUIWindow::updateLayout()
	{
		Widget::performLayout(_renderContext);
	}

	void GUIWindow::updateFocus(Widget* widget)
	{
		for (auto w : _focusPath)
		{
			if (!w->isFocused())
				continue;

			w->focusEvent(false);
		}

		_focusPath.clear();
		Widget* window = nullptr;

		while (widget)
		{
			_focusPath.push_back(widget);
			if (dynamic_cast<Panel*>(widget))
			{
				window = widget;
			}

			widget = widget->getParent();
		}
		for (auto it = _focusPath.rbegin(); it != _focusPath.rend(); ++it)
		{
			(*it)->focusEvent(true);
		}

		if (window)
		{
			movePanelToFront((Panel*)window);
		}
	}

	void GUIWindow::disposePanel(Panel* panel)
	{
		if (std::find(_focusPath.begin(), _focusPath.end(), panel) != _focusPath.end())
		{
			_focusPath.clear();
		}
		removeChild(panel);
	}

	void GUIWindow::centerPanel(Panel* panel)
	{
		if (panel->getSize() == Vector2i::zero())
		{
			panel->setSize(panel->preferredSize(_renderContext));
			panel->performLayout(_renderContext);
		}
		panel->setPosition((getSize() - panel->getSize()) / 2);
	}

	void GUIWindow::movePanelToFront(Panel* panel)
	{
		panel->reference();
		removeChild(panel);
		addWidget(panel);

		bool changed = false;
		do {
			size_t baseIndex = 0;
			for (size_t index = 0; index < Widget::getChildren().size(); ++index)
			{
				if (Widget::getChildren()[index] == panel)
				{
					baseIndex = index;
				}
			}

			changed = false;

			for (size_t index = 0; index < Widget::getChildren().size(); ++index)
			{
				/*Popup* pw = dynamic_cast<Popup *>(Widget::getChildren()[index]);
				if (pw && pw->parentWindow() == window && index < baseIndex) {
					moveWindowToFront(pw);
					changed = true;
					break;
				}*/
			}

		} while (changed);
	}
}
