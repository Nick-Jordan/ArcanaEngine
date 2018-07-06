#include "Window.h"

#include "ContextCreator.h"

namespace Arcana
{
	INITIALIZE_CATEGORY(Arcana, WindowLog)

		Window::Window() :
		_windowContext(nullptr),
		_definition(nullptr)
	{

	}

	Window::Window(const WindowDefinition& definition) :
		_windowContext(nullptr),
		_definition(&definition)
	{
		initialize();
	}

	Window::Window(WindowHandle handle) :   ///FIX THIS
		_windowContext(nullptr),
		_definition(nullptr)
	{

	}

	Window::~Window()
	{
		if (_windowContext)
		{
			//delete _windowContext;
		}

		if (_definition)
		{
			//delete _definition;
		}
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



	void Window::initialize()
	{
		//create context

		_windowContext = ContextCreator::createWindowContext();

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

	bool Window::destroy()
	{
		if (!_windowContext)
			return false;

		return _windowContext->destroy();
	}

	bool Window::isOpen() const
	{
		return _windowContext != nullptr;
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

	void Window::setDefinition(WindowDefinition* definition, bool reinitialize)
	{
		_definition = definition;

		if (reinitialize)
		{
			initialize();
		}
	}

	void Window::setCursor(Cursor* cursor)
	{
		if (_windowContext)
		{
			_windowContext->setCursor(cursor);
		}
	}

	Cursor* Window::getCursor() const
	{
		if (!_windowContext)
			return nullptr;

		return _windowContext->getCursor();
	}

	bool Window::pollMessage(Message& msg)
	{
		if (_windowContext)
		{
			if (_windowContext->getEventProcessor().popMessage(*_windowContext, msg, false))
			{
				//LOG(Debug, CoreEngine, "Test in pollMessage: " + std::to_string(msg.getEvent().getEventId()));

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

}
