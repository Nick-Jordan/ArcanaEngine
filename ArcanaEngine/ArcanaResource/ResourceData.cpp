#include "stdafx.h"
#include "ResourceData.h"

#include "ArcanaLog.h"
#include "Resource.h"

namespace Arcana
{
	ResourceData::ResourceData() : _stringAddition(0)
	{

	}

	ResourceData::ResourceData(const Archive& archive, ResourceHeader& header) : _archive(archive), _header(header), _stringAddition(0)
	{
		initialize(header);
	}

	ResourceData::ResourceData(const ResourceData& other) : _archive(other._archive), _header(other._header), _positions(other._positions), _stringAddition(other._stringAddition)
	{

	}

	ResourceData::~ResourceData()
	{
	}
		
		
	bool ResourceData::getBoolParameter(const std::string& name)
	{
		int64 pos = findPosition(name);
		
		if(pos != -1)
		{
			_archive.seek(pos);
			return _archive.readBool();
		}
		
		LOGF(Error, ResourceLog, "Unable to find parameter with name, \'%s\'", name.c_str());
		return false;
	}
		
	float ResourceData::getFloatParameter(const std::string& name)
	{
		int64 pos = findPosition(name);
		
		if(pos != -1)
		{
			_archive.seek(pos);
			return _archive.readFloat();
		}
		
		LOGF(Error, ResourceLog, "Unable to find parameter with name, \'%s\'", name.c_str());
		return 0.0f;
	}
		
	double ResourceData::getDoubleParameter(const std::string& name)
	{
		int64 pos = findPosition(name);
		
		if(pos != -1)
		{
			_archive.seek(pos);
			return _archive.readDouble();
		}
		
		LOGF(Error, ResourceLog, "Unable to find parameter with name, \'%s\'", name.c_str());
		return 0.0;
	}
		
	char ResourceData::getCharParameter(const std::string& name)
	{
		int64 pos = findPosition(name);
		
		if(pos != -1)
		{
			_archive.seek(pos);
			return _archive.readChar();
		}
		
		LOGF(Error, ResourceLog, "Unable to find parameter with name, \'%s\'", name.c_str());
		return 0;
	}
		
	int8 ResourceData::getInt8Parameter(const std::string& name)
	{
		int64 pos = findPosition(name);
		
		if(pos != -1)
		{
			_archive.seek(pos);
			return _archive.readInt8();
		}
		
		LOGF(Error, ResourceLog, "Unable to find parameter with name, \'%s\'", name.c_str());
		return 0;
	}
		
	int16 ResourceData::getInt16Parameter(const std::string& name)
	{
		int64 pos = findPosition(name);
		
		if(pos != -1)
		{
			_archive.seek(pos);
			return _archive.readInt16();
		}
		
		LOGF(Error, ResourceLog, "Unable to find parameter with name, \'%s\'", name.c_str());
		return 0;
	}
		
	int32 ResourceData::getInt32Parameter(const std::string& name)
	{
		int64 pos = findPosition(name);
		
		if(pos != -1)
		{
			_archive.seek(pos);
			return _archive.readInt32();
		}
		
		LOGF(Error, ResourceLog, "Unable to find parameter with name, \'%s\'", name.c_str());
		return 0;
	}
		
	int64 ResourceData::getInt64Parameter(const std::string& name)
	{
		int64 pos = findPosition(name);
		
		if(pos != -1)
		{
			_archive.seek(pos);
			return _archive.readInt64();
		}
		
		LOGF(Error, ResourceLog, "Unable to find parameter with name, \'%s\'", name.c_str());
		return 0;
	}
		
	uint8 ResourceData::getUint8Parameter(const std::string& name)
	{
		int64 pos = findPosition(name);
		
		if(pos != -1)
		{
			_archive.seek(pos);
			return _archive.readUint8();
		}
		
		LOGF(Error, ResourceLog, "Unable to find parameter with name, \'%s\'", name.c_str());
		return 0;
	}
		
	uint16 ResourceData::getUint16Parameter(const std::string& name)
	{
		int64 pos = findPosition(name);
		
		if(pos != -1)
		{
			_archive.seek(pos);
			return _archive.readUint16();
		}
		
		LOGF(Error, ResourceLog, "Unable to find parameter with name, \'%s\'", name.c_str());
		return 0;
	}
		
	uint32 ResourceData::getUint32Parameter(const std::string& name)
	{
		int64 pos = findPosition(name);
		
		if(pos != -1)
		{
			_archive.seek(pos);
			return _archive.readUint32();
		}
		
		LOGF(Error, ResourceLog, "Unable to find parameter with name, \'%s\'", name.c_str());
		return 0;
	}
		
	uint64 ResourceData::getUint64Parameter(const std::string& name)
	{
		int64 pos = findPosition(name);
		
		if(pos != -1)
		{
			_archive.seek(pos);
			return _archive.readUint64();
		}
		
		LOGF(Error, ResourceLog, "Unable to find parameter with name, \'%s\'", name.c_str());
		return 0;
	}
		
	std::string ResourceData::getStringParameter(const std::string& name)
	{
		int64 pos = findPosition(name);
		
		if(pos != -1)
		{
			_archive.seek(pos);
			std::string string = _archive.readString();
			updatePositionForString(string.size());
			return string;
		}
		
		LOGF(Error, ResourceLog, "Unable to find parameter with name, \'%s\'", name.c_str());
		return "";
	}
	
	const Archive& ResourceData::getArchive()
	{
		return _archive;
	}
	
	const ResourceHeader& ResourceData::getHeader() const
	{
		return _header;
	}
	
	void ResourceData::initialize(ResourceHeader& header)
	{
		while(header.hasDataPoints())
		{
			ResourceHeader::DataPoint data = header.nextDataPoint();
			_positions.emplace(data.name, data.position);
		}
	}
	
	int64 ResourceData::findPosition(const std::string& name)
	{
		std::map<std::string, int64>::iterator i = _positions.find(name);
		if (i != _positions.end())
		{
			return i->second + _stringAddition;
		}
		
		return -1;
	}

	void ResourceData::updatePositionForString(int64 stringSize)
	{
		_stringAddition += stringSize;
	}

	ResourceData& ResourceData::operator=(const ResourceData& other)
	{
		_archive = other._archive;
		_header = other._header;
		_positions = other._positions;
		_stringAddition = other._stringAddition;

		return *this;
	}
}