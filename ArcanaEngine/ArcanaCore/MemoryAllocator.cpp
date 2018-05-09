#include "MemoryAllocator.h"

namespace Arcana
{

	MemoryAllocator::MemoryAllocator() : _data(nullptr)
	{
	}


	MemoryAllocator::~MemoryAllocator()
	{
		if (_data)
		{
			Memory::free(_data);
		}
	}

	void MemoryAllocator::moveToEmpty(MemoryAllocator& other)
	{
		AE_ASSERT(this != &other);

		if (_data)
		{
			Memory::free(_data);
		}

		_data = other._data;
		other._data = nullptr;
	}

	MemoryAllocator::UnknownType* MemoryAllocator::getAllocation() const
	{
		return _data;
	}

	void MemoryAllocator::resizeAllocation(int32 previousNumElements, int32 numElements, size_t numBytesPerElement)
	{
		if (_data || numElements)
		{
			//AE_ASSERT(((uint64)numElements*(uint64)ElementTypeInfo.GetSize() < (uint64)INT_MAX));
			_data = (UnknownType*)Memory::realloc(_data, numElements*numBytesPerElement);
		}
	}

	int32 MemoryAllocator::calculateSlack(int32 numElements, int32 numAllocatedElements, size_t numBytesPerElement) const
	{
		int32 result;

		if (numElements < numAllocatedElements)
		{
			const uint32 currentSlackElements = numAllocatedElements - numElements;
			const size_t currentSlackBytes = (numAllocatedElements - numElements)*numBytesPerElement; //SIZE_T typedef
			const bool tooManySlackBytes = currentSlackBytes >= 16384;
			const bool tooManySlackElements = 3 * numElements < 2 * numAllocatedElements;
			if ((tooManySlackBytes || tooManySlackElements) && (currentSlackElements > 64 || !numElements))
			{
				result = numElements;
			}
			else
			{
				result = numAllocatedElements;
			}
		}
		else if (numElements > 0)
		{
			const int32 firstAllocation = 4;
			if (!numAllocatedElements && numElements <= firstAllocation)
			{
				result = firstAllocation;
			}
			else
			{
				//AE_ASSERT(numElements < MAX_int32);//MAX_int32 typedef
				result = numElements + 3 * numElements / 8 + 16;

				if (numElements > result)
				{
					result = 0;//MAX_int32;
				}
			}
		}
		else
		{
			result = 0;
		}

		return result;
	}

	size_t MemoryAllocator::getAllocatedSize(int32 numAllocatedElements, size_t numBytesPerElement) const
	{
		return numAllocatedElements * numBytesPerElement;
	}

}
