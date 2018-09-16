#include "FileOutputStream.h"

namespace Arcana
{
	FileOutputStream::FileOutputStream() : _file(nullptr)
	{
	}
	
	FileOutputStream::~FileOutputStream()
	{
		if (_file)
			std::fclose(_file);
	}
	
	bool FileOutputStream::open(const std::string& filename, bool append)
	{
		if (_file)
		{
			std::fclose(_file);
		}
		
		const char* mode = append ? "ab" : "wb";
		
		_file = std::fopen(filename.c_str(), mode);

		return _file != nullptr;
	}
	
	void FileOutputStream::write(uint8* bytes, int64 length, int64 offset)
	{
		
		std::fwrite(bytes, 1, length, _file);
	}
}