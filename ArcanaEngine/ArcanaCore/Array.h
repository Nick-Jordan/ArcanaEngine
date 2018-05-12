#ifndef ARRAY_H_
#define ARRAY_H_

#define ARCANA_CORE_EXPORTS

#ifdef ARCANA_CORE_EXPORTS
#define ARCANA_CORE_API __declspec(dllexport)
#else
#define ARCANA_CORE_API __declspec(dllimport)
#endif

#include "Types.h"
#include "ArcanaLog.h"
#include "Defines.h"
#include "ArcanaTemplate.h"
#include "MemoryAllocator.h"

#include <algorithm>

#define INDEX_NONE -1

namespace Arcana
{
	REGISTER_CATEGORY(DynamicArray, none)

	template< typename ContainerType, typename ElementType, typename IndexType>
	class IndexedContainerIterator
	{
	public:

		IndexedContainerIterator(ContainerType& container, IndexType startIndex = 0)
			: _container(container), _index(startIndex)
		{
		}

		IndexedContainerIterator& operator++()
		{
			++_index;
			return *this;
		}
		IndexedContainerIterator operator++(int)
		{
			IndexedContainerIterator temp(*this);
			++_index;
			return temp;
		}

		IndexedContainerIterator& operator--()
		{
			--_index;
			return *this;
		}
		IndexedContainerIterator operator--(int)
		{
			IndexedContainerIterator temp(*this);
			--_index;
			return temp;
		}

		IndexedContainerIterator& operator+=(int32 offset)
		{
			_index += offset;
			return *this;
		}

		IndexedContainerIterator operator+(int32 offset) const
		{
			IndexedContainerIterator temp(*this);
			return temp += offset;
		}

		IndexedContainerIterator& operator-=(int32 offset)
		{
			return *this += -offset;
		}

		IndexedContainerIterator operator-(int32 offset) const
		{
			IndexedContainerIterator temp(*this);
			return temp -= offset;
		}

		ElementType& operator* () const
		{
			return _container[_index];
		}

		ElementType* operator-> () const
		{
			return &_container[_index];
		}

		explicit operator bool() const
		{
			return _container.isValidIndex(_index);
		}

		bool operator !() const
		{
			return !(bool)*this;
		}

		IndexType getIndex() const
		{
			return _index;
		}

		void reset()
		{
			_index = 0;
		}

		friend bool operator==(const IndexedContainerIterator& lhs, const IndexedContainerIterator& rhs) { return &lhs._container == &rhs._container && lhs._index == rhs._index; }
		friend bool operator!=(const IndexedContainerIterator& lhs, const IndexedContainerIterator& rhs) { return &lhs._container != &rhs._container || lhs._index != rhs._index; }

	private:
		ContainerType& _container;
		IndexType      _index;
	};

	template<typename ElementType>
	class ARCANA_CORE_API Array
	{

	public:

		Array();

		Array(const Array<ElementType>& other);

		Array(const Array<ElementType>& other, int32 extraSlack);

		Array<ElementType>& operator=(const Array<ElementType>& other);


		Array(Array<ElementType>&& other);

		Array<ElementType>& operator=(Array<ElementType>&& other);

		~Array();

		ElementType* getData();

		const ElementType* getData() const;

		uint32 getTypeSize() const;

		uint32 getAllocatedSize() const;

		int32 getSlack() const;

		void checkInvariants() const;

		void rangeCheck(int32 index) const;

		bool isValidIndex(int32 index) const;

		int32 size() const;

		int32 max() const;

		ElementType& operator[](int32 index);

		const ElementType& operator[](int32 index) const;

		ElementType pop(bool allowShrinking = true);

		void push(ElementType&& element);

		void push(const ElementType& element);

		ElementType& getTop();

		const ElementType& getTop() const;

		ElementType& getLast(int32 indexFromTheEnd = 0);

		const ElementType& getLast(int32 indexFromTheEnd = 0) const;

		void shrink();

		bool find(const ElementType& element, int32& index) const;

		int32 find(const ElementType& element) const;

		bool findLast(const ElementType& element, int32& index) const;

		int32 findLast(const ElementType& element) const;

		bool operator==(const Array& otherArray) const;

		bool operator!=(const Array& otherArray) const;

		int32 addUninitialized(int32 count = 1);

		void insertUninitialized(int32 index, int32 count = 1);

		void insertZeroed(int32 index, int32 count = 1);

		int32 insert(const Array<ElementType>& elements, const int32 inIndex);

		int32 insert(const ElementType* ptr, int32 count, int32 index);

		void checkAddress(const ElementType* addr) const;

		int32 insert(ElementType&& element, int32 index);

		int32 insert(const ElementType& element, int32 index);

		void removeAt(int32 index, int32 count = 1, bool allowShrinking = true);

		void removeAtSwap(int32 index, int32 count = 1, bool allowShrinking = true);

		void reset(int32 newSize = 0);

		void empty(int32 slack = 0);

		void setSize(int32 newSize, bool allowShrinking = true);

		void setSizeZeroed(int32 newSize);

		void setSizeUninitialized(int32 newSize);

		void append(const Array<ElementType>& source);

		void append(Array<ElementType>&& source);

		void append(const ElementType* ptr, int32 count);

		Array<ElementType>& operator+=(Array<ElementType>&& other);

		Array<ElementType>& operator+=(const Array<ElementType>& other);

		int32 add(ElementType&& element);

		int32 add(const ElementType& element);

		int32 addZeroed(int32 count = 1);

		int32 addDefaulted(int32 count = 1);

		template <typename... ArgsType>
		int32 emplace(ArgsType&&... args)
		{
			const int32 index = addUninitialized(1);
			new(getData() + index) ElementType(Forward<ArgsType>(args)...);
			return index;
		}


	private:

		template <typename ArgsType>
		int32 addUniqueImpl(ArgsType&& args)
		{
			int32 index;
			if (find(args, index))
			{
				return index;
			}

			return add(Forward<ArgsType>(args));
		}

	public:

		int32 addUnique(ElementType&& element);

		int32 addUnique(const ElementType& element);

		void reserve(int32 number);

		void init(const ElementType& element, int32 number);

		int32 removeSingle(const ElementType& element);

		int32 remove(const ElementType& element);

		int32 removeSingleSwap(const ElementType& element, bool allowShrinking = true);

		int32 removeSwap(const ElementType& element);

		void swapMemory(int32 firstIndexToSwap, int32 secondIndexToSwap);

		void swap(int32 firstIndexToSwap, int32 secondIndexToSwap);



		//inline methods

		template <class Predicate>
		int32 removeAll(const Predicate& predicate)
		{
			const int32 originalNum = _arrayNum;
			if (!originalNum)
			{
				return 0; // nothing to do, loop assumes one item so need to deal with this edge case here
			}

			int32 writeIndex = 0;
			int32 readIndex = 0;
			bool notMatch = !predicate(getData()[readIndex]); // use a ! to guarantee it can't be anything other than zero or one
			do
			{
				int32 runStartIndex = readIndex++;
				while (readIndex < originalNum && notMatch == !predicate(getData()[readIndex]))
				{
					readIndex++;
				}
				int32 runLength = readIndex - runStartIndex;
				AE_ASSERT(runLength > 0);
				if (notMatch)
				{
					// this was a non-matching run, we need to move it
					if (writeIndex != runStartIndex)
					{
						Memory::memmove(&getData()[writeIndex], &getData()[runStartIndex], sizeof(ElementType)* runLength);
					}
					writeIndex += runLength;
				}
				else
				{
					// this was a matching run, delete it
					Memory::destructItems(getData() + runStartIndex, runLength);
				}
				notMatch = !notMatch;
			} while (readIndex < originalNum);

			_arrayNum = writeIndex;
			return originalNum - _arrayNum;
		}

		template <class Predicate>
		void removeAllSwap(const Predicate& predicate, bool allowShrinking = true)
		{
			for (int32 itemIndex = 0; itemIndex < size();)
			{
				if (predicate((*this)[itemIndex]))
				{
					removeAtSwap(itemIndex, 1, allowShrinking);
				}
				else
				{
					++itemIndex;
				}
			}
		}

		template <typename Predicate>
		int32 findLastByPredicate(Predicate predicate, int32 startIndex) const
		{
			//AE_ASSERT(startIndex >= 0 && startIndex <= this->size());
			for (const ElementType* start = getData(), *data = start + startIndex; data != start; )
			{
				--data;
				if (predicate(*data))
				{
					return static_cast<int32>(data - start);
				}
			}
			return INDEX_NONE;
		}

		template <typename Predicate>
		int32 findLastByPredicate(Predicate predicate) const
		{
			return findLastByPredicate(predicate, _arrayNum);
		}

		template <typename KeyType>
		int32 indexOfByKey(const KeyType& key) const
		{
			const ElementType* start = getData();
			for (const ElementType* data = start, *dataEnd = start + _arrayNum; data != dataEnd; ++data)
			{
				if (*data == key)
				{
					return static_cast<int32>(data - start);
				}
			}
			return INDEX_NONE;
		}

		template <typename Predicate>
		int32 indexOfByPredicate(Predicate predicate) const
		{
			const ElementType* start = getData();
			for (const ElementType* data = start, *dataEnd = start + _arrayNum; data != dataEnd; ++data)
			{
				if (predicate(*data))
				{
					return static_cast<int32>(data - start);
				}
			}
			return INDEX_NONE;
		}

		template <typename KeyType>
		const ElementType* findByKey(const KeyType& key) const
		{
			return const_cast<Array*>(this)->findByKey(key);
		}

		template <typename KeyType>
		ElementType* findByKey(const KeyType& key)
		{
			for (ElementType* data = getData(), *dataEnd = data + _arrayNum; data != dataEnd; ++data)
			{
				if (*data == key)
				{
					return data;
				}
			}

			return nullptr;
		}

		template <typename Predicate>
		const ElementType* findByPredicate(Predicate predicate) const
		{
			return const_cast<Array*>(this)->findByPredicate(predicate);
		}

		template <typename Predicate>
		ElementType* findByPredicate(Predicate predicate)
		{
			for (ElementType* data = getData(), *dataEnd = data + _arrayNum; data != dataEnd; ++data)
			{
				if (predicate(*data))
				{
					return data;
				}
			}

			return nullptr;
		}

		template <typename Predicate>
		Array<ElementType> filterByPredicate(Predicate predicate) const
		{
			Array<ElementType> filterResults;
			for (const ElementType* data = getData(), *dataEnd = data + _arrayNum; data != dataEnd; ++data)
			{
				if (predicate(*data))
				{
					filterResults.Add(*data);
				}
			}
			return filterResults;
		}

		template <typename ComparisonType>
		bool contains(const ComparisonType& element) const
		{
			for (ElementType* data = getData(), *dataEnd = data + _arrayNum; data != dataEnd; ++data)
			{
				if (*data == element)
				{
					return true;
				}
			}

			return false;
		}

		template <typename Predicate>
		bool containsByPredicate(Predicate predicate) const
		{
			return findByPredicate(predicate) != nullptr;
		}

		template <typename T>
		void* operator new(size_t size, Array<T>& array)
		{
			AE_ASSERT(size == sizeof(T));
			const int32 index = array.addUninitialized(1);
			return &array[index];
		}
		template <typename T>
		void* operator new(size_t size, Array<T>& array, int32 index)
		{
			AE_ASSERT(size == sizeof(T));
			array.insertUninitialized(index, 1);
			return &array[index];
		}

		// Iterators
		typedef IndexedContainerIterator<      Array, ElementType, int32> Iterator;
		typedef IndexedContainerIterator<const Array, const ElementType, int32> ConstIterator;

		Iterator createIterator()
		{
			return Iterator(*this);
		}

		ConstIterator createConstIterator() const
		{
			return ConstIterator(*this);
		}

	private:

		void copyToEmpty(const Array<ElementType>& source, int32 extraSlack = 0);


	protected:

		MemoryAllocator _memoryAllocator;

		int32 _arrayNum;
		int32 _arrayMax;
	};

}

#include "Array.inl"

#endif
