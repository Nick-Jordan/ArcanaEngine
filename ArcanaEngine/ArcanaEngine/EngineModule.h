#ifndef ENGINE_MODULE_H_
#define ENGINE_MODULE_H_

#include "EngineDefines.h"

#include "ModuleInterface.h"

//vld
#include <vld.h>

namespace Arcana
{
	/** \brief The module interface for the ArcanaEngine module.
	*/

	class ARCANA_ENGINE_API EngineModule : public ModuleInterface
	{
	public:

		/** \brief EngineModule default constructor.
		*/

		EngineModule();

		/** \brief EngineModule destructor.
		*/

		~EngineModule();

		/** \brief Starts EngineModule subsystems.
		*/

		virtual bool startUp() override;

		/** \brief Shuts down EngineModule subsystems.
		*/

		virtual bool shutDown() override;

		/** \brief Returns false.
		*  The EngineModule is an ArcanaEngine module.
		*/

		virtual bool isGameModule() override;

		virtual const char* getModuleName() const override;
	};
}

#endif