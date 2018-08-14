#ifndef MEMORY_INPUT_STREAM_H_
#define MEMORY_INPUT_STREAM_H_

#include "IODefines.h"

#include "InputStream.h"
#include <cstdlib>

namespace Arcana
{

	class MemoryInputStream : public InputStream
	{
	public:

		MemoryInputStream();

		virtual ~MemoryInputStream();

		void open(const void* data, std::size_t size);

		virtual int64 read(void* data, int64 size) override;

		virtual int64 seek(int64 position) override;

		virtual int64 tell() override;

		virtual int64 size() override;

	private:

		const char* _data;
		int64 _size;
		int64 _offset;
	};

}

#endif // !MEMORY_INPUT_STREAM_H_

