#ifndef MEMORY_ALLOCATOR_H_
#define MEMORY_ALLOCATOR_H_

#include "CoreDefines.h"

#include "Types.h"
#include "Memory.h"
#include "ArcanaMemory.h"


namespace Arcana
{

	class ARCANA_CORE_API MemoryAllocator
	{
	public:

		struct UnknownType {};

		MemoryAllocator();

		~MemoryAllocator();

		void moveToEmpty(MemoryAllocator& other);

		UnknownType* getAllocation() const;

		void resizeAllocation(int32 previousNumElements, int32 numElements, size_t numBytesPerElement); //SIZE_T typedef

		int32 calculateSlack(int32 numElements, int32 numAllocatedElements, size_t numBytesPerElement) const; //SIZE_T typedef

		size_t getAllocatedSize(int32 numAllocatedElements, size_t numBytesPerElement) const; //SIZE_T typedef

	private:

		UnknownType* _data;
	};
}

#endif // !MEMORY_ALLOCATOR_H_
