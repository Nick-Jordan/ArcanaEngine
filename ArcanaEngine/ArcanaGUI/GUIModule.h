#ifndef GUI_MODULE_H_
#define GUI_MODULE_H_

#include "GUIDefines.h"

#include "ModuleInterface.h"

//vld
#include <vld.h>

/*
 * GUI based on NanoGUI
 */

namespace Arcana
{

	class ARCANA_GUI_API GUIModule : public ModuleInterface
	{
	public:

		GUIModule();

		~GUIModule();

		virtual bool startUp() override;

		virtual bool shutDown() override;

		virtual bool isGameModule() override;

		virtual const char* getModuleName() const override;
	};

}

#endif // !GUI_MODULE_H_

