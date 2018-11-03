#ifndef LOG_TYPE_H_
#define LOG_TYPE_H_

#ifdef ARCANA_LOG_EXPORTS
#define ARCANA_LOG_API __declspec(dllexport)
#else
#define ARCANA_LOG_API __declspec(dllimport)
#endif

#include "Logger.h"

namespace Arcana
{
	/** \brief Defines a log type.
	 *  Container for a Logger object, name, and default color.
	 */
	class ARCANA_LOG_API LogType
	{
	public:

		/** \brief Default constructor.
		 *  Initializes a nullptr logger. 
		 */
		LogType();
		
		/** \brief Initializes a logger object with specified name and default color.
		 */
		LogType(std::string name, std::string defaultColor, Logger* logger);
		
		LogType(const LogType& type);
		
		/** \brief Default destructor.
		 */
		~LogType();
		
		/** \brief Accessor for the log type's name.
		 */
		const std::string& getName();
		
		/** \brief Accessor for the log type's default color.
		 */
		const std::string& getDefaultColor();
		
		/** \brief Accessor for the pointer to the Logger.
		 */
		Logger* getLogger();
	
	private:
	
		/** \brief The log type's name.
		 */
		std::string _name;

		/** \brief The log type's default color.
		 */
		std::string _defaultColor;

		/** \brief Pointer to the type's Logger object.
		 */
		Logger* _logger;
	};
}

#endif //!LOG_TYPE_H_