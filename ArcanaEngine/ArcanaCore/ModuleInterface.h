#ifndef MODULE_INTERFACE_H_
#define MODULE_INTERFACE_H_

#include "CoreDefines.h"

namespace Arcana
{
	/** \brief Interface for Engine and Game modules.
	 */

	class ARCANA_CORE_API ModuleInterface
	{
	public:

		/** \brief ModuleInterface destructor.
		 */

		virtual ~ModuleInterface() { };

		/** \brief Starts up the module.
		 */
		virtual bool startUp() = 0;

		/** \brief Shuts down the module.
		 */
		virtual bool shutDown() = 0;

		/** \brief Returns true if the module is game specific.
		 */
		virtual bool isGameModule() = 0;
	};
}

#endif
