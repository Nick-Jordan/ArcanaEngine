#ifndef ARCANA_TEMPLATE_H_
#define ARCANA_TEMPLATE_H_

namespace Arcana
{
	/** \brief Copies a const value B to A.
	 *  This method destructs the previous value of A and copies B to A with placement new and a copy constructor.
	 */
	template<typename T>
	inline void Move(T& A, const T& B)
	{
		A.~T();

		new(&A) T(B);
	}

	/** \brief Moves a value B to A.
	 *  This method destructs the previous value of A and copies B to A with placement new and a copy constructor.
	 */
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

	/** \brief Casts a reference to an rvalue reference.
	 */

	template <typename T>
	inline T&& Forward(typename RemoveReference<T>::Type& Obj)
	{
		return (T&&)Obj;
	}

	/** \brief Casts a reference to an rvalue reference.
	 */
	template <typename T>
	inline T&& Forward(typename RemoveReference<T>::Type&& Obj)
	{
		return (T&&)Obj;
	}

	/** \brief Swaps two values.
	 */
	template <typename T>
	inline void Swap(T& A, T& B)
	{
		//FMemory::Memswap(&A, &B, sizeof(T));
	}

	/** \brief Swaps two values (same as Swap(A, B)).
	 */
	template <typename T>
	inline void Exchange(T& A, T& B)
	{
		Swap(A, B);
	}

	/** \brief Key/value pairs that are easy to use with custom containers.
	 *  This class acts the same as the standard pair.
	 */
	template <typename KeyType, typename ValueType>
	struct KeyValuePair
	{
		/** \brief Constructor taking a key and a value.
		 */
		KeyValuePair(const KeyType& InKey, const ValueType& InValue)
			: key(InKey), value(InValue)
		{
		}

		/** \brief Constructor taking just a key.
		 *  Creates a pair with an uninitialized value.
		 */
		KeyValuePair(const KeyType& InKey)
			: key(InKey)
		{
		}

		/** \brief Default constructor.
		 *  Creates a pair with an uninitialized key and an uninitialized value.
		 */
		KeyValuePair()
		{
		}

		/** \brief Equals operator.
		 *  Only checks key equality.
		 */
		bool operator==(const KeyValuePair& other) const
		{
			return key == other.key;
		}

		/** \brief Inequality operator.
		 *  Only checks key inequality.
		 */
		bool operator!=(const KeyValuePair& other) const
		{
			return key != other.key;
		}

		/** \brief Less than operator.
		 *  Only checks if this key is less than the other key.
		 */
		bool operator<(const KeyValuePair& other) const
		{
			return key < other.key;
		}

		/** \brief Functor operator taking two different KeyValuePairs.
		 *  Checks if this key is less than the other key.
		 */
		inline bool operator()(const KeyValuePair& A, const KeyValuePair& B) const
		{
			return A.key < B.key;
		}
		KeyType		key;
		ValueType	value;
	};
}

#endif // !ARCANA_TEMPLATE_H_