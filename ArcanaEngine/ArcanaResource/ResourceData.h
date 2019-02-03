#ifndef RESOURCE_DATA_H_
#define RESOURCE_DATA_H_

#include "ResourceDefines.h"

#include "Types.h"
#include "XMLNode.h"
#include "Array.h"
#include "Serializable.h"
#include <map>

namespace Arcana
{
	class ARCANA_RESOURCE_API ResourceDataPoint;

	class ARCANA_RESOURCE_API ResourceData
	{
	public:
		
		ResourceData();

		ResourceData(const XMLNode& node);

		ResourceData(const ResourceData& other);
		
		~ResourceData();
		
		const std::vector<ResourceDataPoint>& getDataPoints() const;
		
		bool getBoolParameter(const std::string& name) const;
		
		float getFloatParameter(const std::string& name) const;
		
		double getDoubleParameter(const std::string& name) const;
		
		char getCharParameter(const std::string& name) const;
		
		int8 getInt8Parameter(const std::string& name) const;
		
		int16 getInt16Parameter(const std::string& name) const;
		
		int32 getInt32Parameter(const std::string& name) const;
		
		int64 getInt64Parameter(const std::string& name) const;
		
		uint8 getUint8Parameter(const std::string& name) const;
		
		uint16 getUint16Parameter(const std::string& name) const;
		
		uint32 getUint32Parameter(const std::string& name) const;
		
		uint64 getUint64Parameter(const std::string& name) const;
		
		std::string getStringParameter(const std::string& name) const;
		
		template<typename T>
		T getObjectParameter(const std::string& name) const;
				

		ResourceData& operator=(const ResourceData& other);
		
	private:
	
		void initialize(const XMLNode& node);

		const ResourceDataPoint* findDataPoint(const std::string& name) const;

	public:

		std::vector<ResourceDataPoint> _dataPoints;
	};

	class ARCANA_RESOURCE_API ResourceDataPoint
	{
		friend class ResourceData;

	public:

		ResourceDataPoint() : name(""), boolData(false), hasResourceData(false), isResourceDependency(false) {};

		ResourceDataPoint(const ResourceDataPoint& data);

		~ResourceDataPoint() {};

		ResourceDataPoint& operator=(const ResourceDataPoint& data);

		std::string name;
		Type type;
		bool hasResourceData;
		bool isResourceDependency;
		ResourceData resourceData;
		std::string stringData;

		union
		{
			bool boolData;
			float floatData;
			double doubleData;
			int8 int8Data;
			int16 int16Data;
			int32 int32Data;
			int64 int64Data;
			uint8 uint8Data;
			uint16 uint16Data;
			uint32 uint32Data;
			uint64 uint64Data;
		};

		bool getBoolAttribute(const std::string& name) const;

		uint32 getUint32Attribute(const std::string& name) const;

	private:

		const XMLAttribute* getAttribute(const std::string& name) const;

	private:

		std::vector<XMLAttribute> _attributes;
	};
	
	template<typename T>
	inline T ResourceData::getObjectParameter(const std::string& name) const
	{
		if(!IsBaseOf<Serializable, T>::Value)
		{
			//LOG(Error, ResourceLog, "Object type must inherit from \'Serializable\'");
			//LOG(Info, ResourceLog, "If your object can't inherit from \'Serializable\', use another resource.");
			return T();
		}
		
		//LOGF(Error, ResourceLog, "Unable to find parameter with name, \'%s\'", name.c_str());
		return T();
	}
}

#endif // !RESOURCE_DATA_H_