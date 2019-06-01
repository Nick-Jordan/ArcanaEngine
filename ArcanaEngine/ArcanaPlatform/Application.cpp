#include "Application.h"

#include "ContextCreator.h"
#include "Input.h"

namespace Arcana
{
	INITIALIZE_CATEGORY(Arcana, ApplicationLog)


	Application::Application() :
		_applicationContext(nullptr),
		_definition(nullptr)
	{

	}

	Application::Application(const ApplicationDefinition& definition) :
		_applicationContext(nullptr),
		_definition(&definition)
	{
		initialize();
	}

	Application::~Application()
	{
		if (_applicationContext)
		{
			delete _applicationContext;
		}

		if (_definition)
		{
			//delete _definition;
		}

		for (auto it = _windows.createIterator(); it; ++it)
		{
			if (!(*it).destroy())
			{
				LOG(Error, ApplicationLog, "Error destroying application window.");
			}
		}
	}

	void Application::initialize()
	{
		//create context

		_applicationContext = ContextCreator::createApplicationContext();

		_windowCloseListener = new WindowCloseListener();
		_windowCloseListener->setParentApplication(this);

		_eventHandler.addEventListener(std::shared_ptr<EventListener>(_windowCloseListener));

		if (_definition)
		{
			if (!create(*_definition))
			{
				LOG(Error, ApplicationLog, "Error creating application: definition is invalid.");
			}
		}
		else
		{
			LOG(Error, ApplicationLog, "Error creating application: definition is nullptr.");
		}
	}

	bool Application::create(const ApplicationDefinition& definition)
	{
		if (!_applicationContext)
		{
			return false;

		}
		
		bool returnValue = _applicationContext->create(definition);

		LOG(Info, ApplicationLog, "Creating Windows...");
		for (auto it = definition.getWindowDefinitionArray().createConstIterator(); it; ++it)
		{
			LOGF(Info, ApplicationLog, "width: %d, height: %d", (*it).get().getWidth(), (*it).get().getHeight());
			_windows.add(Window(*it));
			_windows.getLast().setParent(this);
		}

		return returnValue;
	}

	const ApplicationDefinition& Application::getApplicationDefinition() const
	{
		return *_definition;
	}

	void Application::setDefinition(ApplicationDefinition* definition, bool reinitialize)
	{
		_definition = definition;

		if (reinitialize)
		{
			initialize();
		}
	}

	Window& Application::getActiveWindow()
	{
		return _windows[0];
	}

	EventHandler& Application::getEventHandler()
	{
		return _eventHandler;
	}

	void Application::setCloseOperation(CloseOperation operation)
	{
		_windowCloseListener->setCloseOperation(operation);
	}

	CloseOperation Application::getCloseOperation() const
	{
		return _windowCloseListener->getCloseOperation();
	}

	void Application::start()
	{
		if (_windows.size() > 0)
		{
			Window& window = getActiveWindow();

			while (window.isOpen())
			{
				Message msg;
				while(window.pollMessage(msg))
				{
					_eventHandler.broadcast(msg.getEvent());
				}

				window.render();
			}
		}
	}
}
