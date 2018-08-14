#ifndef INPUT_STREAM_H_
#define INPUT_STREAM_H_

#include "IODefines.h"

#include "Types.h"

namespace Arcana
{
	class ARCANA_IO_API InputStream
	{
	public:

		virtual ~InputStream() {}

		virtual int64 read(void* data, int64 size) = 0;

		virtual int64 seek(int64 position) = 0;

		virtual int64 tell() = 0;

		virtual int64 size() = 0;
	};
}

#endif // !INPUT_STREAM_H_
