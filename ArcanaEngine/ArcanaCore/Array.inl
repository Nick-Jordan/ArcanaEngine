
namespace Arcana
{
	template<typename T>
	Array<T>::Array()
	{
		_arrayMax = 0;
		_arrayNum = 0;
	}

	template<typename T>
	Array<T>::Array(const Array<T>& other)
	{
		copyToEmpty(other);
	}

	template<typename T>
	Array<T>::Array(const Array<T>& other, int32 extraSlack)
	{
		copyToEmpty(other, extraSlack);
	}

	template<typename T>
	Array<T>& Array<T>::operator=(const Array<T>& other)
	{
		Memory::destructItems(getData(), _arrayNum);
		copyToEmpty(other);
		return *this;
	}

	template<typename T>
	Array<T>::Array(Array<T>&& other)
	{
		moveOrCopy(*this, other);
	}

	template<typename T>
	Array<T>& Array<T>::operator=(Array<T>&& other)
	{
		if (this != &other)
		{
			Memory::destructItems(getData(), _arrayNum);
			moveOrCopy(*this, other);
		}
		return *this;
	}

	template<typename T>
	Array<T>::~Array()
	{
		Memory::destructItems<T>(getData(), _arrayNum);
	}

	template<typename T>
	T* Array<T>::getData()
	{
		return (T*)_memoryAllocator.getAllocation();
	}

	template<typename T>
	const T* Array<T>::getData() const
	{
		return (T*)_memoryAllocator.getAllocation();
	}

	template<typename T>
	uint32 Array<T>::getTypeSize() const
	{
		return sizeof(T);
	}

	template<typename T>
	uint32 Array<T>::getAllocatedSize() const
	{
		return sizeof(T) * _arrayMax;
	}

	template<typename T>
	int32 Array<T>::getSlack() const
	{
		return _arrayMax - _arrayNum;
	}

	template<typename T>
	void Array<T>::checkInvariants() const
	{
		AE_ASSERT((_arrayNum >= 0) && (_arrayMax >= _arrayNum));
	}

	template<typename T>
	void Array<T>::rangeCheck(int32 index) const
	{
		checkInvariants();

		if ((index < 0) && (index >= _arrayNum))
		{
			//LOGF(Error, DynamicArray, "Array index out of bounds: %i from an array of size %i", index, _arrayNum);
		}
	}

	template<typename T>
	bool Array<T>::isValidIndex(int32 index) const
	{
		return index >= 0 && index < _arrayNum;
	}

	template<typename T>
	int32 Array<T>::size() const
	{
		return _arrayNum;
	}

	template<typename T>
	int32 (Array<T>::max)() const
	{
		return _arrayMax;
	}

	template<typename T>
	T& Array<T>::operator[](int32 index)
	{
		rangeCheck(index);
		return getData()[index];
	}

	template<typename T>
	const T& Array<T>::operator[](int32 index) const
	{
		rangeCheck(index);
		return getData()[index];
	}

	template<typename T>
	T Array<T>::pop(bool allowShrinking)
	{
		rangeCheck(0);
		T result = MoveTemp(getData()[_arrayNum - 1]);//MoveTemp
		removeAt(_arrayNum - 1, 1, allowShrinking);
		return result;
	}

	template<typename T>
	void Array<T>::push(T&& element)
	{
		add(MoveTemp(element)); //MoveTemp
	}

	template<typename T>
	void Array<T>::push(const T& element)
	{
		add(element);
	}

	template<typename T>
	T& Array<T>::getTop()
	{
		return getLast();
	}

	template<typename T>
	const T& Array<T>::getTop() const
	{
		return getLast();
	}

	template<typename T>
	T& Array<T>::getLast(int32 indexFromTheEnd)
	{
		rangeCheck(_arrayNum - indexFromTheEnd - 1);
		return getData()[_arrayNum - indexFromTheEnd - 1];
	}

	template<typename T>
	const T& Array<T>::getLast(int32 indexFromTheEnd) const
	{
		rangeCheck(_arrayNum - indexFromTheEnd - 1);
		return getData()[_arrayNum - indexFromTheEnd - 1];
	}

	template<typename T>
	void Array<T>::shrink()
	{
		checkInvariants();
		if (_arrayMax != _arrayNum)
		{
			_arrayMax = _arrayNum;
			_memoryAllocator.resizeAllocation(_arrayNum, _arrayMax, sizeof(T));
		}
	}

	template<typename T>
	bool Array<T>::find(const T& element, int32& index) const
	{
		index = this->find(element);
		return index != INDEX_NONE;
	}

	template<typename T>
	int32 Array<T>::find(const T& element) const
	{
		const T* start = getData();
		for (const T* data = start, *dataEnd = data + _arrayNum; data != dataEnd; ++data)
		{
			if (*data == element)
			{
				return static_cast<int32>(data - start);
			}
		}
		return INDEX_NONE;
	}

	template<typename T>
	bool Array<T>::findLast(const T& element, int32& index) const
	{
		index = this->findLast(element);
		return index != INDEX_NONE;
	}

	template<typename T>
	int32 Array<T>::findLast(const T& element) const
	{
		for (const T* start = getData(), *data = start + _arrayNum; data != start; )
		{
			--data;
			if (*data == element)
			{
				return static_cast<int32>(data - start);
			}
		}
		return INDEX_NONE;
	}

	template<typename T>
	bool Array<T>::operator==(const Array& otherArray) const
	{
		int32 count = size();

		return count == otherArray.size() && compareItems(getData(), otherArray.getData(), count);

	}

	template<typename T>
	bool Array<T>::compareItems(const T* a, const T* b, int32 count) const
	{
		if (count < 0)
			return false;

		for (int32 i = 0; i < count; i++)
		{
			if (a[i] != b[i])
			{
				return false;
			}
		}

		return true;
	}

	template<typename T>
	bool Array<T>::operator!=(const Array& otherArray) const
	{
		return !(*this == otherArray);
	}

	template<typename T>
	int32 Array<T>::addUninitialized(int32 count)
	{
		checkInvariants();
		AE_ASSERT(count >= 0);

		const int32 oldNum = _arrayNum;
		if ((_arrayNum += count) > _arrayMax)
		{
			_arrayMax = _memoryAllocator.calculateSlack(_arrayNum, _arrayMax, sizeof(T));
			_memoryAllocator.resizeAllocation(oldNum, _arrayMax, sizeof(T));
		}

		return oldNum;
	}

	template<typename T>
	void Array<T>::insertUninitialized(int32 index, int32 count)
	{
		checkInvariants();
		AE_ASSERT((count >= 0) && (index >= 0) && (index <= _arrayNum));

		const int32 oldNum = _arrayNum;
		if ((_arrayNum += count) > _arrayMax)
		{
			_arrayMax = _memoryAllocator.calculateSlack(_arrayNum, _arrayMax, sizeof(T));
			_memoryAllocator.resizeAllocation(oldNum, _arrayMax, sizeof(T));
		}

		T* data = getData() + index;
		Memory::relocateConstructItems<T>(data + count, data, oldNum - index);
	}

	template<typename T>
	void Array<T>::insertZeroed(int32 index, int32 count)
	{
		insertUninitialized(index, count);
		Memory::Memzero((uint8*)_memoryAllocator.getAllocation() + index * sizeof(T), count * sizeof(T));
	}

	template<typename T>
	int32 Array<T>::insert(const Array<T>& elements, const int32 inIndex)
	{
		AE_ASSERT(this != &elements);
		insertUninitialized(inIndex, elements.size());
		int32 index = inIndex;
		//for (auto It = Items.CreateConstIterator(); It; ++It)
		//{
		//	RangeCheck(Index);
		//	new(GetData() + Index++) T(MoveTemp(*It));
		//}
		return inIndex;
	}

	template<typename T>
	int32 Array<T>::insert(const T* ptr, int32 count, int32 index)
	{
		AE_ASSERT(ptr != nullptr);

		insertUninitialized(index, count);
		Memory::constructItems<T>(getData() + index, ptr, count);

		return index;
	}

	template<typename T>
	void Array<T>::checkAddress(const T* addr) const
	{
		if (addr >= getData() && addr < (getData() + _arrayMax))
		{
			//LOGF(Error, DynamicArray, "Attempting to add a container element (0x%08x) which already comes from the container (0x%08x, ArrayMax: %d)!", addr, getData(), _arrayMax);
		}
	}

	template<typename T>
	int32 Array<T>::insert(T&& element, int32 index)
	{
		checkAddress(&element);

		// construct a copy in place at index (this new operator will insert at 
		// index, then construct that memory with element)
		insertUninitialized(index, 1);
		new(getData() + index) T(MoveTemp(element));//MoveTemp
		return index;
	}

	template<typename T>
	int32 Array<T>::insert(const T& element, int32 index)
	{
		checkAddress(&element);

		// construct a copy in place at index (this new operator will insert at 
		// index, then construct that memory with element)
		insertUninitialized(index, 1);
		new(getData() + index) T(element);
		return index;
	}

	template<typename T>
	void Array<T>::removeAt(int32 index, int32 count, bool allowShrinking)
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
				(uint8*)_memoryAllocator.getAllocation() + (index) * sizeof(T),
				(uint8*)_memoryAllocator.getAllocation() + (index + count) * sizeof(T),
				numToMove * sizeof(T)
			);
		}
		_arrayNum -= count;

		if (allowShrinking)
		{
			const int32 newArrayMax = _memoryAllocator.calculateSlack(_arrayNum, _arrayMax, sizeof(T));
			if (newArrayMax != _arrayMax)
			{
				_arrayMax = newArrayMax;
				_memoryAllocator.resizeAllocation(_arrayNum, _arrayMax, sizeof(T));
			}
		}
	}

	template<typename T>
	void Array<T>::removeAtSwap(int32 index, int32 count, bool allowShrinking)
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
				(uint8*)_memoryAllocator.getAllocation() + (index) * sizeof(T),
				(uint8*)_memoryAllocator.getAllocation() + (_arrayNum - numElementsToMoveIntoHole) * sizeof(T),
				numElementsToMoveIntoHole * sizeof(T)
			);
		}
		_arrayNum -= count;

		if (allowShrinking)
		{
			const int32 newArrayMax = _memoryAllocator.calculateSlack(_arrayNum, _arrayMax, sizeof(T));
			if (newArrayMax != _arrayMax)
			{
				_arrayMax = newArrayMax;
				_memoryAllocator.resizeAllocation(_arrayNum, _arrayMax, sizeof(T));
			}
		}
	}

	template<typename T>
	void Array<T>::reset(int32 newSize)
	{
		if (newSize <= _arrayMax)
		{
			Memory::destructItems(getData(), _arrayNum);
			_arrayNum = 0;
		}
		else
		{
			clear(newSize);
		}
	}

	template<typename T>
	void Array<T>::clear(int32 slack)
	{
		Memory::destructItems(getData(), _arrayNum);

		AE_ASSERT(slack >= 0);
		_arrayNum = 0;
		// only reallocate if we need to, don't trust realloc to the same size to work
		if (_arrayMax != slack)
		{
			_arrayMax = slack;
			_memoryAllocator.resizeAllocation(0, _arrayMax, sizeof(T));
		}
	}

	template<typename T>
	void Array<T>::setSize(int32 newSize, bool allowShrinking)
	{
		if (newSize > size())
		{
			const int32 diff = newSize - _arrayNum;
			const int32 index = addUninitialized(diff);
			Memory::defaultConstructItems<T>((uint8*)_memoryAllocator.getAllocation() + index * sizeof(T), diff);
		}
		else if (newSize < size())
		{
			removeAt(newSize, size() - newSize, allowShrinking);
		}
	}

	template<typename T>
	void Array<T>::setSizeZeroed(int32 newSize)
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

	template<typename T>
	void Array<T>::setSizeUninitialized(int32 newSize)
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

	template<typename T>
	void Array<T>::append(const Array<T>& source)
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

		Memory::constructItems<T>(getData() + _arrayNum, source.getData(), sourceCount);

		_arrayNum += sourceCount;
	}

	template<typename T>
	void Array<T>::append(Array<T>&& source)
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

		Memory::relocateConstructItems<T>(getData() + _arrayNum, source.getData(), sourceCount);
		source._arrayNum = 0;

		_arrayNum += sourceCount;
	}

	template<typename T>
	void Array<T>::append(const T* ptr, int32 count)
	{
		AE_ASSERT(ptr != nullptr);

		int32 pos = addUninitialized(count);
		Memory::constructItems<T>(getData() + pos, ptr, count);
	}

	template<typename T>
	Array<T>& Array<T>::operator+=(Array<T>&& other)
	{
		append(MoveTemp(other));//MoveTemp
		return *this;
	}

	template<typename T>
	Array<T>& Array<T>::operator+=(const Array<T>& other)
	{
		append(other);
		return *this;
	}

	template<typename T>
	int32 Array<T>::add(T&& element)
	{
		checkAddress(&element);
		return emplace(MoveTemp(element)); //MoveTemp
	}

	template<typename T>
	int32 Array<T>::add(const T& element)
	{
		checkAddress(&element);
		return emplace(element);
	}

	template<typename T>
	int32 Array<T>::addZeroed(int32 count)
	{
		const int32 index = addUninitialized(count);
		Memory::Memzero((uint8*)_memoryAllocator.getAllocation() + index * sizeof(T), count * sizeof(T));
		return index;
	}

	template<typename T>
	int32 Array<T>::addDefaulted(int32 count)
	{
		const int32 index = addUninitialized(count);
		Memory::defaultConstructItems<T>((uint8*)_memoryAllocator.getAllocation() + index * sizeof(T), count);
		return index;
	}

	template<typename T>
	int32 Array<T>::addUnique(T&& element)
	{
		return addUniqueImpl(MoveTemp(element)); //MoveTemp
	}

	template<typename T>
	int32 Array<T>::addUnique(const T& element)
	{
		return addUniqueImpl(element);
	}

	template<typename T>
	void Array<T>::reserve(int32 number)
	{
		if (number > _arrayMax)
		{
			_arrayMax = number;
			_memoryAllocator.resizeAllocation(_arrayNum, _arrayMax, sizeof(T));
		}
	}

	template<typename T>
	void Array<T>::init(const T& element, int32 number)
	{
		/*clear(number);
		for (int32 index = 0; index < number; ++index)
		{
			new(*this) T(element);
		}*/
		for (int32 index = 0; index < number; ++index)
		{
			add(element);
		}
	}

	template<typename T>
	int32 Array<T>::removeSingle(const T& element)
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
		Memory::relocateConstructItems<T>(removePtr, removePtr + 1, _arrayNum - (index + 1));

		// Update the array count
		--_arrayNum;

		// Removed one item
		return 1;
	}

	template<typename T>
	int32 Array<T>::remove(const T& element)
	{
		checkAddress(&element);

		// Element is non-const to preserve compatibility with existing code with a non-const operator==() member function
		return removeAll([&element](T& e) { return e == element; });
	}

	template<typename T>
	int32 Array<T>::removeSingleSwap(const T& element, bool allowShrinking)
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

	template<typename T>
	int32 Array<T>::removeSwap(const T& element)
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

	template<typename T>
	void Array<T>::swapMemory(int32 firstIndexToSwap, int32 secondIndexToSwap)
	{
		Memory::Memswap(
			(uint8*)_memoryAllocator.getAllocation() + (sizeof(T)*firstIndexToSwap),
			(uint8*)_memoryAllocator.getAllocation() + (sizeof(T)*secondIndexToSwap),
			sizeof(T)
		);
	}

	template<typename T>
	void Array<T>::swap(int32 firstIndexToSwap, int32 secondIndexToSwap)
	{
		AE_ASSERT((firstIndexToSwap >= 0) && (secondIndexToSwap >= 0));
		AE_ASSERT((_arrayNum > firstIndexToSwap) && (_arrayNum > secondIndexToSwap));
		if (firstIndexToSwap != secondIndexToSwap)
		{
			swapMemory(firstIndexToSwap, secondIndexToSwap);
		}
	}

	template<typename T>
	void Array<T>::copyToEmpty(const Array<T>& source, int32 extraSlack)
	{
		AE_ASSERT(extraSlack >= 0);

		int32 sourceCount = source.size();
		_memoryAllocator.resizeAllocation(0, sourceCount + extraSlack, sizeof(T));

		Memory::constructItems<T>(getData(), source.getData(), sourceCount);

		_arrayNum = sourceCount;
		_arrayMax = sourceCount + extraSlack;
	}
}
