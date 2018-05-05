#pragma once

template<typename T>
inline void Move(T& A, const T& B)
{
	A.~T();

	new(&A) T(B);
}

template<typename T>
inline void Move(T& A, T&& B)
{
	A.~T();

	new(&A) T(MoveTemp(B));
}

template <typename T> struct RemoveReference { typedef T Type; };
template <typename T> struct RemoveReference<T& > { typedef T Type; };
template <typename T> struct RemoveReference<T&&> { typedef T Type; };

template <typename T>
inline typename RemoveReference<T>::Type&& MoveTemp(T&& Obj)
{
	return (typename RemoveReference<T>::Type&&)Obj;
}

template <typename T>
inline T&& Forward(typename RemoveReference<T>::Type& Obj)
{
	return (T&&)Obj;
}

template <typename T>
inline T&& Forward(typename RemoveReference<T>::Type&& Obj)
{
	return (T&&)Obj;
}


template <typename T>
inline void Swap(T& A, T& B)
{
	//FMemory::Memswap(&A, &B, sizeof(T));
}

template <typename T>
inline void Exchange(T& A, T& B)
{
	Swap(A, B);
}

template <typename KeyType, typename ValueType>
struct KeyValuePair
{
	KeyValuePair(const KeyType& InKey, const ValueType& InValue)
		: key(InKey), value(InValue)
	{
	}
	KeyValuePair(const KeyType& InKey)
		: key(InKey)
	{
	}
	KeyValuePair()
	{
	}
	bool operator==(const KeyValuePair& other) const
	{
		return key == other.key;
	}
	bool operator!=(const KeyValuePair& other) const
	{
		return key != other.key;
	}
	bool operator<(const KeyValuePair& other) const
	{
		return key < other.key;
	}
	inline bool operator()(const KeyValuePair& A, const KeyValuePair& B) const
	{
		return A.key < B.key;
	}
	KeyType		key;
	ValueType	value;
};