#ifndef ARCANA_LOG_H_
#define ARCANA_LOG_H_

#ifdef ARCANA_LOG_EXPORTS
#define ARCANA_LOG_API __declspec(dllexport)
#else
#define ARCANA_LOG_API __declspec(dllimport)
#endif

#include "LogType.h"

#define REGISTER_CATEGORY(CategoryName, CategoryHexColor) \
	ARCANA_LOG_API struct Log##CategoryName : public Arcana::LogCategory \
	{ \
		Log##CategoryName() : Arcana::LogCategory(#CategoryName, #CategoryHexColor) {} \
	}; \
	extern ARCANA_LOG_API Log##CategoryName CategoryName;

#define INITIALIZE_CATEGORY(Namespace, CategoryName) \
	Log##CategoryName CategoryName = Log##CategoryName();

#define REGISTER_LOG_TYPE(TypeName, DefaultHexColor) \
	struct Log##TypeName : public Arcana::LogType \
	{ \
		Log##TypeName() : Arcana::LogType(#TypeName, #DefaultHexColor, new Arcana::Logger()) {} \
	}; \
	extern ARCANA_LOG_API Log##TypeName TypeName;

#define INITIALIZE_LOG_TYPE(Namespace, TypeName) \
	Log##TypeName Namespace::TypeName = Log##TypeName();

namespace Arcana
{

	/** \brief Container for the default output file.
	 */

	class LogOutput
	{
	public:	

		/** \brief The default output file.
		 */
		static Logger::File* File;
	};

	
	/** \brief Logs a simple message with a user-defined Logger.
	 */
	extern ARCANA_LOG_API void LOG(Logger* customLogger, LogCategory category, const std::string& msg);
	
	/** \brief Logs a simple message with a pre-defined Logger.
	*/
	extern ARCANA_LOG_API void LOG(LogType type, LogCategory category, const std::string& msg);
	
	/** \brief Logs a message that has arguments (like printf) with a user-defined Logger.
	 */
	extern ARCANA_LOG_API void LOGF(Logger* customLogger, LogCategory category, const char* msg, ...);
	
	/** \brief Logs a message that has arguments (like printf) with a pre-defined Logger.
	 */
	extern ARCANA_LOG_API void LOGF(LogType type, LogCategory category, const char* msg, ...);


	/** \brief Default Engine log category.
	 */
	REGISTER_CATEGORY(CoreEngine, none)

	/** \brief Default Debug logger type.
	 */
	REGISTER_LOG_TYPE(Debug, #888888)
	/** \brief Default Warning logger type.
	 */
	REGISTER_LOG_TYPE(Warning, #CC9900)
	/** \brief Default Error logger type.
	 */
	REGISTER_LOG_TYPE(Error, #CC0000)
	/** \brief Default Info logger type.
	 */
	REGISTER_LOG_TYPE(Info, #0000FF)
}

#endif //!ARCANA_LOG_H_