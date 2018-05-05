#ifndef MEMORY_H_
#define MEMORY_H_

#ifdef ARCANA_CORE_EXPORTS
#define ARCANA_CORE_API __declspec(dllexport)
#else
#define ARCANA_CORE_API __declspec(dllimport)
#endif

#include "Types.h"

namespace Arcana
{

	class ARCANA_CORE_API Memory
	{
	public:
		
		enum
		{
			DEFAULT_ALIGNMENT = 0,

			MIN_ALIGNMENT = 8,
		};

		static void* Memmove(void* dest, const void* src, size_t count);//SIZE_T typedef

		static int32 Memcmp(const void* buf1, const void* buf2, size_t count);//SIZE_T typedef

		static void* Memset(void* dest, uint8 ch, size_t count);//SIZE_T typedef

		template< class T >
		static void Memset(T& src, uint8 value);

		static void* Memzero(void* dest, size_t count);//SIZE_T typedef

		template< class T >
		static void Memzero(T& src);

		static void* Memcpy(void* dest, const void* src, size_t count);//SIZE_T typedef

		template< class T >
		static void Memcpy(T& dest, const T& src);

		static void* BigBlockMemcpy(void* dest, const void* src, size_t count);//SIZE_T typedef

		static void* StreamingMemcpy(void* dest, const void* src, size_t count);//SIZE_T typedef

		static void Memswap(void* ptr1, void* ptr2, size_t size);

		static void* systemMalloc(size_t size);//SIZE_T typedef

		static void systemFree(void* ptr);

		static void* Malloc(size_t count, uint32 alignment = DEFAULT_ALIGNMENT);//SIZE_T typedef

		static void* Realloc(void* original, size_t count, uint32 alignment = DEFAULT_ALIGNMENT);//SIZE_T typedef

		static void Free(void* original);

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
	inline void Memory::Memset(T& src, uint8 value)
	{
		Memset(&src, value, sizeof(T));
	}

	template< class T >
	inline void Memory::Memzero(T& src)
	{
		Memzero(&src, sizeof(T));
	}

	template< class T >
	inline void Memory::Memcpy(T& dest, const T& src)
	{
		Memcpy(dest, src, sizeof(T));
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
		Memcpy(dest, source, sizeof(SourceElementType) * count);
	}

	template <typename ElementType>
	inline void Memory::defaultConstructItems(void* elements, int32 count)
	{
		Memory::Memset(elements, 0, sizeof(ElementType) * count);
	}

	template <typename DestinationElementType, typename SourceElementType>
	inline void Memory::relocateConstructItems(void* dest, const SourceElementType* source, int32 count)
	{
		Memmove(dest, source, sizeof(SourceElementType) * count);
	}

}

#endif // !MEMORY_H_

