#ifndef MEMORY_H_
#define MEMORY_H_

#include "CoreDefines.h"

#include "Types.h"

namespace Arcana
{

	/** \brief Handles memory allocations.
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

		template <typename ElementType>
		static void destructItems(ElementType* element, int32 count);

		template <typename DestinationElementType, typename SourceElementType>
		static void constructItems(void* dest, const SourceElementType* source, int32 count);

		template <typename ElementType>
		static void defaultConstructItems(void* elements, int32 count);

		template <typename DestinationElementType, typename SourceElementType>
		static void relocateConstructItems(void* dest, const SourceElementType* source, int32 count);

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
	inline void Memory::destructItems(ElementType* element, int32 count)
	{
		while (count)
		{
			typedef ElementType DestructItemsElementTypeTypedef;

			element->DestructItemsElementTypeTypedef::~DestructItemsElementTypeTypedef();
			++element;
			--count;
		}
	}

	template <typename DestinationElementType, typename SourceElementType>
	inline void Memory::constructItems(void* dest, const SourceElementType* source, int32 count)
	{
		memcpy(dest, source, sizeof(SourceElementType) * count);
	}

	template <typename ElementType>
	inline void Memory::defaultConstructItems(void* elements, int32 count)
	{
		Memory::memset(elements, 0, sizeof(ElementType) * count);
	}

	template <typename DestinationElementType, typename SourceElementType>
	inline void Memory::relocateConstructItems(void* dest, const SourceElementType* source, int32 count)
	{
		memmove(dest, source, sizeof(SourceElementType) * count);
	}

}

#endif // !MEMORY_H_

