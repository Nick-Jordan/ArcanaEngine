#ifndef ARCHIVE_H_
#define ARCHIVE_H_

#include "IODefines.h"

#include "Types.h"
#include "InputStream.h"
#include "OutputStream.h"
#include <vector>

namespace Arcana
{
	
	class ARCANA_IO_API Archive
	{
	public:
		
		Archive();
		
		Archive(const std::vector<uint8>& bytes, int64 readPosition = 0);
		
		Archive(const uint8* bytes, int64 size, int64 readPosition = 0);
		
		Archive(InputStream& stream);
		
		Archive(const Archive& other);
		
		~Archive();
		
		
		void writeBool(bool b);
		
		void writeFloat(float f);
		
		void writeDouble(double d);
		
		void writeChar(char c);
		
		void writeInt8(int8 i);
		
		void writeInt16(int16 i);
		
		void writeInt32(int32 i);
		
		void writeInt64(int64 i);
		
		void writeUint8(uint8 i);
		
		void writeUint16(uint16 i);
		
		void writeUint32(uint32 i);
		
		void writeUint64(uint64 i);
		
		void writeString(const std::string& string);
		
		void writeData(const uint8* bytes, int64 size);
		
		void writeData(const std::vector<uint8>& bytes);
		
		
		bool readBool();
		
		float readFloat();
		
		double readDouble();
		
		char readChar();
		
		int8 readInt8();
		
		int16 readInt16();
		
		int32 readInt32();
		
		int64 readInt64();
		
		uint8 readUint8();
		
		uint16 readUint16();
		
		uint32 readUint32();
		
		uint64 readUint64();
		
		std::string readString();
		
		const std::vector<uint8> readData(int64 size);
		
		
		void seek(int64 position);

		int64 tell();
		
		int64 size();
		
		void writeToOutputStream(OutputStream& stream);
		
		void clear();
		
		
		Archive& operator=(const Archive& other);
		
	private:
	
		template<typename T>
		void writePrimitive(T primitive);
		
		template<typename T>
		T readPrimitive();
		
	
		int64 _readPosition;
		std::vector<uint8> _bytes;
	};
	
	
	template<typename T>
	inline void Archive::writePrimitive(T primitive)
	{
		T* ptr = &primitive;
		uint8* bytes = reinterpret_cast<uint8*>(ptr);
	
		for(size_t i = 0; i < sizeof(T); i++)
		{
			uint8 b = *bytes;
			_bytes.push_back(b);
			bytes++;
		}
	};
	
	template<typename T>
	inline T Archive::readPrimitive()
	{
		int64 size = sizeof(T);
		
		if(_readPosition + size <= _bytes.size())
		{
			const uint8* bytes = &_bytes[_readPosition];
			
			T result;
			memcpy(&result, bytes, size);
			_readPosition += size;
			return result;
		}
		
		return (T)0;
	};
	
}

#endif // !ARCHIVE_H_