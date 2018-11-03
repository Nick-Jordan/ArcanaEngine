#ifndef SCENE_MODULE_H_
#define SCENE_MODULE_H_

#include "SceneDefines.h"

#include "ModuleInterface.h"

//vld
#include <vld.h>

namespace Arcana
{
	class ARCANA_SCENE_API SceneModule : public ModuleInterface
	{
	public:

		SceneModule();

		virtual ~SceneModule();

		virtual bool startUp() override;

		virtual bool shutDown() override;

		virtual bool isGameModule() override;
	};
}

#endif