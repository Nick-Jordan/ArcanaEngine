#ifndef FILE_OUTPUT_STREAM_H_
#define FILE_OUTPUT_STREAM_H_

#include "IODefines.h"

#include "OutputStream.h"
#include <cstdio>
#include <string>

namespace Arcana
{
	
	class ARCANA_IO_API FileOutputStream : public OutputStream
	{
	public:
	
		FileOutputStream();
		
		virtual ~FileOutputStream();
		
		
		bool open(const std::string& filename, bool append = false);
						
		virtual void write(uint8* byte, int64 length, int64 offset) override;
		
	private:
	
		std::FILE* _file;
	};
	
}

#endif // !FILE_OUTPUT_STREAM_H_