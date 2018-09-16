#ifndef RESOURCE_HEADER_H_
#define RESOURCE_HEADER_H_

#include "ResourceDefines.h"

#include "Types.h"
#include "Array.h"
#include <string>
#include <vector>

namespace Arcana
{
	
	class ARCANA_RESOURCE_API ResourceHeader
	{
	public:
	
		struct DataPoint
		{
			std::string name;
			Type type;
			int64 position;

			DataPoint();
			DataPoint(const DataPoint& other);
			~DataPoint();

			DataPoint& operator=(const DataPoint& other);
		};

		ResourceHeader();
	
		ResourceHeader(const std::string& filename);

		ResourceHeader(const ResourceHeader& other);
		
		~ResourceHeader();
		
		DataPoint nextDataPoint();
		
		bool hasDataPoints();
		
		const std::string& getType() const;

		ResourceHeader& operator=(const ResourceHeader& other);

	private:
	
		void initialize(std::string filename);
	
		std::string _type;
		int32 _index;
		int64 _lastPosition;
		std::vector<DataPoint> _dataPoints;  //SWITCH TO ARRAY
	};
	
}

#endif // !RESOURCE_HEADER_H_