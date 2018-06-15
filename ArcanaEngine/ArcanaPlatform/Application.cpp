#include "Application.h"

#include "ContextCreator.h"

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
	}

	void Application::initialize()
	{
		//create context

		_applicationContext = ContextCreator::createApplicationContext();

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
			LOG(Info, ApplicationLog, std::to_string((*it).get().getWidth()));
			_windows.add(Window(*it));
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
}
