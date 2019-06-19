#ifndef RESOURCE_DATA_H_
#define RESOURCE_DATA_H_

#include "ResourceDefines.h"

#include "Types.h"
#include "XMLNode.h"
#include "Array.h"
#include "Serializable.h"
#include "GlobalObjectID.h"
#include "ResourceLoggers.h"
#include <map>

namespace Arcana
{
	class ARCANA_RESOURCE_API Resource;
	class ARCANA_RESOURCE_API ResourceDataPoint;

	class ARCANA_RESOURCE_API ResourceData
	{
	public:
		
		ResourceData();

		ResourceData(const XMLNode& node);

		ResourceData(const ResourceData& other);
		
		~ResourceData();
		
		const std::vector<ResourceDataPoint>& getDataPoints() const;

		const std::vector<std::pair<std::string, ResourceData>>& getAdditionalData() const;
		
		const std::vector<std::string>& getResourceDependencies() const;

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

		const ResourceData* getAdditionalData(const std::string& name) const;

		template<typename T>
		T* getResourceDependency(const std::string& name) const;
				

		ResourceData& operator=(const ResourceData& other);
		
	private:
	
		void initialize(const XMLNode& node);

		const ResourceDataPoint* findDataPoint(const std::string& name) const;

		Resource* getLoadedResource(const GlobalObjectID& id) const;

	public:

		//attributes ???
		std::vector<std::pair<std::string, ResourceData>> _additionalData;
		std::vector<ResourceDataPoint> _dataPoints;
		std::vector<std::pair<std::string, GlobalObjectID>> _dependencies;
		std::vector<std::string> _dependencyNames;
	};

	class ARCANA_RESOURCE_API ResourceDataPoint
	{
		friend class ResourceData;

	public:

		ResourceDataPoint() : Name(""), BoolData(false) {};

		ResourceDataPoint(const ResourceDataPoint& data);

		~ResourceDataPoint();

		ResourceDataPoint& operator=(const ResourceDataPoint& data);

		std::string Name;
		Type Type;
		std::string StringData;

		union
		{
			bool BoolData;
			float FloatData;
			double DoubleData;
			int8 Int8Data;
			int16 Int16Data;
			int32 Int32Data;
			int64 Int64Data;
			uint8 Uint8Data;
			uint16 Uint16Data;
			uint32 Uint32Data;
			uint64 Uint64Data;
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

	template<typename T>
	inline T* ResourceData::getResourceDependency(const std::string& name) const
	{
		for (auto i = _dependencies.begin(); i != _dependencies.end(); i++)
		{
			if ((*i).first == name)
			{
				return dynamic_cast<T*>(getLoadedResource((*i).second));
			}
		}

		LOGF(Error, ResourceLog, "Unable to find resource dependency with name, \'%s\'", name.c_str());

		return nullptr;
	}
}

#endif // !RESOURCE_DATA_H_