#include "Archive.h"

namespace Arcana
{
	Archive::Archive() : _readPosition(0)
	{
	}
	
	Archive::Archive(const std::vector<uint8>& bytes, int64 readPosition) : _bytes(bytes), _readPosition(readPosition)
	{
	}
	
	Archive::Archive(const uint8* bytes, int64 size, int64 readPosition) : _readPosition(readPosition)
	{
		_bytes = std::vector<uint8>(bytes, bytes + size);
	}
	
	Archive::Archive(InputStream& stream) : _readPosition(0)
	{
		_bytes.resize(stream.size());
		stream.read(&_bytes[0], stream.size());
	}
		
	Archive::Archive(const Archive& other)
	{
		_bytes = other._bytes;
		_readPosition = other._readPosition;
	}
		
	Archive::~Archive()
	{
	}
		

	void Archive::writeBool(bool b)
	{
		writePrimitive<bool>(b);
	}
		
	void Archive::writeFloat(float f)
	{
		writePrimitive<float>(f);
	}
		
	void Archive::writeDouble(double d)
	{
		writePrimitive<double>(d);
	}
		
	void Archive::writeChar(char c)
	{
		writePrimitive<char>(c);
	}
		
	void Archive::writeInt8(int8 i)
	{
		writePrimitive<int8>(i);
	}
		
	void Archive::writeInt16(int16 i)
	{
		writePrimitive<int16>(i);
	}
		
	void Archive::writeInt32(int32 i)
	{
		writePrimitive<int32>(i);
	}
		
	void Archive::writeInt64(int64 i)
	{
		writePrimitive<int64>(i);
	}
		
	void Archive::writeUint8(uint8 i)
	{
		writePrimitive<uint8>(i);
	}
		
	void Archive::writeUint16(uint16 i)
	{
		writePrimitive<uint16>(i);
	}
		
	void Archive::writeUint32(uint32 i)
	{
		writePrimitive<uint32>(i);
	}
		
	void Archive::writeUint64(uint64 i)
	{
		writePrimitive<uint64>(i);
	}
		
	void Archive::writeString(const std::string& string)
	{
		writeInt64(string.size());
		writeData(reinterpret_cast<const uint8*>(string.data()), string.size());
	}
		
	void Archive::writeData(const uint8* bytes, int64 size)
	{
		_bytes.insert(_bytes.end(), bytes, bytes + size);
	}
		
	void Archive::writeData(const std::vector<uint8>& bytes)
	{
		_bytes.insert(_bytes.end(), bytes.begin(), bytes.end());
	}
		
		
	bool Archive::readBool()
	{
		return readPrimitive<bool>();
	}
		
	float Archive::readFloat()
	{
		return readPrimitive<float>();
	}
		
	double Archive::readDouble()
	{
		return readPrimitive<double>();
	}
		
	char Archive::readChar()
	{
		return readPrimitive<char>();
	}
		
	int8 Archive::readInt8()
	{
		return readPrimitive<int8>();
	}
		
	int16 Archive::readInt16()
	{
		return readPrimitive<int16>();
	}
		
	int32 Archive::readInt32()
	{
		return readPrimitive<int32>();
	}
		
	int64 Archive::readInt64()
	{
		return readPrimitive<int64>();
	}
		
	uint8 Archive::readUint8()
	{
		return readPrimitive<uint8>();
	}
		
	uint16 Archive::readUint16()
	{
		return readPrimitive<uint16>();
	}
		
	uint32 Archive::readUint32()
	{
		return readPrimitive<uint32>();
	}
		
	uint64 Archive::readUint64()
	{
		return readPrimitive<uint64>();
	}
		
	std::string Archive::readString()
	{
		int64 size = readInt64();
		const std::vector<uint8> data = readData(size);
		
		return std::string(data.begin(), data.end());
	}
	
	const std::vector<uint8> Archive::readData(int64 size)
	{
		std::vector<uint8> data;
		
		if(_readPosition + size <= _bytes.size())
		{
			data.assign(_bytes.begin() + _readPosition, _bytes.begin() + _readPosition + size);
			_readPosition += size;
		}
		
		return data;
	}
		
	
	void Archive::seek(int64 position)
	{
		_readPosition = position;
	}

	int64 Archive::tell()
	{
		return _readPosition;
	}
		
	int64 Archive::size()
	{
		return _bytes.size();
	}
	
	void Archive::writeToOutputStream(OutputStream& stream)
	{
		stream.write(&_bytes[0], size(), 0);
	}
	
	void Archive::clear()
	{
		_bytes.clear();
		seek(0);
	}
		
	Archive& Archive::operator=(const Archive& other)
	{
		_bytes = other._bytes;
		_readPosition = other._readPosition;
		return *this;
	}
}