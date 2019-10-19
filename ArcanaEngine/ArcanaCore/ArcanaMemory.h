#ifndef MEMORY_H_
#define MEMORY_H_

#include "CoreDefines.h"

#include "Types.h"
#include "TypeTraits.h"
#include "ArcanaTemplate.h"


//test
#include "ArcanaLog.h"

namespace Arcana
{

	/** \brief Handles memory allocations.
	 *
	 *  This class wraps platform-specific memory operations and provides simple memory management methods.
	 */

	class ARCANA_CORE_API Memory
	{
	public:
		
		enum
		{
			DEFAULT_ALIGNMENT = 0,

			MIN_ALIGNMENT = 8,
		};

		static void* memmove(void* dest, const void* src, size_t count);//SIZE_T typedef

		static int32 memcmp(const void* buf1, const void* buf2, size_t count);//SIZE_T typedef

		static void* memset(void* dest, uint8 ch, size_t count);//SIZE_T typedef

		template< class T >
		static void memset(T& src, uint8 value);

		static void* memzero(void* dest, size_t count);//SIZE_T typedef

		template< class T >
		static void memzero(T& src);

		static void* memcpy(void* dest, const void* src, size_t count);//SIZE_T typedef

		template< class T >
		static void memcpy(T& dest, const T& src);

		static void* bigblockMemcpy(void* dest, const void* src, size_t count);//SIZE_T typedef

		static void* streamingMemcpy(void* dest, const void* src, size_t count);//SIZE_T typedef

		static void memswap(void* ptr1, void* ptr2, size_t size);

		static void* systemMalloc(size_t size);//SIZE_T typedef

		static void systemFree(void* ptr);

		static void* malloc(size_t count, uint32 alignment = DEFAULT_ALIGNMENT);//SIZE_T typedef

		static void* realloc(void* original, size_t count, uint32 alignment = DEFAULT_ALIGNMENT);//SIZE_T typedef

		static void free(void* original);

		static size_t getAllocSize(void* original); //SIZE_T typedef


	public:

		//template <typename ElementType>
		//static void destructItems(ElementType* element, int32 count);

		template <typename ElementType>
		static typename EnableIf<TypeTraits<ElementType>::NeedsDestructor>::Type destructItems(ElementType* element, int32 count);

		template <typename ElementType>
		static typename EnableIf<!TypeTraits<ElementType>::NeedsDestructor>::Type destructItems(ElementType* elements, int32 count);

		template <typename DestinationElementType, typename SourceElementType>
		static void constructItems(void* dest, const SourceElementType* source, int32 count);

		//template <typename ElementType>
		//static void defaultConstructItems(void* elements, int32 count);

		template <typename ElementType>
		static typename EnableIf<!TypeTraits<ElementType>::IsZeroConstructType>::Type defaultConstructItems(void* Address, int32 Count);

		template <typename ElementType>
		static typename EnableIf<TypeTraits<ElementType>::IsZeroConstructType>::Type defaultConstructItems(void* elements, int32 count);

		template <typename DestinationElementType, typename SourceElementType>
		static void relocateConstructItems(void* dest, const SourceElementType* source, int32 count);

		template <typename ElementType>
		static typename EnableIf<TypeTraits<ElementType>::NeedsMoveConstructor>::Type moveConstructItems(void* dest, const ElementType* source, int32 num);

		template <typename ElementType>
		static typename EnableIf<!TypeTraits<ElementType>::NeedsMoveConstructor>::Type moveConstructItems(void* dest, const ElementType* source, int32 num);
	
		template <typename ElementType>
		static typename EnableIf<TypeTraits<ElementType>::NeedsMoveAssignment>::Type moveAssignItems(ElementType* dest, const ElementType* source, int32 num);

		template <typename ElementType>
		static typename EnableIf<!TypeTraits<ElementType>::NeedsMoveAssignment>::Type moveAssignItems(ElementType* dest, const ElementType* source, int32 num);
	
		template <typename ElementType>
		static typename EnableIf<TypeTraits<ElementType>::NeedsCopyAssignment>::Type copyAssignItems(ElementType* dest, const ElementType* source, int32 num);

		template <typename ElementType>
		static typename EnableIf<!TypeTraits<ElementType>::NeedsCopyAssignment>::Type copyAssignItems(ElementType* dest, const ElementType* source, int32 num);
	};


	template< class T >
	inline void Memory::memset(T& src, uint8 value)
	{
		memset(&src, value, sizeof(T));
	}

	template< class T >
	inline void Memory::memzero(T& src)
	{
		memzero(&src, sizeof(T));
	}

	template< class T >
	inline void Memory::memcpy(T& dest, const T& src)
	{
		memcpy(dest, src, sizeof(T));
	}

	template <typename ElementType>
	inline typename EnableIf<TypeTraits<ElementType>::NeedsDestructor>::Type Memory::destructItems(ElementType* element, int32 count)
	{
		while (count)
		{
			typedef ElementType DestructItemsElementTypeTypedef;

			element->DestructItemsElementTypeTypedef::~DestructItemsElementTypeTypedef();
			++element;
			--count;
		}
	}


	template <typename ElementType>
	inline typename EnableIf<!TypeTraits<ElementType>::NeedsDestructor>::Type Memory::destructItems(ElementType* elements, int32 count)
	{
	}

	template <typename DestinationElementType, typename SourceElementType>
	inline void Memory::constructItems(void* dest, const SourceElementType* source, int32 count)
	{
		memcpy(dest, source, sizeof(SourceElementType) * count);
	}

	template <typename ElementType>
	inline typename EnableIf<!TypeTraits<ElementType>::IsZeroConstructType>::Type Memory::defaultConstructItems(void* address, int32 count)
	{
		ElementType* element = (ElementType*)address;
		while (count)
		{
			new (element) ElementType;
			++element;
			--count;
		}
	}

	template <typename ElementType>
	inline typename EnableIf<TypeTraits<ElementType>::IsZeroConstructType>::Type Memory::defaultConstructItems(void* elements, int32 count)
	{
		memset(elements, 0, sizeof(ElementType) * count);
	}

	template <typename DestinationElementType, typename SourceElementType>
	inline void Memory::relocateConstructItems(void* dest, const SourceElementType* source, int32 count)
	{
		memmove(dest, source, sizeof(SourceElementType) * count);
	}

	template <typename ElementType>
	inline typename EnableIf<TypeTraits<ElementType>::NeedsMoveConstructor>::Type Memory::moveConstructItems(void* dest, const ElementType* source, int32 num)
	{
		while (num)
		{
			new (dest) ElementType((ElementType&&)* source);
			++(ElementType * &)dest;
			++source;
			--num;
		}
	}

	template <typename T>
	inline typename EnableIf<!TypeTraits<T>::NeedsMoveConstructor>::Type Memory::moveConstructItems(void* dest, const T* source, int32 num)
	{
		memmove(dest, source, sizeof(T) * num);
	}

	template <typename ElementType>
	inline typename EnableIf<TypeTraits<ElementType>::NeedsMoveAssignment>::Type Memory::moveAssignItems(ElementType* dest, const ElementType* source, int32 num)
	{
		while (num)
		{
			*dest = (ElementType&&)* source;
			++dest;
			++source;
			--num;
		}
	}

	template <typename ElementType>
	inline typename EnableIf<!TypeTraits<ElementType>::NeedsMoveAssignment>::Type Memory::moveAssignItems(ElementType* dest, const ElementType* source, int32 num)
	{
		memmove(dest, source, sizeof(ElementType) * num);
	}

	template <typename ElementType>
	inline typename EnableIf<TypeTraits<ElementType>::NeedsCopyAssignment>::Type Memory::copyAssignItems(ElementType* dest, const ElementType* source, int32 num)
	{
		while (num)
		{
			*dest = *source;
			++dest;
			++source;
			--num;
		}
	}


	template <typename ElementType>
	inline typename EnableIf<!TypeTraits<ElementType>::NeedsCopyAssignment>::Type Memory::copyAssignItems(ElementType* dest, const ElementType* source, int32 num)
	{
		memcpy(dest, source, sizeof(ElementType) * num);
	}
}

#endif // !MEMORY_H_

