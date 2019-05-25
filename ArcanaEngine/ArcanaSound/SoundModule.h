#ifndef SOUND_MODULE_H_
#define SOUND_MODULE_H_

#include "SoundDefines.h"

#include "ModuleInterface.h"

//vld
#include <vld.h>

namespace Arcana
{
	class ARCANA_SOUND_API SoundModule : public ModuleInterface
	{
	public:

		SoundModule();

		virtual ~SoundModule();

		virtual bool startUp() override;

		virtual bool shutDown() override;

		virtual bool isGameModule() override;
	};
}

#endif