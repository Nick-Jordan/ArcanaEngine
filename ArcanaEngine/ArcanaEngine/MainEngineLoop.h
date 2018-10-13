#ifndef MAIN_ENGINE_LOOP_H_
#define MAIN_ENGINE_LOOP_H_

#include "EngineDefines.h"

#include "EngineLoop.h"
#include "Array.h"
#include "ModuleInterface.h"

namespace Arcana
{

	/** \brief The main engine loop.
	 *
	 *  (EXTENDED DESCRIPTION)
	 */

	class ARCANA_ENGINE_API MainEngineLoop : public EngineLoop
	{
	public:

		/** \brief MainEngineLoop default constructor.
		 */
		
		MainEngineLoop();

		virtual ~MainEngineLoop();
		
		/** \brief preInitialize
		 */

		int32 preInitialize(int32 argc, char* argv[], char* additional = nullptr);
		
		/** \brief preInitialize
		 */

		int32 preInitialize(const char* commandLineArgs);

		void preInitModules();

		int32 startupModules();

		int32 shutdownModules();
		
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
		void update2()
		{

		}
		
		/** \brief clearPendingCleanupObjects
		 */

		virtual void clearPendingCleanupObjects() override;

	private:

		Array<ModuleInterface*> _initQueue;
	};
}

#endif