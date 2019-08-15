
namespace Arcana
{
	template<typename T, uint32 NumElements>
	StaticArray<T, NumElements>::StaticArray()
	{
		for (uint32 i = 0; i < NumElements; i++)
		{
			new(&(*this)[i]) T;
		}
	}

	template<typename T, uint32 NumElements>
	StaticArray<T, NumElements>::StaticArray(StaticArray<T, NumElements>&& move)
	{
		Memory::moveConstructItems((void*)_array, (const T*)move._array, NumElements);
	}

	template<typename T, uint32 NumElements>
	StaticArray<T, NumElements>::StaticArray(const StaticArray<T, NumElements>& copy)
	{
		Memory::constructItems<T>((void*)_array, (const T*)copy._array, NumElements);
	}

	template<typename T, uint32 NumElements>
	StaticArray<T, NumElements>::~StaticArray()
	{
		Memory::destructItems((T*)_array, NumElements);
	}

	template<typename T, uint32 NumElements>
	uint32 StaticArray<T, NumElements>::size() const
	{
		return NumElements;
	}


	template<typename T, uint32 NumElements>
	StaticArray<T, NumElements>& StaticArray<T, NumElements>::operator=(StaticArray<T, NumElements>&& move)
	{
		Memory::moveAssignItems((T*)_array, (const T*)move._array, NumElements);
		return *this;
	}

	template<typename T, uint32 NumElements>
	StaticArray<T, NumElements>& StaticArray<T, NumElements>::operator=(const StaticArray<T, NumElements>& copy)
	{
		Memory::copyAssignItems((T*)_array, (const T*)copy._array, NumElements);
		return *this;
	}

	template<typename T, uint32 NumElements>
	T& StaticArray<T, NumElements>::operator[](uint32 index)
	{
		AE_ASSERT(index < NumElements);
		return *(T*)&_array[index];
	}

	template<typename T, uint32 NumElements>
	const T& StaticArray<T, NumElements>::operator[](uint32 index) const
	{
		AE_ASSERT(index < NumElements);
		return *(T*)& _array[index];
	}

	template<typename T, uint32 NumElements>
	bool StaticArray<T, NumElements>::operator==(const StaticArray<T, NumElements>& rhs)
	{
		for (uint32 i = 0; i < NumElements; i++)
		{
			if ((*this)[i] != rhs[i])
			{
				return false;
			}
		}
		return true;
	}

	template<typename T, uint32 NumElements>
	bool StaticArray<T, NumElements>::operator!=(const StaticArray<T, NumElements>& rhs)
	{
		for (uint32 i = 0; i < NumElements; i++)
		{
			if ((*this)[i] != rhs[i])
			{
				return true;
			}
		}
		return false;
	}
}