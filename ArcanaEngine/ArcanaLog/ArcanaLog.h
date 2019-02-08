#ifndef ARCANA_LOG_H_
#define ARCANA_LOG_H_

#ifdef ARCANA_LOG_EXPORTS
#define ARCANA_LOG_API __declspec(dllexport)
#else
#define ARCANA_LOG_API __declspec(dllimport)
#endif

//vld
#include <vld.h>

#include "LogType.h"

/** \def REGISTER_CATEGORY(CategoryName, CategoryHexColor)
 *
 *  Registers a log category.
 *  Categories need a name and can have a color.
 *  Category colors override logger colors.
 *  If your category doesn't need a color, set it to 'none.'
 */
#define REGISTER_CATEGORY(CategoryName, CategoryHexColor) \
	struct Log##CategoryName : public Arcana::LogCategory \
	{ \
		Log##CategoryName() : Arcana::LogCategory(#CategoryName, #CategoryHexColor) {} \
	}; \
	extern Log##CategoryName CategoryName;

/** \def REGISTER_CATEGORY_ENGINE(API, CategoryName, CategoryHexColor)
 *
 *  Registers an engine-specific log category.
 *  Exports the category with __declspec.
 */
#define REGISTER_CATEGORY_ENGINE(API, CategoryName, CategoryHexColor) \
	struct API Log##CategoryName : public Arcana::LogCategory \
	{ \
		Log##CategoryName() : Arcana::LogCategory(#CategoryName, #CategoryHexColor) {} \
	}; \
	extern API Log##CategoryName CategoryName;

/** \def INITIALIZE_CATEGORY(Namespace, CategoryName)
 *
 *  Initializes a log category.
 *  Meant to be called in a cpp file.
 */
#define INITIALIZE_CATEGORY(Namespace, CategoryName) \
	Log##CategoryName CategoryName = Log##CategoryName();

/** \def REGISTER_LOG_TYPE(TypeName, DefaultHexColor)
 *
 *  Registers a log type.
 *  Log types need a name and a default color.
 */
#define REGISTER_LOG_TYPE(TypeName, DefaultHexColor) \
	struct Log##TypeName : public Arcana::LogType \
	{ \
		Log##TypeName() : Arcana::LogType(#TypeName, #DefaultHexColor, new Arcana::Logger()) {} \
	}; \
	extern Log##TypeName TypeName;

/** \def REGISTER_LOG_TYPE_ENGINE(API, TypeName, DefaultHexColor)
 *
 *  Registers an engine-specific log type.
 *  Exports the category with __declspec.
 */
#define REGISTER_LOG_TYPE_ENGINE(API, TypeName, DefaultHexColor) \
	struct API Log##TypeName : public Arcana::LogType \
	{ \
		Log##TypeName() : Arcana::LogType(#TypeName, #DefaultHexColor, new Arcana::Logger()) {} \
	}; \
	extern API Log##TypeName TypeName;

/** \def INITIALIZE_LOG_TYPE(Namespace, TypeName)
 *
 *  Initializes a log type.
 *  Meant to be called in a cpp file.
 */
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
		static Logger::File File;
	};

	
	/** \brief Logs a simple message with a user-defined Logger.
	 */
	extern ARCANA_LOG_API void LOG(Logger* customLogger, LogCategory category, const std::string& msg);
	
	/** \brief Logs a simple message with a pre-defined Logger.
	 */
	extern ARCANA_LOG_API void LOG(LogType& type, LogCategory category, const std::string& msg);
	
	/** \brief Logs a message that has format specifiers (like printf) with a user-defined Logger.
	 */
	extern ARCANA_LOG_API void LOGF(Logger* customLogger, LogCategory category, const char* msg, ...);
	
	/** \brief Logs a message that has format specifiers (like printf) with a pre-defined Logger.
	 */
	extern ARCANA_LOG_API void LOGF(LogType& type, LogCategory category, const char* msg, ...);


	/** \brief Default Engine log category.
	 */
	REGISTER_CATEGORY_ENGINE(ARCANA_LOG_API, CoreEngine, none)


	/** \brief Default Debug logger type.
	 */
	REGISTER_LOG_TYPE_ENGINE(ARCANA_LOG_API, Debug, #888888)

	/** \brief Default Warning logger type.
	 */
	REGISTER_LOG_TYPE_ENGINE(ARCANA_LOG_API, Warning, #CC9900)

	/** \brief Default Error logger type.
	 */
	REGISTER_LOG_TYPE_ENGINE(ARCANA_LOG_API, Error, #CC0000)

	/** \brief Default Info logger type.
	 */
	REGISTER_LOG_TYPE_ENGINE(ARCANA_LOG_API, Info, #0000FF)
}

#endif //!ARCANA_LOG_H_