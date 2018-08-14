#include "MemoryInputStream.h"

#include <memory>

namespace Arcana
{

	MemoryInputStream::MemoryInputStream() :
		_data(nullptr),
		_size(0),
		_offset(0)
	{
	}


	MemoryInputStream::~MemoryInputStream()
	{
	}

	void MemoryInputStream::open(const void* data, std::size_t size)
	{
		_data = static_cast<const char*>(data);
		_size = size;
		_offset = 0;
	}

	int64 MemoryInputStream::read(void* data, int64 size)
	{
		if (!_data)
		{
			return -1;
		}

		int64 endPosition = _offset + size;
		int64 count = endPosition <= _size ? size : _size - _offset;

		if (count > 0)
		{
			std::memcpy(data, _data + _offset, static_cast<std::size_t>(count));
			_offset += count;
		}

		return count;

	}

	int64 MemoryInputStream::seek(int64 position)
	{
		if (!_data)
		{
			return -1;
		}

		_offset = position < _size ? position : _size;
		return _offset;
	}

	int64 MemoryInputStream::tell()
	{
		if (!_data)
		{
			return -1;
		}

		return _offset;
	}

	int64 MemoryInputStream::size()
	{
		if (!_data)
		{
			return -1;
		}

		return _size;
	}

}
