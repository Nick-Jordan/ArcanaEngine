#ifndef INPUT_MODULE_H_
#define INPUT_MODULE_H_

#include "InputDefines.h"

#include "ModuleInterface.h"

namespace Arcana
{
	class ARCANA_INPUT_API InputModule : public ModuleInterface
	{
	public:

		InputModule();

		virtual ~InputModule();

		virtual bool startUp() override;

		virtual bool shutDown() override;

		virtual bool isGameModule() override;
	};
}

#endif // !INPUT_MODULE_H_
