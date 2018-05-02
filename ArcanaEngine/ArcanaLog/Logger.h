#ifndef LOGGER_H_
#define LOGGER_H_

#ifdef ARCANA_LOG_EXPORTS
#define ARCANA_LOG_API __declspec(dllexport)
#else
#define ARCANA_LOG_API __declspec(dllimport)
#endif

#include <string>
#include <fstream>
#include <mutex>

#include "LogCategory.h"

#define BUFFER_SIZE 1024

namespace Arcana
{
	/** \brief Manages logging messages to the output files.
	 */
	class ARCANA_LOG_API Logger
	{
	public:
		
		/** \brief Container for an fstream that writes to an output file.
		 */
		class ARCANA_LOG_API File
		{
		public:

			/** \brief Opens a file with fstream open.
			 */
			File(const std::string &name);

		private:

			/** \brief The file's fstream.
			 */
			std::fstream stream;

			unsigned int prev;

			unsigned int cur;

			char prevBuf[BUFFER_SIZE];

			char curBuf[BUFFER_SIZE];

			bool repetition();

			void flush();

			friend class Logger;
		};

		/** \brief Default constructor.
		 *  Logs to the default output file.
		 */
		Logger();

		/** \brief Initializes the output file to a user-defined file.
		 */
		Logger(File* out);

		/** \brief Default destructor.
		 *  Closes the html tags in the output file. 
		 */
		~Logger();
		
		/** \brief Logs a message with a category to the output file.
		 *  This method is virtual to allow users to create custom Logger classes.
		 */
		virtual void log(LogCategory category, const std::string& msg);

		/** \brief Mutator for the logger name.
		 */
		void setName(const std::string& name);

		/** \brief Accessor for the logger name.
		 */
		const std::string& getName();

		/** \brief Mutator for the logger's default color.
		 */
		void setDefaultColor(const std::string& color);

		/** \brief Accessor for the logger's default color.
		 */
		const std::string& getDefaultColor();
		
	private:
	
		/** \brief The file to which the log will be written.
		 */
		File* _outFile;

		/** \brief Standard mutex for locking the writing method.
		 */
		std::mutex _fileMutex;

		/** \brief The logger's name.
		 */
		std::string _name;

		/** \brief The logger's default color.
		 *  The default color can be overridden by a category color.
		 */
		std::string _defaultColor;
	};
}

#endif //!LOGGER_H_