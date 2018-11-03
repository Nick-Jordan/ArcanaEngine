#ifndef IO_MODULE_H_
#define IO_MODULE_H_

#include "IODefines.h"

#include "ModuleInterface.h"

//vld
#include <vld.h>

namespace Arcana
{

	class ARCANA_IO_API IOModule : public ModuleInterface
	{
	public:

		IOModule();

		~IOModule();

		virtual bool startUp() override;

		virtual bool shutDown() override;

		virtual bool isGameModule() override;
	};

}

#endif // !IO_MODULE_H_

