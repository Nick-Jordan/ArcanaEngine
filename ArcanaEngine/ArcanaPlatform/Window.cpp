#include "Window.h"

#include "ContextCreator.h"

namespace Arcana
{
	INITIALIZE_CATEGORY(Arcana, WindowLog)

	Window::Window() : 
		_windowContext(nullptr), 
		_width(0),
		_height(0), 
		_definition(nullptr)
	{

	}

	Window::Window(const WindowDefinition& definition) : 
		_windowContext(nullptr), 
		_width(definition.getWidth()),
		_height(definition.getHeight()),
		_definition(&definition)
	{
		initialize();
	}

	Window::Window(WindowHandle handle) :   ///FIX THIS
		_windowContext(nullptr),
		_width(0),
		_height(0),
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

	const uint32& Window::getWidth() const
	{
		return _width;
	}

	const uint32& Window::getHeight() const
	{
		return _height;
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

}
