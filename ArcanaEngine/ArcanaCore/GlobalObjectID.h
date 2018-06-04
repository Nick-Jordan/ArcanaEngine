#ifndef GLOBAL_OBJECT_ID_H_
#define GLOBAL_OBJECT_ID_H_

#include "CoreDefines.h"

#include <string>
#include "Types.h"

namespace Arcana
{

	class ARCANA_CORE_API GlobalObjectID
	{
	public:

		GlobalObjectID();

		GlobalObjectID(const std::string& name);

		GlobalObjectID(const std::string& name, int64 id);

		~GlobalObjectID();

		int64 hashString(const std::string& string);


		const std::string& getName() const;

		int64 getId() const;

		bool operator==(const GlobalObjectID& other);

	private:

		std::string _name;
		int64 _id;
	};

}

#endif

