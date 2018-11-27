#include "Window.h"

#include "ContextCreator.h"
#include "CoreDefines.h"
#include "Application.h"

namespace Arcana
{
	INITIALIZE_CATEGORY(Arcana, WindowLog)

	Window::Window() :
		_windowContext(nullptr),
		_definition(nullptr),
		_renderer(nullptr)
	{

	}

	Window::Window(const WindowDefinition& definition) :
		_windowContext(nullptr),
		_definition(&definition),
		_renderer(nullptr)
	{
		initialize();
	}

	Window::Window(WindowHandle handle) :   ///FIX THIS
		_windowContext(nullptr),
		_definition(nullptr),
		_renderer(nullptr)
	{

	}

	/*Window::Window(const Window& window) :
		_windowContext(window._windowContext),
		_definition(window._definition),
		_running(window._running)
	{

	}*/

	Window::~Window()
	{
		//destroy();
	}

	Vector2i Window::getPosition() const
	{
		if (!_windowContext)
			return Vector2i::zero();

		return _windowContext->getPosition();
	}

	void Window::setPosition(const Vector2i& position)
	{
		if (!_windowContext)
			return;

		return _windowContext->setPosition(position);
	}

	Vector2i Window::getSize() const
	{
		if (!_windowContext)
			return Vector2i::zero();

		return _windowContext->getSize();
	}

	float Window::getAspectRatio() const
	{
		Vector2i size = getSize();

		if (size.y != 0)
		{
			return (float)size.x / (float)size.y;
		}

		return 1.0f;
	}

	void Window::setSize(const Vector2i& size)
	{
		if (!_windowContext)
			return;

		return _windowContext->setSize(size);
	}

	void Window::setTitle(const std::string& title)
	{
		if (!_windowContext)
			return;

		return _windowContext->setTitle(title);
	}

	void Window::setIcon(unsigned int width, unsigned int height, const uint8* pixels)
	{
		if (!_windowContext)
			return;

		return _windowContext->setIcon(width, height, pixels);
	}

	void Window::setVisible(bool visible)
	{
		if (!_windowContext)
			return;

		return _windowContext->setVisible(visible);
	}

	void Window::requestFocus()
	{
		if (!_windowContext)
			return;

		return _windowContext->requestFocus();
	}

	bool Window::hasFocus() const
	{
		if (!_windowContext)
			return false;

		return _windowContext->hasFocus();
	}


	void Window::setMouseCursorVisible(bool visible)
	{
		if (!_windowContext)
			return;

		return _windowContext->setMouseCursorVisible(visible);
	}

	void Window::setLockMouseCursor(bool lock, Recti rect)
	{
		if (!_windowContext)
			return;

		return _windowContext->setLockMouseCursor(lock, rect);
	}

	void Window::repeatKeyEvents(bool repeat)
	{
		if (!_windowContext)
			return;

		return _windowContext->repeatKeyEvents(repeat);
	}


	void Window::initialize()
	{
		//create context

		_windowContext = ContextCreator::createWindowContext();

		_running = 1;

		if (_definition)
		{
			if (!create(*_definition))
			{
				LOG(Error, WindowLog, "Error creating window: definition is invalid.");
			}
		}
		else
		{
			LOG(Error, WindowLog, "Error creating window: definition is nullptr.");
		}
	}

	bool Window::create(const WindowDefinition& definition)
	{
		if (!_windowContext)
		{
			return false;

		}
		return _windowContext->create(definition);
	}

	void Window::close()
	{
		_running = 0;
	}

	bool Window::destroy()
	{
		_running = 0;

		if (_windowContext)
		{
			bool contextDestroyed = _windowContext->destroy();

			AE_DELETE(_windowContext);
			//AE_DELETE(_definition);

			return contextDestroyed;
		}

		return false;
	}

	bool Window::isOpen() const
	{
		return _running != 0;
	}

	void Window::render()
	{
		if (_renderer)
		{
			_renderer->render();
		}
	}

	void Window::setVerticalSync(bool enabled)
	{
		if (_renderer)
		{
			_renderer->setVerticalSync(enabled);
		}
	}

	HWND Window::getWindowHandle() const
	{
		if (!_windowContext)
			return 0;

		return _windowContext->getWindowHandle();
	}

	const WindowDefinition& Window::getWindowDefinition() const
	{
		return *_definition;
	}

	const WindowContext* Window::getWindowContext() const
	{
		return _windowContext;
	}

	void Window::setDefinition(WindowDefinition* definition, bool reinitialize)
	{
		_definition = definition;

		if (reinitialize)
		{
			initialize();
		}
	}

	void Window::setCursor(const Cursor& cursor)
	{
		if (_windowContext)
		{
			_windowContext->setCursor(cursor.getContext());
		}
	}

	void Window::setParent(Application* parent)
	{
		_parent = parent;
	}

	Application* Window::getParent() const
	{
		return _parent;
	}

	bool Window::pollMessage(Message& msg)
	{
		if (_windowContext)
		{
			if (_windowContext->getEventProcessor().popMessage(*_windowContext, msg, false))
			{
				return filterEvent(msg.getEvent());
			}
			else
			{
				return false;
			}
		}

		return false;
	}

	bool Window::waitMessage(Message& msg)
	{
		if (_windowContext)
		{
			if (_windowContext->getEventProcessor().popMessage(*_windowContext, msg, true))
			{
				return filterEvent(msg.getEvent());
			}
			else
			{
				return false;
			}
		}

		return false;
	}

	bool Window::filterEvent(const Event& event)
	{

		return true;
	}


	/*Window& Window::operator=(const Window& window)
	{
		_windowContext = window._windowContext;
		_definition = window._definition;
		_running = window._running;
		return *this;
	}*/
}
