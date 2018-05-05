
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
		destructItems(getData(), _arrayNum);
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
			destructItems(getData(), _arrayNum);
			//moveOrCopy(*this, other);
		}
		return *this;
	}

	template<typename ElementType>
	Array<ElementType>::~Array()
	{
		if (_data != nullptr)
		{
			//delete[] _data;
			//_data = 0;
		}
	}

	template<typename ElementType>
	ElementType* Array<ElementType>::getData()
	{
		return _data;
	}

	template<typename ElementType>
	const ElementType* Array<ElementType>::getData() const
	{
		return _data;
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
		return _arrayMax - _arrayNUm;
	}

	template<typename ElementType>
	void Array<ElementType>::checkInvariants() const
	{
		AE_ASSERT(_arrayNum >= 0) && (_arrayMax >= _arrayNum);
	}

	template<typename ElementType>
	void Array<ElementType>::rangeCheck(int32 index) const
	{
		checkInvariants();

		if ((index >= 0) && (index < _arrayNum))
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
		return _data[index];
	}

	template<typename ElementType>
	const ElementType& Array<ElementType>::operator[](int32 index) const
	{
		rangeCheck(index);
		return _data[index];
	}

	template<typename ElementType>
	ElementType Array<ElementType>::pop(bool allowShrinking)
	{
		rangeCheck(0);
		ElementType result = _data[_arrayNum - 1];//MoveTemp
		removeAt(_arrayNum - 1, 1, allowShrinking);
		return result;
	}

	template<typename ElementType>
	void Array<ElementType>::push(ElementType&& element)
	{
		add(element); //MoveTemp
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
		return _data[_arrayNum - indexFromTheEnd - 1];
	}

	template<typename ElementType>
	const ElementType& Array<ElementType>::getLast(int32 indexFromTheEnd) const
	{
		rangeCheck(_arrayNum - indexFromTheEnd - 1);
		return _data[_arrayNum - indexFromTheEnd - 1];
	}

	template<typename ElementType>
	void Array<ElementType>::shrink()
	{
		checkInvariants();
		if (_arrayMax != _arrayNum)
		{
			_arrayMax = _arrayNum;
			//AllocatorInstance.ResizeAllocation(ArrayNum, ArrayMax, sizeof(ElementType));
		}
	}

	template<typename ElementType>
	bool Array<ElementType>::find(const ElementType& element, int32& index) const
	{
		index = this->Find(Item);
		return index != INDEX_NONE;
	}

	template<typename ElementType>
	int32 Array<ElementType>::find(const ElementType& element) const
	{
		const ElementType* start = _data;
		for (const ElementType* data = start, *dataEnd = data + _data; data != dataEnd; ++data)
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
		for (const ElementType* start = _data, *data = start + _arrayNum; data != start; )
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

		return count == otherArray.size() && compareItems(_data, otherArray.getData(), count);

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
			//ArrayMax = AllocatorInstance.CalculateSlack(ArrayNum, ArrayMax, sizeof(ElementType));
			//AllocatorInstance.ResizeAllocation(OldNum, ArrayMax, sizeof(ElementType));
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
			//ArrayMax = AllocatorInstance.CalculateSlack(ArrayNum, ArrayMax, sizeof(ElementType));
			//AllocatorInstance.ResizeAllocation(OldNum, ArrayMax, sizeof(ElementType));
		}

		ElementType* data = _data + index;
		//RelocateConstructItems<ElementType>(Data + Count, Data, OldNum - Index);
	}

	template<typename ElementType>
	void Array<ElementType>::insertZeroed(int32 index, int32 count)
	{
		insertUninitialized(index, count);
		//FMemory::Memzero((uint8*)AllocatorInstance.GetAllocation() + Index * sizeof(ElementType), Count * sizeof(ElementType));
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
		//ConstructItems<ElementType>(GetData() + Index, Ptr, Count);

		return index;
	}

	template<typename ElementType>
	void Array<ElementType>::checkAddress(const ElementType* addr) const
	{
		if (addr < _data || addr >= (_data + _arrayMax))
		{
			LOGF(Error, DynamicArray, "Attempting to add a container element (0x%08x) which already comes from the container (0x%08x, ArrayMax: %d)!", addr, _data, _arrayMax);
		}
	}

	template<typename ElementType>
	int32 Array<ElementType>::insert(ElementType&& element, int32 index)
	{
		checkAddress(&element);

		// construct a copy in place at index (this new operator will insert at 
		// index, then construct that memory with element)
		insertUninitialized(index, 1);
		new(getData() + index) ElementType(element);//MoveTemp
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

		destructItems(getData() + index, count);

		// Skip memmove in the common case that there is nothing to move.
		int32 numToMove = _arrayNum - index - count;
		if (numToMove)
		{
			//FMemory::Memmove
			//(
			//	(uint8*)AllocatorInstance.GetAllocation() + (Index) * sizeof(ElementType),
			//	(uint8*)AllocatorInstance.GetAllocation() + (Index + Count) * sizeof(ElementType),
			//	NumToMove * sizeof(ElementType)
			//);
		}
		_arrayNum -= count;

		if (allowShrinking)
		{
			const int32 newArrayMax = 0;//AllocatorInstance.CalculateSlack(ArrayNum, ArrayMax, sizeof(ElementType));
			if (newArrayMax != _arrayMax)
			{
				_arrayMax = newArrayMax;
				//AllocatorInstance.ResizeAllocation(ArrayNum, ArrayMax, sizeof(ElementType));
			}
		}
	}

	template<typename ElementType>
	void Array<ElementType>::removeAtSwap(int32 index, int32 count, bool allowShrinking)
	{
		checkInvariants();
		AE_ASSERT((count >= 0) & (index >= 0) & (index + count <= _arrayNum));

		destructItems(getData() + index, count);

		// Replace the elements in the hole created by the removal with elements from the end of the array, so the range of indices used by the array is contiguous.
		const int32 numElementsInHole = count;
		const int32 numElementsAfterHole = _arrayNum - (index + count);
		const int32 numElementsToMoveIntoHole = (std::min)(numElementsInHole, numElementsAfterHole);
		if (numElementsToMoveIntoHole)
		{
			//FMemory::Memcpy(
			//	(uint8*)AllocatorInstance.GetAllocation() + (Index) * sizeof(ElementType),
			//	(uint8*)AllocatorInstance.GetAllocation() + (ArrayNum - NumElementsToMoveIntoHole) * sizeof(ElementType),
			//	NumElementsToMoveIntoHole * sizeof(ElementType)
			//);
		}
		_arrayNum -= count;

		if (allowShrinking)
		{
			const int32 newArrayMax = 0;// AllocatorInstance.CalculateSlack(ArrayNum, ArrayMax, sizeof(ElementType));
			if (newArrayMax != _arrayMax)
			{
				_arrayMax = newArrayMax;
				//AllocatorInstance.ResizeAllocation(ArrayNum, ArrayMax, sizeof(ElementType));
			}
		}
	}

	template<typename ElementType>
	void Array<ElementType>::reset(int32 newSize)
	{
		if (newSize <= _arrayMax)
		{
			destructItems(getData(), _arrayNum);
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
		destructItems(getData(), _arrayNum);

		AE_ASSERT(slack >= 0);
		_arrayNum = 0;
		// only reallocate if we need to, I don't trust realloc to the same size to work
		if (_arrayMax != slack)
		{
			_arrayMax = slack;
			//AllocatorInstance.ResizeAllocation(0, ArrayMax, sizeof(ElementType));
		}
	}

	template<typename ElementType>
	void Array<ElementType>::setSize(int32 newSize, bool allowShrinking)
	{
		if (newSize > size())
		{
			const int32 diff = newSize - _arrayNum;
			const int32 index = addUninitialized(diff);
			//DefaultConstructItems<ElementType>((uint8*)AllocatorInstance.GetAllocation() + Index * sizeof(ElementType), Diff);
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

		//ConstructItems<ElementType>(GetData() + ArrayNum, Source.GetData(), SourceCount);

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

		//RelocateConstructItems<ElementType>(GetData() + ArrayNum, Source.GetData(), SourceCount);
		source._arrayNum = 0;

		_arrayNum += sourceCount;
	}

	template<typename ElementType>
	void Array<ElementType>::append(const ElementType* ptr, int32 count)
	{
		AE_ASSERT(ptr != nullptr);

		int32 pos = addUninitialized(count);
		//ConstructItems<ElementType>(GetData() + Pos, Ptr, Count);
	}

	template<typename ElementType>
	Array<ElementType>& Array<ElementType>::operator+=(Array<ElementType>&& other)
	{
		append(other);//MoveTemp
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
		return emplace(element); //MoveTemp
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
		//FMemory::Memzero((uint8*)AllocatorInstance.GetAllocation() + Index * sizeof(ElementType), Count * sizeof(ElementType));
		return index;
	}

	template<typename ElementType>
	int32 Array<ElementType>::addDefaulted(int32 count)
	{
		const int32 index = addUninitialized(count);
		//DefaultConstructItems<ElementType>((uint8*)AllocatorInstance.GetAllocation() + Index * sizeof(ElementType), Count);
		return index;
	}

	template<typename ElementType>
	int32 Array<ElementType>::addUnique(ElementType&& element)
	{
		return addUniqueImpl(element); //MoveTemp
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
			//AllocatorInstance.ResizeAllocation(ArrayNum, ArrayMax, sizeof(ElementType));
		}
	}

	template<typename ElementType>
	void Array<ElementType>::init(const ElementType& element, int32 number)
	{
		empty(number);
		for (int32 index = 0; index < number; ++index)
		{
			new(*this) ElementType(element);
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
		destructItems(removePtr, 1);
		const int32 nextIndex = index + 1;
		//RelocateConstructItems<ElementType>(RemovePtr, RemovePtr + 1, ArrayNum - (Index + 1));

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
		//FMemory::Memswap(
		//	(uint8*)AllocatorInstance.GetAllocation() + (sizeof(ElementType)*FirstIndexToSwap),
		//	(uint8*)AllocatorInstance.GetAllocation() + (sizeof(ElementType)*SecondIndexToSwap),
		//	sizeof(ElementType)
		//);
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
		//AllocatorInstance.ResizeAllocation(0, sourceCount + extraSlack, sizeof(ElementType));

		//ConstructItems<ElementType>(GetData(), source.getData(), sourceCount);

		_arrayNum = sourceCount;
		_arrayMax = sourceCount + extraSlack;
	}
}
