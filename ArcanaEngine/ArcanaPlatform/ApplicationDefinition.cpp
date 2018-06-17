#include "ApplicationDefinition.h"

namespace Arcana
{

	ApplicationDefinition::ApplicationDefinition() : _appName("Application")
	{
	}


	ApplicationDefinition::~ApplicationDefinition()
	{
	}

	const std::string& ApplicationDefinition::getAppName() const
	{
		return _appName;
	}

	void ApplicationDefinition::setAppName(const std::string& name)
	{
		_appName = name;
	}

	void ApplicationDefinition::addWindowDefinition(WindowDefinition& definition, bool resetTitle)
	{
		if(resetTitle)
		{
			definition.setTitle(_appName);
		}

		updateWindowDefinitionData(definition);

		_windowDefinitions.add(definition);
	}

	const Array<ApplicationDefinition::WindowDefinitionRefWrapper>& ApplicationDefinition::getWindowDefinitionArray() const
	{
		return _windowDefinitions;
	}
}