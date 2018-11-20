#include "ResourceData.h"
#include "XMLFile.h"

#include "ArcanaLog.h"
#include "Resource.h"

#include <iostream>

namespace Arcana
{
	ResourceData::ResourceData() : _dataPoints()
	{

	}

	ResourceData::ResourceData(const XMLNode& node)
	{
		initialize(node);
	}

	ResourceData::ResourceData(const ResourceData& other) : _dataPoints(other._dataPoints)
	{

	}

	ResourceData::~ResourceData()
	{
	}
		
	
	const std::vector<ResourceDataPoint>& ResourceData::getDataPoints() const
	{
		return _dataPoints;
	}

	bool ResourceData::getBoolParameter(const std::string& name) const
	{
		const ResourceDataPoint* dataPoint = findDataPoint(name);

		if (dataPoint)
		{
			return (*dataPoint).boolData;
		}
		
		LOGF(Error, ResourceLog, "Unable to find bool parameter with name, \'%s\'", name.c_str());
		return false;
	}
		
	float ResourceData::getFloatParameter(const std::string& name) const
	{
		const ResourceDataPoint* dataPoint = findDataPoint(name);

		if (dataPoint)
		{
			return (*dataPoint).floatData;
		}
		
		LOGF(Error, ResourceLog, "Unable to find float parameter with name, \'%s\'", name.c_str());
		return 0.0f;
	}
		
	double ResourceData::getDoubleParameter(const std::string& name) const
	{
		const ResourceDataPoint* dataPoint = findDataPoint(name);

		if (dataPoint)
		{
			return (*dataPoint).doubleData;
		}
		
		LOGF(Error, ResourceLog, "Unable to find double parameter with name, \'%s\'", name.c_str());
		return 0.0;
	}
		
	char ResourceData::getCharParameter(const std::string& name) const
	{
		const ResourceDataPoint* dataPoint = findDataPoint(name);

		if (dataPoint)
		{
			return (*dataPoint).int8Data;
		}
		
		LOGF(Error, ResourceLog, "Unable to find char parameter with name, \'%s\'", name.c_str());
		return 0;
	}
		
	int8 ResourceData::getInt8Parameter(const std::string& name) const
	{
		const ResourceDataPoint* dataPoint = findDataPoint(name);

		if (dataPoint)
		{
			return (*dataPoint).int8Data;
		}
		
		LOGF(Error, ResourceLog, "Unable to find int8 parameter with name, \'%s\'", name.c_str());
		return 0;
	}
		
	int16 ResourceData::getInt16Parameter(const std::string& name) const
	{
		const ResourceDataPoint* dataPoint = findDataPoint(name);

		if (dataPoint)
		{
			return (*dataPoint).int16Data;
		}
		
		LOGF(Error, ResourceLog, "Unable to find int16 parameter with name, \'%s\'", name.c_str());
		return 0;
	}
		
	int32 ResourceData::getInt32Parameter(const std::string& name) const
	{
		const ResourceDataPoint* dataPoint = findDataPoint(name);

		if (dataPoint)
		{
			return (*dataPoint).int32Data;
		}
		
		LOGF(Error, ResourceLog, "Unable to find int32 parameter with name, \'%s\'", name.c_str());
		return 0;
	}
		
	int64 ResourceData::getInt64Parameter(const std::string& name) const
	{
		const ResourceDataPoint* dataPoint = findDataPoint(name);

		if (dataPoint)
		{
			return (*dataPoint).int64Data;
		}
		
		LOGF(Error, ResourceLog, "Unable to find int64 parameter with name, \'%s\'", name.c_str());
		return 0;
	}
		
	uint8 ResourceData::getUint8Parameter(const std::string& name) const
	{
		const ResourceDataPoint* dataPoint = findDataPoint(name);

		if (dataPoint)
		{
			return (*dataPoint).uint8Data;
		}
		
		LOGF(Error, ResourceLog, "Unable to find uint8 parameter with name, \'%s\'", name.c_str());
		return 0;
	}
		
	uint16 ResourceData::getUint16Parameter(const std::string& name) const
	{
		const ResourceDataPoint* dataPoint = findDataPoint(name);

		if (dataPoint)
		{
			return (*dataPoint).uint16Data;
		}
		
		LOGF(Error, ResourceLog, "Unable to find uint16 parameter with name, \'%s\'", name.c_str());
		return 0;
	}
		
	uint32 ResourceData::getUint32Parameter(const std::string& name) const
	{
		const ResourceDataPoint* dataPoint = findDataPoint(name);

		if (dataPoint)
		{
			return (*dataPoint).uint32Data;
		}
		
		LOGF(Error, ResourceLog, "Unable to find uint32 parameter with name, \'%s\'", name.c_str());
		return 0;
	}
		
	uint64 ResourceData::getUint64Parameter(const std::string& name) const
	{
		const ResourceDataPoint* dataPoint = findDataPoint(name);

		if (dataPoint)
		{
			return (*dataPoint).uint64Data;
		}
		
		LOGF(Error, ResourceLog, "Unable to find uint64 parameter with name, \'%s\'", name.c_str());
		return 0;
	}
		
	std::string ResourceData::getStringParameter(const std::string& name) const
	{
		const const ResourceDataPoint* dataPoint = findDataPoint(name);
		
		if (dataPoint)
		{
			return (*dataPoint).stringData;
		}

		LOGF(Error, ResourceLog, "Unable to find string parameter with name, \'%s\'", name.c_str());
		return "";
	}
	
	void ResourceData::initialize(const XMLNode& node)
	{
		for (auto i = node.getChildren().begin(); i != node.getChildren().end(); i++)
		{
			XMLNode n = (*i);

			std::string name = n.getName();
			std::string value = n.getValue();

			ResourceDataPoint dataPoint;
			dataPoint.name = name;
			dataPoint._attributes = n.getAttributes();

			bool skip = false;

			if (n.getChildren().size() > 0)
			{
				dataPoint.hasResourceData = true;
				dataPoint.resourceData.initialize(n);
				skip = true;
			}

			if (!skip)
			{

				std::string typeString = "string";

				for (auto j = n.getAttributes().begin(); j != n.getAttributes().end(); j++)
				{
					XMLAttribute a = (*j);

					if (a.getName() == "type")
					{
						typeString = a.getValue();
						break;
					}
				}

				Type type = Types::parseTypeFromString(typeString);

				dataPoint.type = type;
				dataPoint.isResourceDependency = typeString == "resource";

				if (type == Types::Boolean)
				{
					dataPoint.boolData = value == "true" || value == "1";
				}
				else if (type == Types::Float)
				{
					dataPoint.floatData = stof(value);
				}
				else if (type == Types::Double)
				{
					dataPoint.doubleData = stod(value);
				}
				else if (type == Types::Int8)
				{
					dataPoint.int8Data = (int8)stoi(value);
				}
				else if (type == Types::Int16)
				{
					dataPoint.int16Data = (int16)stoi(value);
				}
				else if (type == Types::Int32)
				{
					dataPoint.int32Data = (int32)stoi(value);
				}
				else if (type == Types::Int64)
				{
					dataPoint.int64Data = (int64)stoll(value);
				}
				else if (type == Types::Uint8)
				{
					dataPoint.uint8Data = (uint8)stoul(value);
				}
				else if (type == Types::Uint16)
				{
					dataPoint.uint16Data = (uint16)stoul(value);
				}
				else if (type == Types::Uint32)
				{
					dataPoint.uint32Data = (uint32)stoul(value);
				}
				else if (type == Types::Uint64)
				{
					dataPoint.uint64Data = (uint8)stoull(value);
				}
				else
				{
					dataPoint.stringData = value;
				}
			}

			_dataPoints.push_back(dataPoint);
		}
	}
	
	const ResourceDataPoint* ResourceData::findDataPoint(const std::string& name) const
	{
		const ResourceDataPoint* dataPoint = nullptr;

		for(std::vector<ResourceDataPoint>::const_iterator iter = _dataPoints.begin(); iter != _dataPoints.end(); iter++)
		{
			if ((*iter).name == name)
			{
				dataPoint = &(*iter);
			}
		}

		return dataPoint;
	}

	ResourceData& ResourceData::operator=(const ResourceData& other)
	{
		_dataPoints = other._dataPoints;

		return *this;
	}

	ResourceDataPoint::ResourceDataPoint(const ResourceDataPoint& copy) 
		: name(copy.name), type(copy.type), hasResourceData(copy.hasResourceData), isResourceDependency(copy.isResourceDependency)
	{
		if (type == Types::Boolean)
		{
			boolData = copy.boolData;
		}
		else if (type == Types::Float)
		{
			floatData = copy.floatData;
		}
		else if (type == Types::Double)
		{
			doubleData = copy.doubleData;
		}
		else if (type == Types::Int8)
		{
			int8Data = copy.int8Data;
		}
		else if (type == Types::Int16)
		{
			int16Data = copy.int16Data;
		}
		else if (type == Types::Int32)
		{
			int32Data = copy.int32Data;
		}
		else if (type == Types::Int64)
		{
			int64Data = copy.int64Data;
		}
		else if (type == Types::Uint8)
		{
			uint8Data = copy.uint8Data;
		}
		else if (type == Types::Uint16)
		{
			uint16Data = copy.uint16Data;
		}
		else if (type == Types::Uint32)
		{
			uint32Data = copy.uint32Data;
		}
		else if (type == Types::Uint64)
		{
			uint64Data = copy.uint64Data;
		}
		else
		{
			stringData = copy.stringData;
		}
		
		if (hasResourceData)
		{
			resourceData = copy.resourceData;
		}
	}

	ResourceDataPoint& ResourceDataPoint::operator=(const ResourceDataPoint& copy)
	{
		name = copy.name;
		type = copy.type;
		hasResourceData = copy.hasResourceData;
		isResourceDependency = copy.isResourceDependency;

		if (type == Types::Boolean)
		{
			boolData = copy.boolData;
		}
		else if (type == Types::Float)
		{
			floatData = copy.floatData;
		}
		else if (type == Types::Double)
		{
			doubleData = copy.doubleData;
		}
		else if (type == Types::Int8)
		{
			int8Data = copy.int8Data;
		}
		else if (type == Types::Int16)
		{
			int16Data = copy.int16Data;
		}
		else if (type == Types::Int32)
		{
			int32Data = copy.int32Data;
		}
		else if (type == Types::Int64)
		{
			int64Data = copy.int64Data;
		}
		else if (type == Types::Uint8)
		{
			uint8Data = copy.uint8Data;
		}
		else if (type == Types::Uint16)
		{
			uint16Data = copy.uint16Data;
		}
		else if (type == Types::Uint32)
		{
			uint32Data = copy.uint32Data;
		}
		else if (type == Types::Uint64)
		{
			uint64Data = copy.uint64Data;
		}
		else
		{
			stringData = copy.stringData;
		}

		if (hasResourceData)
		{
			resourceData = copy.resourceData;
		}

		return *this;
	}

	bool ResourceDataPoint::getBoolAttribute(const std::string& name) const
	{
		const XMLAttribute* attr = getAttribute(name);

		if (attr)
		{
			return attr->getValue() == "true" || attr->getValue() == "1";
		}

		return false;
	}

	const XMLAttribute* ResourceDataPoint::getAttribute(const std::string& name) const
	{
		std::vector<XMLAttribute>::const_iterator iter;
		for (iter = _attributes.begin(); iter != _attributes.end(); iter++)
		{
			const XMLAttribute& attr = *iter;

			if (attr.getName() == name)
			{
				return &attr;
			}
		}

		return nullptr;
	}
}