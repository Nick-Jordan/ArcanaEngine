#ifndef FILE_INPUT_STREAM_H_
#define FILE_INPUT_STREAM_H_

#include "IODefines.h"

#include "InputStream.h"
#include <cstdio>
#include <string>

namespace Arcana
{

	class FileInputStream : public InputStream
	{
	public:

		FileInputStream();
		
		virtual ~FileInputStream();


		bool open(const std::string& filename);

		virtual int64 read(void* data, int64 size) override;

		virtual int64 seek(int64 position) override;

		virtual int64 tell() override;

		virtual int64 size() override;

	private:

		std::FILE* _file;
	};

}

#endif // !FILE_INPUT_STREAM_H_

