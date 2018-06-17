#ifndef APPLICATION_DEFINITION_H_
#define APPLICATION_DEFINITION_H_

#include "PlatformDefines.h"

#include "WindowDefinition.h"
#include "Array.h"

#include <string>

namespace Arcana
{

	class ARCANA_PLATFORM_API ApplicationDefinition
	{
	public:

		typedef std::reference_wrapper<WindowDefinition> WindowDefinitionRefWrapper;


		ApplicationDefinition();

		virtual ~ApplicationDefinition();

		virtual void updateWindowDefinitionData(WindowDefinition& definition) = 0;


		const std::string& getAppName() const;

		void setAppName(const std::string& name);

		void addWindowDefinition(WindowDefinition& definition, bool resetTitle = true);

		const Array<WindowDefinitionRefWrapper>& getWindowDefinitionArray() const;

	private:

		std::string _appName;

		Array<WindowDefinitionRefWrapper> _windowDefinitions;
	};

}

#endif // !APPLICATION_DEFINITION_H_
