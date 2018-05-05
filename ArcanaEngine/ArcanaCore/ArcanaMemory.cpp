#include "ArcanaMemory.h"

#include <memory.h>
#include <cstdlib>

namespace Arcana
{
	void* Memory::Memmove(void* dest, const void* src, size_t count)
	{
		//platform specific memmove

		return memmove(dest, src, count);
	}

	int32 Memory::Memcmp(const void* buf1, const void* buf2, size_t count)
	{
		//platform specific memcmp

		return memcmp(buf1, buf2, count);
	}

	void* Memory::Memset(void* dest, uint8 ch, size_t count)
	{
		//platform specific memset

		return memset(dest, ch, count);
	}


	void* Memory::Memzero(void* dest, size_t count)
	{
		//platform specific memzero

		return nullptr;
	}


	void* Memory::Memcpy(void* dest, const void* src, size_t count)
	{
		//platform specific memcpy

		return memcpy(dest, src, count);
	}


	void* Memory::BigBlockMemcpy(void* dest, const void* src, size_t count)
	{
		//platform specific BigBlockMemcpy
		return nullptr;
	}

	void* Memory::StreamingMemcpy(void* dest, const void* src, size_t count)
	{
		//platform specific StreamingMemcpy
		return nullptr;
	}

	void Memory::Memswap(void* ptr1, void* ptr2, size_t size)
	{
		//platform specific memswap

		
	}

	void* Memory::systemMalloc(size_t size)
	{
		return ::malloc(size);
	}

	void Memory::systemFree(void* ptr)
	{
		::free(ptr);
	}

	//all platform specific

	void* Memory::Malloc(size_t count, uint32 alignment)
	{
		return malloc(count);
	}

	void* Memory::Realloc(void* original, size_t count, uint32 alignment)
	{
		return realloc(original, count);
	}

	void Memory::Free(void* original)
	{
		return free(original);
	}

	size_t Memory::getAllocSize(void* original)
	{
		return 0;
	}
}
