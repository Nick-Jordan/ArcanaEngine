#include "FileInputStream.h"

namespace Arcana
{

	FileInputStream::FileInputStream() : _file(nullptr)
	{
	}


	FileInputStream::~FileInputStream()
	{
		if (_file)
			std::fclose(_file);
	}

	bool FileInputStream::open(const std::string& filename)
	{
		if (_file)
		{
			std::fclose(_file);
		}
		_file = std::fopen(filename.c_str(), "rb");

		return _file != nullptr;
	}

	int64 FileInputStream::read(void* data, int64 size)
	{
		if (_file)
		{
			return std::fread(data, 1, static_cast<std::size_t>(size), _file);
		}

		return -1;
	}

	int64 FileInputStream::seek(int64 position)
	{
		if (_file)
		{
			if (std::fseek(_file, static_cast<long>(position), SEEK_SET))
			{
				return -1;
			}

			return tell();
		}

		return -1;
	}

	int64 FileInputStream::tell()
	{
		if (_file)
		{
			return std::ftell(_file);
		}

		return -1;
	}

	int64 FileInputStream::size()
	{
		if (_file)
		{
			int64 position = tell();
			std::fseek(_file, 0, SEEK_END);
			int64 size = tell();
			seek(position);

			return size;
		}

		return -1;
	}

}