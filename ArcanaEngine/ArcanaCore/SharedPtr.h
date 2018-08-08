/*#ifndef SHARED_PTR_H_
#define SHARED_PTR_H_

#include "Object.h"

#include "ArcanaLog.h"

namespace Arcana
{
	template<typename ObjectType = Object>
	class SharedPtr
	{
	public:

		SharedPtr();

		SharedPtr(ObjectType* ptr);

		SharedPtr(const SharedPtr& ptr);

	private:

		ObjectType* _pointer;
		unsigned int* _referenceCount;
	};
}

#endif // !SHARED_PTR_H_*/
