#include "Logger.h"
#include "ArcanaLog.h"

#include <algorithm>
#include <iostream>

namespace Arcana
{
	Logger::File::File(const std::string &name) : prev(0), cur(0)
	{
		stream.open(name.c_str(), std::ios::in | std::ios::out | std::ios::trunc | std::ios::binary);
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
		stream.flush();
	}

	bool Logger::File::repetition()
	{
		unsigned int n = (std::min)((unsigned int) BUFFER_SIZE, (unsigned int) (cur - prev));
		stream.seekg(prev, std::ios::beg);
		stream.read(prevBuf, n);
		stream.seekg(cur, std::ios::beg);
		stream.read(curBuf, n);
		stream.tellg();
		return strncmp(prevBuf, curBuf, n) == 0;
	}

	void Logger::File::flush()
	{
		unsigned int pos = (unsigned int) stream.tellp();
		if (prev == 0) {
			prev = pos;
		} else if (cur == 0) {
			cur = pos;
		} else {
			unsigned int prevSize = cur - prev;
			unsigned int curSize = pos - cur;
			if (curSize > 0 && curSize == prevSize && repetition()) {
				stream.seekp(cur, std::ios::beg);
			} else {
				prev = cur;
				cur = pos;
			}
		}
	}

	Logger::Logger() : _outFile(LogOutput::File)
	{
		Instances++;
	}

	Logger::Logger(File* out) : _outFile(out)
	{
		Instances++;
	}

	Logger::~Logger()
	{
		if (_outFile)
		{
			std::fstream &o = _outFile->stream;
			if (o.is_open()) {
				o << "</table>\n";
				o << "</body>\n";
				o << "</html>\n";
				o.close();
			}
		}

		Instances--;
		if (Instances == 0)
		{
			delete LogOutput::File;
		}
	}
		
	void Logger::log(LogCategory category, const std::string& msg)
	{
		time_t rawtime;
		char timestring[256];

		time(&rawtime);
#ifdef _MSC_VER
		struct tm timeinfo;
		localtime_s(&timeinfo, &rawtime);
		strftime(timestring, 256, "%H:%M:%S", &timeinfo);
#else
		struct tm *timeinfo = localtime(&rawtime);
		strftime(timestring, 256, "%H:%M:%S", timeinfo);
#endif

		_fileMutex.lock();
		std::fstream &o = _outFile->stream;
		o << "<tr><td class=\"DATE\">" << timestring << "</td>\n";
		std::string colorHexCode = (category.getHexColor().compare("none") == 0) ? _defaultColor : category.getHexColor();
		o << "<td style=\"color:" << colorHexCode << ";\">" << "[" << category.getName() << "] ";
		bool pre = false;
		bool bold = false;
		for (std::string::const_iterator it = msg.begin(); it < msg.end(); it++) {
			char c = *it;
			if (c == '\033') {
				o << (pre ? "</pre>" : "<pre>");
				pre = !pre;
			}
			else if (c == '\'') {
				o << (bold ? "</b>" : "<b>");
				bold = !bold;
			}
			else if (c == '<') {
				o << "&lt;";
			}
			else if (c == '>') {
				o << "&gt;";
			}
			else {
				o << c;
			}
		}
		o << "</td></tr>\n";
		o.flush();
		_fileMutex.unlock();

#ifdef _DEBUG
		std::cout << timestring << " [" << category.getName() << "] " << msg << std::endl;
#endif
	}

	void Logger::setName(const std::string& name)
	{
		_name = name;
	}

	const std::string& Logger::getName()
	{
		return _name;
	}

	void Logger::setDefaultColor(const std::string& color)
	{
		_defaultColor = color;
	}

	const std::string& Logger::getDefaultColor()
	{
		return _defaultColor;
	}




	void Logger::reference()
	{
		_refcount++;
	}

	void Logger::release()
	{
		_refcount--;
		if (!_refcount)
		{
			delete this;
		}
	}

	//memory leaks

	int Logger::Instances = 0;
}