
namespace Arcana
{
	INITIALIZE_CATEGORY(Arcana, DynamicArray)

		template<typename ElementType>
	Array<ElementType>::Array()
	{
		_arrayMax = 0;
		_arrayNum = 0;
	}

	template<typename ElementType>
	Array<ElementType>::Array(const Array<ElementType>& other)
	{
		copyToEmpty(other);
	}

	template<typename ElementType>
	Array<ElementType>::Array(const Array<ElementType>& other, int32 extraSlack)
	{
		copyToEmpty(other, extraSlack);
	}

	template<typename ElementType>
	Array<ElementType>& Array<ElementType>::operator=(const Array<ElementType>& other)
	{
		Memory::destructItems(getData(), _arrayNum);
		copyToEmpty(other);
		return *this;
	}

	template<typename ElementType>
	Array<ElementType>::Array(Array<ElementType>&& other)
	{
		//moveOrCopy(*this, other);
	}

	template<typename ElementType>
	Array<ElementType>& Array<ElementType>::operator=(Array<ElementType>&& other)
	{
		if (this != &other)
		{
			Memory::destructItems(getData(), _arrayNum);
			//moveOrCopy(*this, other);
		}
		return *this;
	}

	template<typename ElementType>
	Array<ElementType>::~Array()
	{
		Memory::destructItems(getData(), _arrayNum);
	}

	template<typename ElementType>
	ElementType* Array<ElementType>::getData()
	{
		return (ElementType*)_memoryAllocator.getAllocation();
	}

	template<typename ElementType>
	const ElementType* Array<ElementType>::getData() const
	{
		return (ElementType*)_memoryAllocator.getAllocation();
	}

	template<typename ElementType>
	uint32 Array<ElementType>::getTypeSize() const
	{
		return sizeof(ElementType);
	}

	template<typename ElementType>
	uint32 Array<ElementType>::getAllocatedSize() const
	{
		return sizeof(ElementType) * _arrayMax;
	}

	template<typename ElementType>
	int32 Array<ElementType>::getSlack() const
	{
		return _arrayMax - _arrayNum;
	}

	template<typename ElementType>
	void Array<ElementType>::checkInvariants() const
	{
		AE_ASSERT((_arrayNum >= 0) && (_arrayMax >= _arrayNum));
	}

	template<typename ElementType>
	void Array<ElementType>::rangeCheck(int32 index) const
	{
		checkInvariants();

		if ((index < 0) && (index >= _arrayNum))
		{
			LOGF(Error, DynamicArray, "Array index out of bounds: %i from an array of size %i", index, _arrayNum);
		}
	}

	template<typename ElementType>
	bool Array<ElementType>::isValidIndex(int32 index) const
	{
		return index >= 0 && index < _arrayNum;
	}

	template<typename ElementType>
	int32 Array<ElementType>::size() const
	{
		return _arrayNum;
	}

	template<typename ElementType>
	int32 Array<ElementType>::max() const
	{
		return _arrayMax;
	}

	template<typename ElementType>
	ElementType& Array<ElementType>::operator[](int32 index)
	{
		rangeCheck(index);
		return getData()[index];
	}

	template<typename ElementType>
	const ElementType& Array<ElementType>::operator[](int32 index) const
	{
		rangeCheck(index);
		return getData()[index];
	}

	template<typename ElementType>
	ElementType Array<ElementType>::pop(bool allowShrinking)
	{
		rangeCheck(0);
		ElementType result = MoveTemp(getData()[_arrayNum - 1]);//MoveTemp
		removeAt(_arrayNum - 1, 1, allowShrinking);
		return result;
	}

	template<typename ElementType>
	void Array<ElementType>::push(ElementType&& element)
	{
		add(MoveTemp(element)); //MoveTemp
	}

	template<typename ElementType>
	void Array<ElementType>::push(const ElementType& element)
	{
		add(element);
	}

	template<typename ElementType>
	ElementType& Array<ElementType>::getTop()
	{
		return getLast();
	}

	template<typename ElementType>
	const ElementType& Array<ElementType>::getTop() const
	{
		return getLast();
	}

	template<typename ElementType>
	ElementType& Array<ElementType>::getLast(int32 indexFromTheEnd)
	{
		rangeCheck(_arrayNum - indexFromTheEnd - 1);
		return getData()[_arrayNum - indexFromTheEnd - 1];
	}

	template<typename ElementType>
	const ElementType& Array<ElementType>::getLast(int32 indexFromTheEnd) const
	{
		rangeCheck(_arrayNum - indexFromTheEnd - 1);
		return getData()[_arrayNum - indexFromTheEnd - 1];
	}

	template<typename ElementType>
	void Array<ElementType>::shrink()
	{
		checkInvariants();
		if (_arrayMax != _arrayNum)
		{
			_arrayMax = _arrayNum;
			_memoryAllocator.resizeAllocation(_arrayNum, _arrayMax, sizeof(ElementType));
		}
	}

	template<typename ElementType>
	bool Array<ElementType>::find(const ElementType& element, int32& index) const
	{
		index = this->find(element);
		return index != INDEX_NONE;
	}

	template<typename ElementType>
	int32 Array<ElementType>::find(const ElementType& element) const
	{
		const ElementType* start = getData();
		for (const ElementType* data = start, *dataEnd = data + _arrayNum; data != dataEnd; ++data)
		{
			if (*data == element)
			{
				return static_cast<int32>(data - start);
			}
		}
		return INDEX_NONE;
	}

	template<typename ElementType>
	bool Array<ElementType>::findLast(const ElementType& element, int32& index) const
	{
		index = this->findLast(element);
		return index != INDEX_NONE;
	}

	template<typename ElementType>
	int32 Array<ElementType>::findLast(const ElementType& element) const
	{
		for (const ElementType* start = getData(), *data = start + _arrayNum; data != start; )
		{
			--data;
			if (*data == element)
			{
				return static_cast<int32>(data - start);
			}
		}
		return INDEX_NONE;
	}

	template<typename ElementType>
	bool Array<ElementType>::operator==(const Array& otherArray) const
	{
		int32 count = size();

		return count == otherArray.size() && compareItems(getData(), otherArray.getData(), count);

	}

	template<typename ElementType>
	bool Array<ElementType>::operator!=(const Array& otherArray) const
	{
		return !(*this == otherArray);
	}

	template<typename ElementType>
	int32 Array<ElementType>::addUninitialized(int32 count)
	{
		checkInvariants();
		AE_ASSERT(count >= 0);

		const int32 oldNum = _arrayNum;
		if ((_arrayNum += count) > _arrayMax)
		{
			_arrayMax = _memoryAllocator.calculateSlack(_arrayNum, _arrayMax, sizeof(ElementType));
			_memoryAllocator.resizeAllocation(oldNum, _arrayMax, sizeof(ElementType));
		}

		return oldNum;
	}

	template<typename ElementType>
	void Array<ElementType>::insertUninitialized(int32 index, int32 count)
	{
		checkInvariants();
		AE_ASSERT((count >= 0) && (index >= 0) && (index <= _arrayNum));

		const int32 oldNum = _arrayNum;
		if ((_arrayNum += count) > _arrayMax)
		{
			_arrayMax = _memoryAllocator.calculateSlack(_arrayNum, _arrayMax, sizeof(ElementType));
			_memoryAllocator.resizeAllocation(oldNum, _arrayMax, sizeof(ElementType));
		}

		ElementType* data = getData() + index;
		Memory::relocateConstructItems<ElementType>(data + count, data, oldNum - index);
	}

	template<typename ElementType>
	void Array<ElementType>::insertZeroed(int32 index, int32 count)
	{
		insertUninitialized(index, count);
		Memory::Memzero((uint8*)_memoryAllocator.getAllocation() + index * sizeof(ElementType), count * sizeof(ElementType));
	}

	template<typename ElementType>
	int32 Array<ElementType>::insert(const Array<ElementType>& elements, const int32 inIndex)
	{
		AE_ASSERT(this != &elements);
		insertUninitialized(inIndex, elements.size());
		int32 index = inIndex;
		//for (auto It = Items.CreateConstIterator(); It; ++It)
		//{
		//	RangeCheck(Index);
		//	new(GetData() + Index++) ElementType(MoveTemp(*It));
		//}
		return inIndex;
	}

	template<typename ElementType>
	int32 Array<ElementType>::insert(const ElementType* ptr, int32 count, int32 index)
	{
		AE_ASSERT(ptr != nullptr);

		insertUninitialized(index, count);
		Memory::constructItems<ElementType>(getData() + index, ptr, count);

		return index;
	}

	template<typename ElementType>
	void Array<ElementType>::checkAddress(const ElementType* addr) const
	{
		if (addr >= getData() && addr < (getData() + _arrayMax))
		{
			LOGF(Error, DynamicArray, "Attempting to add a container element (0x%08x) which already comes from the container (0x%08x, ArrayMax: %d)!", addr, getData(), _arrayMax);
		}
	}

	template<typename ElementType>
	int32 Array<ElementType>::insert(ElementType&& element, int32 index)
	{
		checkAddress(&element);

		// construct a copy in place at index (this new operator will insert at 
		// index, then construct that memory with element)
		insertUninitialized(index, 1);
		new(getData() + index) ElementType(MoveTemp(element));//MoveTemp
		return index;
	}

	template<typename ElementType>
	int32 Array<ElementType>::insert(const ElementType& element, int32 index)
	{
		checkAddress(&element);

		// construct a copy in place at index (this new operator will insert at 
		// index, then construct that memory with element)
		insertUninitialized(index, 1);
		new(getData() + index) ElementType(element);
		return index;
	}

	template<typename ElementType>
	void Array<ElementType>::removeAt(int32 index, int32 count, bool allowShrinking)
	{
		checkInvariants();
		AE_ASSERT((count >= 0) && (index >= 0) && (index + count <= _arrayNum));

		Memory::destructItems(getData() + index, count);

		// Skip memmove in the common case that there is nothing to move.
		int32 numToMove = _arrayNum - index - count;
		if (numToMove)
		{
			Memory::memmove
			(
				(uint8*)_memoryAllocator.getAllocation() + (index) * sizeof(ElementType),
				(uint8*)_memoryAllocator.getAllocation() + (index + count) * sizeof(ElementType),
				numToMove * sizeof(ElementType)
			);
		}
		_arrayNum -= count;

		if (allowShrinking)
		{
			const int32 newArrayMax = _memoryAllocator.calculateSlack(_arrayNum, _arrayMax, sizeof(ElementType));
			if (newArrayMax != _arrayMax)
			{
				_arrayMax = newArrayMax;
				_memoryAllocator.resizeAllocation(_arrayNum, _arrayMax, sizeof(ElementType));
			}
		}
	}

	template<typename ElementType>
	void Array<ElementType>::removeAtSwap(int32 index, int32 count, bool allowShrinking)
	{
		checkInvariants();
		AE_ASSERT((count >= 0) && (index >= 0) && (index + count <= _arrayNum));

		Memory::destructItems(getData() + index, count);

		// Replace the elements in the hole created by the removal with elements from the end of the array, so the range of indices used by the array is contiguous.
		const int32 numElementsInHole = count;
		const int32 numElementsAfterHole = _arrayNum - (index + count);
		const int32 numElementsToMoveIntoHole = (std::min)(numElementsInHole, numElementsAfterHole);
		if (numElementsToMoveIntoHole)
		{
			Memory::memcpy(
				(uint8*)_memoryAllocator.getAllocation() + (index) * sizeof(ElementType),
				(uint8*)_memoryAllocator.getAllocation() + (_arrayNum - numElementsToMoveIntoHole) * sizeof(ElementType),
				numElementsToMoveIntoHole * sizeof(ElementType)
			);
		}
		_arrayNum -= count;

		if (allowShrinking)
		{
			const int32 newArrayMax = _memoryAllocator.calculateSlack(_arrayNum, _arrayMax, sizeof(ElementType));
			if (newArrayMax != _arrayMax)
			{
				_arrayMax = newArrayMax;
				_memoryAllocator.resizeAllocation(_arrayNum, _arrayMax, sizeof(ElementType));
			}
		}
	}

	template<typename ElementType>
	void Array<ElementType>::reset(int32 newSize)
	{
		if (newSize <= _arrayMax)
		{
			Memory::destructItems(getData(), _arrayNum);
			_arrayNum = 0;
		}
		else
		{
			empty(newSize);
		}
	}

	template<typename ElementType>
	void Array<ElementType>::empty(int32 slack)
	{
		Memory::destructItems(getData(), _arrayNum);

		AE_ASSERT(slack >= 0);
		_arrayNum = 0;
		// only reallocate if we need to, I don't trust realloc to the same size to work
		if (_arrayMax != slack)
		{
			_arrayMax = slack;
			_memoryAllocator.resizeAllocation(0, _arrayMax, sizeof(ElementType));
		}
	}

	template<typename ElementType>
	void Array<ElementType>::setSize(int32 newSize, bool allowShrinking)
	{
		if (newSize > size())
		{
			const int32 diff = newSize - _arrayNum;
			const int32 index = addUninitialized(diff);
			Memory::defaultConstructItems<ElementType>((uint8*)_memoryAllocator.getAllocation() + index * sizeof(ElementType), diff);
		}
		else if (newSize < size())
		{
			removeAt(newSize, size() - newSize, allowShrinking);
		}
	}

	template<typename ElementType>
	void Array<ElementType>::setSizeZeroed(int32 newSize)
	{
		if (newSize > size())
		{
			addZeroed(newSize - size());
		}
		else if (newSize < size())
		{
			removeAt(newSize, size() - newSize);
		}
	}

	template<typename ElementType>
	void Array<ElementType>::setSizeUninitialized(int32 newSize)
	{
		if (newSize > size())
		{
			addUninitialized(newSize - size());
		}
		else if (newSize < size())
		{
			removeAt(newSize, size() - newSize);
		}
	}

	template<typename ElementType>
	void Array<ElementType>::append(const Array<ElementType>& source)
	{
		AE_ASSERT(this != &source);

		int32 sourceCount = source.size();

		// Do nothing if the source is empty.
		if (!sourceCount)
		{
			return;
		}

		// Allocate memory for the new elements.
		reserve(_arrayNum + sourceCount);

		Memory::constructItems<ElementType>(getData() + _arrayNum, source.getData(), sourceCount);

		_arrayNum += sourceCount;
	}

	template<typename ElementType>
	void Array<ElementType>::append(Array<ElementType>&& source)
	{
		AE_ASSERT(this != &source);

		int32 sourceCount = source.size();

		// Do nothing if the source is empty.
		if (!sourceCount)
		{
			return;
		}

		// Allocate memory for the new elements.
		reserve(_arrayNum + sourceCount);

		Memory::relocateConstructItems<ElementType>(getData() + _arrayNum, source.getData(), sourceCount);
		source._arrayNum = 0;

		_arrayNum += sourceCount;
	}

	template<typename ElementType>
	void Array<ElementType>::append(const ElementType* ptr, int32 count)
	{
		AE_ASSERT(ptr != nullptr);

		int32 pos = addUninitialized(count);
		Memory::constructItems<ElementType>(getData() + pos, ptr, count);
	}

	template<typename ElementType>
	Array<ElementType>& Array<ElementType>::operator+=(Array<ElementType>&& other)
	{
		append(MoveTemp(other));//MoveTemp
		return *this;
	}

	template<typename ElementType>
	Array<ElementType>& Array<ElementType>::operator+=(const Array<ElementType>& other)
	{
		append(other);
		return *this;
	}

	template<typename ElementType>
	int32 Array<ElementType>::add(ElementType&& element)
	{
		checkAddress(&element);
		return emplace(MoveTemp(element)); //MoveTemp
	}

	template<typename ElementType>
	int32 Array<ElementType>::add(const ElementType& element)
	{
		checkAddress(&element);
		return emplace(element);
	}

	template<typename ElementType>
	int32 Array<ElementType>::addZeroed(int32 count)
	{
		const int32 index = addUninitialized(count);
		Memory::Memzero((uint8*)_memoryAllocator.getAllocation() + index * sizeof(ElementType), count * sizeof(ElementType));
		return index;
	}

	template<typename ElementType>
	int32 Array<ElementType>::addDefaulted(int32 count)
	{
		const int32 index = addUninitialized(count);
		Memory::defaultConstructItems<ElementType>((uint8*)_memoryAllocator.getAllocation() + index * sizeof(ElementType), count);
		return index;
	}

	template<typename ElementType>
	int32 Array<ElementType>::addUnique(ElementType&& element)
	{
		return addUniqueImpl(MoveTemp(element)); //MoveTemp
	}

	template<typename ElementType>
	int32 Array<ElementType>::addUnique(const ElementType& element)
	{
		return addUniqueImpl(element);
	}

	template<typename ElementType>
	void Array<ElementType>::reserve(int32 number)
	{
		if (number > _arrayMax)
		{
			_arrayMax = number;
			_memoryAllocator.resizeAllocation(_arrayNum, _arrayMax, sizeof(ElementType));
		}
	}

	template<typename ElementType>
	void Array<ElementType>::init(const ElementType& element, int32 number)
	{
		/*empty(number);
		for (int32 index = 0; index < number; ++index)
		{
			new(*this) ElementType(element);
		}*/
		for (int32 index = 0; index < number; ++index)
		{
			add(element);
		}
	}

	template<typename ElementType>
	int32 Array<ElementType>::removeSingle(const ElementType& element)
	{
		int32 index = find(element);
		if (index == INDEX_NONE)
		{
			return 0;
		}

		auto* removePtr = getData() + index;

		// Destruct items that match the specified Item.
		Memory::destructItems(removePtr, 1);
		const int32 nextIndex = index + 1;
		Memory::relocateConstructItems<ElementType>(removePtr, removePtr + 1, _arrayNum - (index + 1));

		// Update the array count
		--_arrayNum;

		// Removed one item
		return 1;
	}

	template<typename ElementType>
	int32 Array<ElementType>::remove(const ElementType& element)
	{
		checkAddress(&element);

		// Element is non-const to preserve compatibility with existing code with a non-const operator==() member function
		return removeAll([&element](ElementType& e) { return e == element; });
	}

	template<typename ElementType>
	int32 Array<ElementType>::removeSingleSwap(const ElementType& element, bool allowShrinking)
	{
		int32 index = find(element);
		if (index == INDEX_NONE)
		{
			return 0;
		}

		removeAtSwap(index, 1, allowShrinking);

		// Removed one item
		return 1;
	}

	template<typename ElementType>
	int32 Array<ElementType>::removeSwap(const ElementType& element)
	{
		checkAddress(&element);

		const int32 originalNum = _arrayNum;
		for (int32 index = 0; index < _arrayNum; index++)
		{
			if ((*this)[index] == element)
			{
				removeAtSwap(index--);
			}
		}
		return originalNum - _arrayNum;
	}

	template<typename ElementType>
	void Array<ElementType>::swapMemory(int32 firstIndexToSwap, int32 secondIndexToSwap)
	{
		Memory::Memswap(
			(uint8*)_memoryAllocator.getAllocation() + (sizeof(ElementType)*firstIndexToSwap),
			(uint8*)_memoryAllocator.getAllocation() + (sizeof(ElementType)*secondIndexToSwap),
			sizeof(ElementType)
		);
	}

	template<typename ElementType>
	void Array<ElementType>::swap(int32 firstIndexToSwap, int32 secondIndexToSwap)
	{
		AE_ASSERT((firstIndexToSwap >= 0) && (secondIndexToSwap >= 0));
		AE_ASSERT((_arrayNum > firstIndexToSwap) && (_arrayNum > secondIndexToSwap));
		if (firstIndexToSwap != secondIndexToSwap)
		{
			swapMemory(firstIndexToSwap, secondIndexToSwap);
		}
	}

	template<typename ElementType>
	void Array<ElementType>::copyToEmpty(const Array<ElementType>& source, int32 extraSlack)
	{
		AE_ASSERT(extraSlack >= 0);

		int32 sourceCount = source.size();
		_memoryAllocator.resizeAllocation(0, sourceCount + extraSlack, sizeof(ElementType));

		Memory::constructItems<ElementType>(getData(), source.getData(), sourceCount);

		_arrayNum = sourceCount;
		_arrayMax = sourceCount + extraSlack;
	}
}
