#ifndef RESOURCE_DATA_H_
#define RESOURCE_DATA_H_

#include "ResourceDefines.h"

#include "ResourceHeader.h"
#include "Archive.h"
#include "Types.h"
#include <map>

namespace Arcana
{
	
	class ARCANA_RESOURCE_API ResourceData
	{
	public:
		
		ResourceData();

		ResourceData(const Archive& archive, ResourceHeader& header);

		ResourceData(const ResourceData& other);
		
		~ResourceData();
		
		
		bool getBoolParameter(const std::string& name);
		
		float getFloatParameter(const std::string& name);
		
		double getDoubleParameter(const std::string& name);
		
		char getCharParameter(const std::string& name);
		
		int8 getInt8Parameter(const std::string& name);
		
		int16 getInt16Parameter(const std::string& name);
		
		int32 getInt32Parameter(const std::string& name);
		
		int64 getInt64Parameter(const std::string& name);
		
		uint8 getUint8Parameter(const std::string& name);
		
		uint16 getUint16Parameter(const std::string& name);
		
		uint32 getUint32Parameter(const std::string& name);
		
		uint64 getUint64Parameter(const std::string& name);
		
		std::string getStringParameter(const std::string& name);
		
		template<typename T>
		T getObjectParameter(const std::string& name);
		
		const Archive& getArchive();
		
		const ResourceHeader& getHeader() const;


		ResourceData& operator=(const ResourceData& other);
		
	private:
	
		void initialize(ResourceHeader& header);
		
		int64 findPosition(const std::string& name);
	
		void updatePositionForString(int64 stringSize);
	
		Archive _archive;
		
		ResourceHeader _header;
	
		std::map<std::string, int64> _positions;

		int64 _stringAddition;
	};
	
	template<typename T>
	inline T ResourceData::getObjectParameter(const std::string& name)
	{
		if(!IsBaseOf<Serializable, T>::Value)
		{
			LOG(Error, ResourceLog, "Object type must inherit from \'Serializable\'");
			LOG(Info, ResourceLog, "If your object can't inherit from \'Serializable\', use another resource.");
			return T();
		}
		
		int64 pos = findPosition(name);
				
		if(pos != -1)
		{
			_archive.seek(pos);
			
			T object;
			object.readObject(_archive);
			
			return object;
		}
		
		LOGF(Error, ResourceLog, "Unable to find parameter with name, \'%s\'", name.c_str());
		return T();
	}
}

#endif // !RESOURCE_DATA_H_