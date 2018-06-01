#ifndef OBJECT_H_
#define OBJECT_H_

#include "CoreDefines.h"

#include <string>

namespace Arcana
{

	class ARCANA_CORE_API Object
	{
	public:

		Object();

		Object(const std::string& type);

		~Object();

		const std::string& getType() const;

		void yee();

	private:

		std::string _type;
	};

}

#endif // !OBJECT_H_

