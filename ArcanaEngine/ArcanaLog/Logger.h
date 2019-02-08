#ifndef LOGGER_H_
#define LOGGER_H_

#include "LogDefines.h"

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

			/** \brief The stream's previous position.
			 */
			unsigned int prev;

			/** \brief The stream's current position.
			 */
			unsigned int cur;

			/** \brief The previous message buffer.
			 */
			char prevBuf[BUFFER_SIZE];

			/** \brief The current message buffer.
			 */
			char curBuf[BUFFER_SIZE];

			/** \brief Checks if the current message equals the previous message.
			 */
			bool repetition();

			/** \brief Flushes the fstream.
			 */
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

		/** \brief Logger copy constructor
		 */
		Logger(const Logger& logger);

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
			
		//referencing
		void reference();

		void release();

		//memory leaks
		static int Instances;

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

		//references
		int _refcount = 0;
	};
}

#endif //!LOGGER_H_