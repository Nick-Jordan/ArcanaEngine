#ifndef MAIN_ENGINE_LOOP_H_
#define MAIN_ENGINE_LOOP_H_

#include "CoreDefines.h"

#include "EngineLoop.h"

namespace Arcana
{

	/** \brief The main engine loop.
	 *
	 *  (EXTENDED DESCRIPTION)
	 */

	class ARCANA_CORE_API MainEngineLoop : public EngineLoop
	{
	public:

		/** \brief MainEngineLoop default constructor.
		 */
		
		MainEngineLoop();
		
		/** \brief preInitialize
		 */

		int32 preInitialize(int32 argc, char* argv[], char* additional = nullptr);
		
		/** \brief preInitialize
		 */

		int32 preInitialize(const char* commandLineArgs);
		
		/** \brief Loads engine modules that are needed prior to engine initialization.
		 */

		void loadPreInitModules();
		
		/** \brief Loads core engine modules.
		 */

		void loadCoreModules();

		/** \brief loadStartupCoreModules
		 */
		
		bool loadStartupCoreModules();

		/** \brief loadStartupModules
		 */
		
		bool loadStartupModules();
		
		/** \brief initialize
		 */

		virtual int32 initialize() override;

		/** \brief Initializes engine timing system.
		 */
		
		void initializeTime();

		/** \brief exit
		 */
		
		void exit();
		
		//idle
		
		/** \brief update
		 */

		virtual void update() override;
		
		/** \brief clearPendingCleanupObjects
		 */

		virtual void clearPendingCleanupObjects() override;
	};
}

#endif