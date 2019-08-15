#ifndef STATIC_ARRAY_H_
#define STATIC_ARRAY_H_

#include "Types.h"
#include "ArcanaMemory.h"

namespace Arcana
{
	template<typename T, uint32 NumElements>
	class StaticArray
	{
	public:

		StaticArray();

		StaticArray(StaticArray<T, NumElements>&& move);

		StaticArray(const StaticArray<T, NumElements>& copy);

		~StaticArray();

		uint32 size() const;


		StaticArray<T, NumElements>& operator=(StaticArray<T, NumElements>&& move);

		StaticArray<T, NumElements>& operator=(const StaticArray<T, NumElements>& copy);

		T& operator[](uint32 index);

		const T& operator[](uint32 index) const;

		bool operator==(const StaticArray<T, NumElements>& rhs);

		bool operator!=(const StaticArray<T, NumElements>& rhs);

	private:

		T _array[NumElements];
	};
}

#include "StaticArray.inl"

#endif // !STATIC_ARRAY_H_
