#include "stdafx.h"
#include "ResourceHeader.h"

#include "XMLFile.h"

namespace Arcana
{
	ResourceHeader::ResourceHeader() : _lastPosition(0), _index(0)
	{

	}

	ResourceHeader::ResourceHeader(const std::string& filename) : _lastPosition(0), _index(0)
	{
		initialize(filename);
	}

	ResourceHeader::ResourceHeader(const ResourceHeader& other) : _type(other._type), _lastPosition(other._lastPosition), _dataPoints(other._dataPoints), _index(other._index)
	{

	}
		
	ResourceHeader::~ResourceHeader()
	{
	}
		
	ResourceHeader::DataPoint ResourceHeader::nextDataPoint()
	{
		DataPoint d = _dataPoints.at(_index);
		_index++;
		
		DataPoint returnData;
		returnData.name = d.name;
		returnData.type = d.type;
		returnData.position = _lastPosition;
		
		_lastPosition += d.position;
		
		return returnData;
	}
	
	bool ResourceHeader::hasDataPoints()
	{
		return _index < _dataPoints.size();
	}
	
	const std::string& ResourceHeader::getType() const
	{
		return _type;
	}

	ResourceHeader& ResourceHeader::operator=(const ResourceHeader& other)
	{
		_type = other._type;
		_lastPosition = other._lastPosition;
		_dataPoints = other._dataPoints;

		return *this;
	}

	void ResourceHeader::initialize(std::string filename)
	{
		XMLFile file(filename);

		LOGF(Info, CoreEngine, "Loading resource header from file, \'%s\'", filename.c_str());

		//for (auto iter = file.getNodes().createConstIterator(); iter; ++iter)
		for (auto iter = file.getNodes().begin(); iter != file.getNodes().end(); ++iter)
		{
			XMLNode n = (*iter);
			if (n.getName() == "resource_type")
			{
				_type = n.getValue();
			}
			else if (n.getName() == "value")
			{
				DataPoint dataPoint;

				//for (auto iter = n.getChildren().createConstIterator(); iter; ++iter)
				for (auto iter = n.getChildren().begin(); iter != n.getChildren().end(); ++iter)
				{
					XMLNode c = (*iter);
					if (c.getName() == "name")
					{
						dataPoint.name = c.getValue();
					}
					else if (c.getName() == "type")
					{
						Type type = Types::parseTypeFromString(c.getValue());

						dataPoint.position = type.size;
						dataPoint.type = type;
					}
				}

				_dataPoints.push_back(dataPoint);
			}
		}
	
		/*LOGF(Error, CoreEngine, "Type: %s", _type.c_str());

		for (int i = 0; i < _dataPoints.size(); i++)
		{
			LOGF(Error, CoreEngine, "Point: %s - %s, size: %d", _dataPoints[i].name.c_str(), _dataPoints[i].type.displayName.c_str(), _dataPoints[i].position);
		}*/
	
	}


	ResourceHeader::DataPoint::DataPoint() : name(""), position(0), type(Types::ErrorType)
	{

	}
	ResourceHeader::DataPoint::DataPoint(const DataPoint& other) : name(other.name), position(other.position), type(other.type)
	{

	}
	ResourceHeader::DataPoint::~DataPoint()
	{

	}

	ResourceHeader::DataPoint& ResourceHeader::DataPoint::operator=(const DataPoint& other)
	{
		name = other.name;
		position = other.position;
		type = other.type;

		return *this;
	}
}