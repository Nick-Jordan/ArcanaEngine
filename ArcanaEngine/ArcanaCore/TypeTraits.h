#ifndef TYPE_TRAITS_H_
#define TYPE_TRAITS_H_

#include "CoreDefines.h"
#include "Types.h"

#include <type_traits>

namespace Arcana
{
	template<typename A, typename B>
	struct AreTypesEqual;

	template<typename, typename>
	struct AreTypesEqual
	{
		enum { Value = false };
	};

	template<typename A>
	struct AreTypesEqual<A, A>
	{
		enum { Value = true };
	};

	template<typename T> struct TypeTraitsBase
	{
		enum { NeedsCopyConstructor = !std::is_trivially_copyable<T>::value && !std::is_pod<T>::value && !std::is_pod<T>::value };
		enum { NeedsCopyAssignment = !std::is_trivially_assignable<T, T>::value && !std::is_pod<T>::value };

		enum { NeedsMoveConstructor = NeedsCopyConstructor };
		enum { NeedsMoveAssignment = NeedsCopyAssignment };

		enum { NeedsDestructor = !std::is_trivially_destructible<T>::value && !std::is_pod<T>::value };

		enum { IsBytewiseComparable = std::is_enum<T>::value || std::is_arithmetic<T>::value || std::is_pointer<T>::value };

		enum { IsZeroConstructType = IsBytewiseComparable };
	};

	template<typename T> struct TypeTraits : public TypeTraitsBase<T> {};
}

#endif // !TYPE_TRAITS_H_