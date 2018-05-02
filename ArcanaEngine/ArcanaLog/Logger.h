#ifndef LOGGER_H_
#define LOGGER_H_

#define ARCANA_LOG_EXPORTS

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
	class ARCANA_LOG_API Logger
	{
	public:
		
		class File
		{
		public:

			File(const std::string &name) : prev(0), cur(0)
			{
				/*stream.open(name.c_str(), std::ios::in | std::ios::out | std::ios::trunc | std::ios::binary);
				//stream.exceptions(fstream::eofbit | fstream::failbit | fstream::badbit);
				stream << "<html>\n";
				stream << "<head>\n";
				stream << "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\"/>\n";
				stream << "<title>Arcana Log</title>\n";
				stream << "<style type=\"text/css\">\n";
				stream << "body, html { background: #EEEEEE; color: #000000; font-family: sans-serif; }\n";
				stream << ".DATE { background: #DDDDDD; color: #333333; vertical-align: top; }\n";
				stream << "</style>\n";
				stream << "</head>\n";
				stream << "<body>\n";
				stream << "<table cellspacing=\"0\" cellpadding=\"2\">\n";
				stream.flush();*/
			}

		private:

			std::fstream stream;

			unsigned int prev;

			unsigned int cur;

			char prevBuf[BUFFER_SIZE];

			char curBuf[BUFFER_SIZE];

			bool repetition();

			void flush();

			friend class Logger;
		};

		Logger();
		Logger(File* file);
		~Logger();
		
		virtual void log(LogCategory category, const std::string& msg);

		void setName(const std::string& name);
		const std::string& getName();

		void setDefaultColor(const std::string& color);
		const std::string& getDefaultColor();
		
	private:
	
		File* _outFile;

		std::mutex _fileMutex;

		std::string _name;

		std::string _defaultColor;
	};
}

#endif //!LOGGER_H_