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
		Memory::memswap(&A, &B, sizeof(T));
	}

	/** \brief Swaps two values (same as Swap(A, B)).
	 */
	template <typename T>
	inline void Exchange(T& A, T& B)
	{
		Swap(A, B);
	}

	template <bool Predicate, typename Result = void>
	class EnableIf;

	template <typename Result>
	class EnableIf<true, Result>
	{
	public:
		typedef Result Type;
	};

	template <typename Result>
	class EnableIf<false, Result>
	{ };
}

#endif // !ARCANA_TEMPLATE_H_