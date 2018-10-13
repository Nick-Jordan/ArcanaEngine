#ifndef CORE_MODULE_H_
#define CORE_MODULE_H_

#include "CoreDefines.h"

#include "ModuleInterface.h"

namespace Arcana
{
	/** \brief The module interface for the ArcanaCore module.
	 */

	class ARCANA_CORE_API CoreModule : public ModuleInterface
	{
	public:

		/** \brief CoreModule default constructor.
		 */

		CoreModule();

		/** \brief CoreModule destructor.
		 */

		~CoreModule();

		/** \brief Starts CoreModule subsystems.
		 */

		virtual bool startUp() override;

		/** \brief Shuts down CoreModule subsystems.
		 */

		virtual bool shutDown() override;

		/** \brief Returns false.
		 *  The CoreModule is an ArcanaEngine module.
		 */

		virtual bool isGameModule() override;
	};
}

#endif