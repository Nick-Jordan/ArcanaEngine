#ifndef UUID_H_
#define UUID_H_

#include "CoreDefines.h"

#include "../Dependencies/include/uuid/uuid.h"

namespace Arcana
{
	class ARCANA_CORE_API UUID
	{
	public:

		static UUID newUUID();

		static UUID Empty;

		UUID();

		UUID(const UUID& uuid);

		~UUID();

		bool isEmpty() const;

		std::string toString() const;

		UUID& operator=(const UUID& rhs);

		bool operator==(const UUID& rhs) const;

		bool operator!=(const UUID& rhs) const;

		bool operator<(const UUID& rhs) const;

	private:

		uuids::uuid _id;
	};
}

#endif // !UUID_H_