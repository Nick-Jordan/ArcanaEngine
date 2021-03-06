#include "ResourceData.h"
#include "XMLFile.h"

#include "StringUtils.h"
#include "ResourceManager.h"

#include <iostream>

namespace Arcana
{
	ResourceData::ResourceData() : _dataPoints(), _additionalData()
	{

	}

	ResourceData::ResourceData(const XMLNode& node)
	{
		initialize(node);
	}

	ResourceData::ResourceData(const ResourceData& other)
		: _dataPoints(other._dataPoints), _additionalData(other._additionalData), 
		_dependencies(other._dependencies)
	{

	}

	ResourceData::~ResourceData()
	{
	}


	const std::vector<ResourceDataPoint>& ResourceData::getDataPoints() const
	{
		return _dataPoints;
	}

	const std::vector<KeyValuePair<std::string, ResourceData>>& ResourceData::getAdditionalData() const
	{
		return _additionalData;
	}

	const std::vector<ResourceDependency>& ResourceData::getResourceDependencies() const
	{
		return _dependencies;
	}

	bool ResourceData::getBoolParameter(const std::string& name, bool defaultValue) const
	{
		const ResourceDataPoint* dataPoint = findDataPoint(name);

		if (dataPoint)
		{
			return (*dataPoint).BoolData;
		}

		LOGF(Error, ResourceLog, "Unable to find bool parameter with name, \'%s\'", name.c_str());
		return defaultValue;
	}

	float ResourceData::getFloatParameter(const std::string& name, float defaultValue) const
	{
		const ResourceDataPoint* dataPoint = findDataPoint(name);

		if (dataPoint)
		{
			return (*dataPoint).FloatData;
		}

		LOGF(Error, ResourceLog, "Unable to find float parameter with name, \'%s\'", name.c_str());
		return defaultValue;
	}

	double ResourceData::getDoubleParameter(const std::string& name, double defaultValue) const
	{
		const ResourceDataPoint* dataPoint = findDataPoint(name);

		if (dataPoint)
		{
			return (*dataPoint).DoubleData;
		}

		LOGF(Error, ResourceLog, "Unable to find double parameter with name, \'%s\'", name.c_str());
		return defaultValue;
	}

	char ResourceData::getCharParameter(const std::string& name, char defaultValue) const
	{
		const ResourceDataPoint* dataPoint = findDataPoint(name);

		if (dataPoint)
		{
			return (*dataPoint).Int8Data;
		}

		LOGF(Error, ResourceLog, "Unable to find char parameter with name, \'%s\'", name.c_str());
		return defaultValue;
	}

	int8 ResourceData::getInt8Parameter(const std::string& name, int8 defaultValue) const
	{
		const ResourceDataPoint* dataPoint = findDataPoint(name);

		if (dataPoint)
		{
			return (*dataPoint).Int8Data;
		}

		LOGF(Error, ResourceLog, "Unable to find int8 parameter with name, \'%s\'", name.c_str());
		return defaultValue;
	}

	int16 ResourceData::getInt16Parameter(const std::string& name, int16 defaultValue) const
	{
		const ResourceDataPoint* dataPoint = findDataPoint(name);

		if (dataPoint)
		{
			return (*dataPoint).Int16Data;
		}

		LOGF(Error, ResourceLog, "Unable to find int16 parameter with name, \'%s\'", name.c_str());
		return defaultValue;
	}

	int32 ResourceData::getInt32Parameter(const std::string& name, int32 defaultValue) const
	{
		const ResourceDataPoint* dataPoint = findDataPoint(name);

		if (dataPoint)
		{
			return (*dataPoint).Int32Data;
		}

		LOGF(Error, ResourceLog, "Unable to find int32 parameter with name, \'%s\'", name.c_str());
		return defaultValue;
	}

	int64 ResourceData::getInt64Parameter(const std::string& name, int64 defaultValue) const
	{
		const ResourceDataPoint* dataPoint = findDataPoint(name);

		if (dataPoint)
		{
			return (*dataPoint).Int64Data;
		}

		LOGF(Error, ResourceLog, "Unable to find int64 parameter with name, \'%s\'", name.c_str());
		return defaultValue;
	}

	uint8 ResourceData::getUint8Parameter(const std::string& name, uint8 defaultValue) const
	{
		const ResourceDataPoint* dataPoint = findDataPoint(name);

		if (dataPoint)
		{
			return (*dataPoint).Uint8Data;
		}

		LOGF(Error, ResourceLog, "Unable to find uint8 parameter with name, \'%s\'", name.c_str());
		return defaultValue;
	}

	uint16 ResourceData::getUint16Parameter(const std::string& name, uint16 defaultValue) const
	{
		const ResourceDataPoint* dataPoint = findDataPoint(name);

		if (dataPoint)
		{
			return (*dataPoint).Uint16Data;
		}

		LOGF(Error, ResourceLog, "Unable to find uint16 parameter with name, \'%s\'", name.c_str());
		return defaultValue;
	}

	uint32 ResourceData::getUint32Parameter(const std::string& name, uint32 defaultValue) const
	{
		const ResourceDataPoint* dataPoint = findDataPoint(name);

		if (dataPoint)
		{
			return (*dataPoint).Uint32Data;
		}

		LOGF(Error, ResourceLog, "Unable to find uint32 parameter with name, \'%s\'", name.c_str());
		return defaultValue;
	}

	uint64 ResourceData::getUint64Parameter(const std::string& name, uint64 defaultValue) const
	{
		const ResourceDataPoint* dataPoint = findDataPoint(name);

		if (dataPoint)
		{
			return (*dataPoint).Uint64Data;
		}

		LOGF(Error, ResourceLog, "Unable to find uint64 parameter with name, \'%s\'", name.c_str());
		return defaultValue;
	}

	std::string ResourceData::getStringParameter(const std::string& name, std::string defaultValue) const
	{
		const ResourceDataPoint* dataPoint = findDataPoint(name);

		if (dataPoint)
		{
			return (*dataPoint).StringData;
		}

		LOGF(Error, ResourceLog, "Unable to find string parameter with name, \'%s\'", name.c_str());
		return defaultValue;
	}

	const ResourceDataPoint* ResourceData::getDataPoint(const std::string& name) const
	{
		const ResourceDataPoint* dataPoint = findDataPoint(name);

		if (!dataPoint)
		{
			LOGF(Error, ResourceLog, "Unable to find data point with name, \'%s\'", name.c_str());
		}

		return dataPoint;
	}

	const ResourceData* ResourceData::getAdditionalData(const std::string& name) const
	{
		//for (auto i = _additionalData.begin(); i != _additionalData.end(); i++)
		for (int i = 0; i < _additionalData.size(); i++)
		{
			//const KeyValuePair<std::string, ResourceData>& p = *i;
			const KeyValuePair<std::string, ResourceData>& p = _additionalData[i];

			if (p.key == name)
			{
				return &p.value;
			}
		}

		return nullptr;
	}

	const GlobalObjectID& ResourceData::getResourceDependency(const std::string& name) const
	{
		//for (auto i = _dependencies.begin(); i != _dependencies.end(); i++)
		for(int i = 0; i < _dependencies.size(); i++)
		{
			//if ((*i).Name == name)
			//	return (*i)._id;

			if (_dependencies[i].Name == name)
				return _dependencies[i]._id;
		}

		LOGF(Error, ResourceLog, "Unable to find resource dependency, \'%s\'", name.c_str());
		
		return GlobalObjectID::Empty;
	}

	void ResourceData::initialize(const XMLNode& node)
	{
		for (auto i = node.getChildren().begin(); i != node.getChildren().end(); i++)
		{
			XMLNode n = (*i);

			std::string name = n.getName();
			std::string value = n.getValue();

			if (n.getChildren().size() > 0)
			{
				ResourceData data(n);
				data._attributes = n.getAttributes();
				_additionalData.push_back(MakePair(name, data));
				continue;
			}

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

			if (StringUtils::startsWith(typeString, "resource"))
			{
				std::vector<std::string> v = StringUtils::split(typeString, ";");
				std::string resourceType = v.size() > 1 ? v[1] : "unknown";

				ResourceDependency dep;
				dep.Name = name;
				dep.Type = resourceType;
				dep._id = GlobalObjectID(value);
				dep._attributes = n.getAttributes();

				_dependencies.push_back(dep);
				continue;
			}

			ResourceDataPoint dataPoint;
			dataPoint.Name = name;
			dataPoint._attributes = n.getAttributes();

			Type type = Types::parseTypeFromString(typeString);

			dataPoint.Type = type;

			if (type == Types::Boolean)
			{
				dataPoint.BoolData = StringUtils::convertStringToBool(value);
			}
			else if (type == Types::Float)
			{
				dataPoint.FloatData = StringUtils::convertStringToFloat(value);
			}
			else if (type == Types::Double)
			{
				dataPoint.DoubleData = StringUtils::convertStringToDouble(value);
			}
			else if (type == Types::Int8)
			{
				dataPoint.Int8Data = StringUtils::convertStringToInt8(value);
			}
			else if (type == Types::Int16)
			{
				dataPoint.Int16Data = StringUtils::convertStringToInt16(value);
			}
			else if (type == Types::Int32)
			{
				dataPoint.Int32Data = StringUtils::convertStringToInt32(value);
			}
			else if (type == Types::Int64)
			{
				dataPoint.Int64Data = StringUtils::convertStringToInt64(value);
			}
			else if (type == Types::Uint8)
			{
				dataPoint.Uint8Data = StringUtils::convertStringToUint8(value);
			}
			else if (type == Types::Uint16)
			{
				dataPoint.Uint16Data = StringUtils::convertStringToUint16(value);
			}
			else if (type == Types::Uint32)
			{
				dataPoint.Uint32Data = StringUtils::convertStringToUint32(value);
			}
			else if (type == Types::Uint64)
			{
				dataPoint.Uint64Data = StringUtils::convertStringToUint64(value);
			}
			else
			{
				dataPoint.StringData = value;
			}

			_dataPoints.push_back(dataPoint);
		}
	}

	const ResourceDataPoint* ResourceData::findDataPoint(const std::string& name) const
	{
		const ResourceDataPoint* dataPoint = nullptr;

		//for (std::vector<ResourceDataPoint>::const_iterator iter = _dataPoints.begin(); iter != _dataPoints.end(); iter++)
		for(int i = 0; i < _dataPoints.size(); i++)
		{
			//if ((*iter).Name == name)
			if(_dataPoints[i].Name == name)
			{
				//dataPoint = &(*iter);
				dataPoint = &_dataPoints[i];
			}
		}

		return dataPoint;
	}

	ResourceData& ResourceData::operator=(const ResourceData& other)
	{
		_dataPoints = other._dataPoints;
		_dependencies = other._dependencies;
		_additionalData = other._additionalData;

		return *this;
	}

	ResourceDataPoint::ResourceDataPoint(const ResourceDataPoint& copy)
		: XMLAttributeAccessor(copy), Name(copy.Name), Type(copy.Type)
	{
		if (Type == Types::Boolean)
		{
			BoolData = copy.BoolData;
		}
		else if (Type == Types::Float)
		{
			FloatData = copy.FloatData;
		}
		else if (Type == Types::Double)
		{
			DoubleData = copy.DoubleData;
		}
		else if (Type == Types::Int8)
		{
			Int8Data = copy.Int8Data;
		}
		else if (Type == Types::Int16)
		{
			Int16Data = copy.Int16Data;
		}
		else if (Type == Types::Int32)
		{
			Int32Data = copy.Int32Data;
		}
		else if (Type == Types::Int64)
		{
			Int64Data = copy.Int64Data;
		}
		else if (Type == Types::Uint8)
		{
			Uint8Data = copy.Uint8Data;
		}
		else if (Type == Types::Uint16)
		{
			Uint16Data = copy.Uint16Data;
		}
		else if (Type == Types::Uint32)
		{
			Uint32Data = copy.Uint32Data;
		}
		else if (Type == Types::Uint64)
		{
			Uint64Data = copy.Uint64Data;
		}
		else
		{
			StringData = copy.StringData;
		}

		_attributes = copy._attributes;
	}

	ResourceDataPoint::~ResourceDataPoint()
	{
		if (Type == Types::String)
		{
			StringData.~basic_string();
		}
	}

	ResourceDataPoint& ResourceDataPoint::operator=(const ResourceDataPoint& copy)
	{
		XMLAttributeAccessor::operator=(copy);

		Name = copy.Name;
		Type = copy.Type;

		if (Type == Types::Boolean)
		{
			BoolData = copy.BoolData;
		}
		else if (Type == Types::Float)
		{
			FloatData = copy.FloatData;
		}
		else if (Type == Types::Double)
		{
			DoubleData = copy.DoubleData;
		}
		else if (Type == Types::Int8)
		{
			Int8Data = copy.Int8Data;
		}
		else if (Type == Types::Int16)
		{
			Int16Data = copy.Int16Data;
		}
		else if (Type == Types::Int32)
		{
			Int32Data = copy.Int32Data;
		}
		else if (Type == Types::Int64)
		{
			Int64Data = copy.Int64Data;
		}
		else if (Type == Types::Uint8)
		{
			Uint8Data = copy.Uint8Data;
		}
		else if (Type == Types::Uint16)
		{
			Uint16Data = copy.Uint16Data;
		}
		else if (Type == Types::Uint32)
		{
			Uint32Data = copy.Uint32Data;
		}
		else if (Type == Types::Uint64)
		{
			Uint64Data = copy.Uint64Data;
		}
		else
		{
			StringData = copy.StringData;
		}

		_attributes = copy._attributes;

		return *this;
	}
}