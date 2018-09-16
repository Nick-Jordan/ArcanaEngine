#ifndef OUTPUT_STREAM_H_
#define OUTPUT_STREAM_H_

#include "IODefines.h"

#include "Types.h"

namespace Arcana
{
	
	class OutputStream
	{
	public:
	
		virtual ~OutputStream() {}
						
		virtual void write(uint8* bytes, int64 length, int64 offset) = 0;
	};
	
}

#endif // !OUTPUT_STREAM_H_