#ifndef ARCANA_LOG_H_
#define ARCANA_LOG_H_

#ifdef ARCANA_LOG_EXPORTS
#define ARCANA_LOG_API __declspec(dllexport)
#else
#define ARCANA_LOG_API __declspec(dllimport)
#endif

#include "LogType.h"

#define REGISTER_CATEGORY(CategoryName, CategoryHexColor) \
	struct Log##CategoryName : public Arcana::LogCategory \
	{ \
		Log##CategoryName() : Arcana::LogCategory(#CategoryName, #CategoryHexColor) {} \
	}; \
	extern Log##CategoryName CategoryName;

#define INITIALIZE_CATEGORY(Namespace, CategoryName) \
	Log##CategoryName CategoryName = Log##CategoryName();

#define REGISTER_LOG_TYPE(TypeName, DefaultHexColor) \
	struct Log##TypeName : public Arcana::LogType \
	{ \
		Log##TypeName() : Arcana::LogType(#TypeName, #DefaultHexColor, new Arcana::Logger()) {} \
	}; \
	extern Log##TypeName TypeName;

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
	struct ARCANA_LOG_API LogCoreEngine : public Arcana::LogCategory
	{
		LogCoreEngine() : Arcana::LogCategory("CoreEngine", "none") {}
	};
	extern ARCANA_LOG_API LogCoreEngine CoreEngine;


	/** \brief Default Debug logger type.
	 */
	struct ARCANA_LOG_API LogDebug : public Arcana::LogType
	{
		LogDebug() : Arcana::LogType("Debug", "#888888", new Arcana::Logger()) {}
	};
	extern ARCANA_LOG_API LogDebug Debug;

	/** \brief Default Warning logger type.
	 */
	struct ARCANA_LOG_API LogWarning : public Arcana::LogType
	{
		LogWarning() : Arcana::LogType("Warning", "#CC9900", new Arcana::Logger()) {}
	};
	extern ARCANA_LOG_API LogWarning Warning;

	/** \brief Default Error logger type.
	 */
	struct ARCANA_LOG_API LogError : public Arcana::LogType
	{
		LogError() : Arcana::LogType("Error", "#CC0000", new Arcana::Logger()) {}
	};
	extern ARCANA_LOG_API LogError Error;

	/** \brief Default Info logger type.
	 */
	struct ARCANA_LOG_API LogInfo : public Arcana::LogType
	{
		LogInfo() : Arcana::LogType("Info", "#0000FF", new Arcana::Logger()) {}
	};
	extern ARCANA_LOG_API LogInfo Info;
}

#endif //!ARCANA_LOG_H_