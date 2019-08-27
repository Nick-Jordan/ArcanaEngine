#ifndef ARRAY_H_
#define ARRAY_H_

#include "Types.h"
#include "ArcanaLog.h"
#include "CoreDefines.h"
#include "ArcanaTemplate.h"
#include "MemoryAllocator.h"
#include "KeyValuePair.h"

#include <algorithm>

#define INDEX_NONE -1

namespace Arcana
{
	/** \brief Iterator for custom ArcanaEngine containers.
	 *
	 *  Array iterators are created easily with Array.createIterator() or Array.createConstIterator().
	 *  This class provides operators for incrementing or decrementing the array index.
	 *  Using the dereference operator returns a reference to the array element at the current index.
	 */

	template< typename ContainerType, typename T, typename IndexType>
	class IndexedContainerIterator
	{
	public:

		/** \brief IndexedContainerIterator constructor.
		 *  This constructor takes a reference to the container and the starting index as arguments.
		 *  Rarely used as containers provide createIterator() methods.
		 *  Can be used with user-created containers.
		 */

		IndexedContainerIterator(ContainerType& container, IndexType startIndex = 0)
			: _container(container), _index(startIndex)
		{
		}

		/** \brief Prefix increment operator.
		 *  Increments the index of the iterator.
		 */

		IndexedContainerIterator& operator++()
		{
			++_index;
			return *this;
		}

		/** \brief Postfix increment operator.
		 *  Increments the index of the iterator.
		 */

		IndexedContainerIterator operator++(int)
		{
			IndexedContainerIterator temp(*this);
			++_index;
			return temp;
		}

		/** \brief Prefix decrement operator.
		 *  Decrements the index of the iterator.
		 */

		IndexedContainerIterator& operator--()
		{
			--_index;
			return *this;
		}

		/** \brief Postfix decrement operator.
		 *  Decrements the index of the iterator.
		 */

		IndexedContainerIterator operator--(int)
		{
			IndexedContainerIterator temp(*this);
			--_index;
			return temp;
		}

		/** \brief Addition assignment operator.
		 *  Adds an offset to the index of the iterator.
		 */

		IndexedContainerIterator& operator+=(int32 offset)
		{
			_index += offset;
			return *this;
		}

		/** \brief Addition operator.
		 *  Adds an offset to the index of the iterator.
		 */

		IndexedContainerIterator operator+(int32 offset) const
		{
			IndexedContainerIterator temp(*this);
			return temp += offset;
		}

		/** \brief Subtraction assignment operator.
		 *  Subtracts an offset to the index of the iterator.
		 */

		IndexedContainerIterator& operator-=(int32 offset)
		{
			return *this += -offset;
		}

		/** \brief Subtraction operator.
		 *  Subtracts an offset to the index of the iterator.
		 */

		IndexedContainerIterator operator-(int32 offset) const
		{
			IndexedContainerIterator temp(*this);
			return temp -= offset;
		}

		/** \brief Dereference operator.
		 *  Returns a reference to the container element at the current index.
		 */

		T& operator* () const
		{
			return _container[_index];
		}

		/** \brief Overload of pointer member selection operator.
		 *  Returns a pointer to the container element at the current index.
		 */

		T* operator-> () const
		{
			return &_container[_index];
		}

		/** \brief Boolean conversion operator.
		 *  Returns true if the current index is valid.
		 *  Uses Array.isValidIndex().
		 */

		explicit operator bool() const
		{
			return _container.isValidIndex(_index);
		}

		/** \brief Logical negation operator.
		 *  Returns false if the current index is valid.
		 *  Uses Array.isValidIndex().
		 */

		bool operator !() const
		{
			return !(bool)*this;
		}

		/** \brief Returns the current index.
		 */

		IndexType getIndex() const
		{
			return _index;
		}

		/** \brief Resets the index to zero.
		 */

		void reset()
		{
			_index = 0;
		}

		/** \brief Relational equivalence operator.
		 *  Returns true if both the container and index of the two iterators are equal.
		 */
		friend bool operator==(const IndexedContainerIterator& lhs, const IndexedContainerIterator& rhs) { return &lhs._container == &rhs._container && lhs._index == rhs._index; }

		/** \brief Relational "is not equal to" operator.
		 *  Returns false if the container or index of the two iterators are equal.
		 */

		friend bool operator!=(const IndexedContainerIterator& lhs, const IndexedContainerIterator& rhs) { return &lhs._container != &rhs._container || lhs._index != rhs._index; }

	private:

		ContainerType& _container;  ///< The container to be iterated through.
		IndexType      _index;      ///< The current index of the iterator.
	};


	/** \brief A dynamic array implementation.
	 *  
	 *  This container class stores elements with a pointer.
	 *  It quickly adds elements by allocating slack when the number of elements is too large.
	 *  This class provides methods for managing and manipulating array elements (sorting, inserting, searching, etc.).
	 */

	template<typename T>
	class Array
	{
	public:

		/** \brief Default array constructor.
		 */

		Array();

		/** \brief Array copy constructor.
		 */

		Array(const Array<T>& other);

		/** \brief Array copy constructor with slack parameter.
		 *  Copies array normally, but adds specified extra slack.
		 */

		Array(const Array<T>& other, int32 extraSlack);

		/** \brief Array assignment operator.
		 */

		Array<T>& operator=(const Array<T>& other);

		/** \brief Array move constructor.
		 */

		Array(Array<T>&& other);

		/** \brief Array move assignment operator.
		 */

		Array<T>& operator=(Array<T>&& other);

		/** \brief Array destructor.
		 *  Frees allocated memory.
		 */

		~Array();

		/** \brief Returns a raw pointer to the array data.
		 */

		T* getData();

		/** \brief Returns a const pointer to the array data.
		 */

		const T* getData() const;

		/** \brief Returns the size in bytes of the element type.
		 */

		uint32 getTypeSize() const;

		/** \brief Returns total size in bytes allocated to this array by the MemoryAllocator.
		 */

		uint32 getAllocatedSize() const;

		/** \brief Returns the amount of slack allocated.
		 */

		int32 getSlack() const;

		/** \brief Ensures a non-negative array size and a max size greater than or equal to the current size.
		 */

		void checkInvariants() const;

		/** \brief Ensures an index is non-negative and less than the array size.
		 */

		void rangeCheck(int32 index) const;

		/** \brief Return true if the index is non-negative and less than the array size.
		*/

		bool isValidIndex(int32 index) const;

		/** \brief Returns the number of elements in the array.
		 */

		int32 size() const;

		/** \brief Returns the maximum number of elements that can be added without reallocating.
		 */

		int32 (max)() const;

		/** \brief Returns true if the array is empty.
		 */

		bool isEmpty() const;

		/** \brief Array offset operator.
		 *  Returns a reference to the element at the specified index.
		 */

		T& operator[](int32 index);

		/** \brief Array offset operator.
		 *  Returns a const reference to the element at the specified index.
		 */

		const T& operator[](int32 index) const;

		/** \brief Removes the last element in the array.
		 *  If 'allowShrinking' is true, the MemoryAllocator can decrease the array's allocated memory.
		 */

		T pop(bool allowShrinking = true);

		/** \brief Inserts an element at the end of the array.
		 */

		void push(T&& element);

		/** \brief Inserts an element at the end of the array.
		 */

		void push(const T& element);

		/** \brief Returns a reference to the element at the end of the array.
		 */

		T& getTop();

		/** \brief Returns a const reference to the element at the end of the array.
		 */

		const T& getTop() const;

		/** \brief Returns a reference to the element at a specified index from the end.
		 *  Returns a reference to the last element by default.
		 */

		T& getLast(int32 indexFromTheEnd = 0);

		/** \brief Returns a const reference to the element at a specified index from the end.
		 *  Returns a const reference to the last element by default.
		 */

		const T& getLast(int32 indexFromTheEnd = 0) const;

		/** \brief Frees excess memory allocated as slack.
		 */

		void shrink();

		/** \brief Finds an element in the array.
		 *  Returns true if the array contains the element.
		 *  Sets 'index' to the element's index in the array.
		 */

		bool find(const T& element, int32& index) const;

		/** \brief Finds an element in the array.
		 *  Returns the element's index in the array.
		 */

		int32 find(const T& element) const;

		/** \brief Finds an element in the array, searching from the end.
		 *  Returns true if the array contains the element.
		 *  Sets 'index' to the element's index in the array.
		 */

		bool findLast(const T& element, int32& index) const;

		/** \brief Finds an element in the array, searching from the end.
		 *  Returns the element's index in the array.
		 */

		int32 findLast(const T& element) const;

		/** \brief Array relational equivalence operator.
		 *  Returns true if the size and elements of this array equal another.
		 */

		bool operator==(const Array& otherArray) const;

		/** \brief Array relational 'is not equal to' operator.
		 *  Returns true if either the size or elements of this array do not equal another.
		 */

		bool operator!=(const Array& otherArray) const;

		/** \brief Adds a specified number of uninitialized elements to the array.
		 */

		int32 addUninitialized(int32 count = 1);

		/** \brief Inserts a specified number of uninitialized elements at a certain index.
		 */

		void insertUninitialized(int32 index, int32 count = 1);

		/** \brief Inserts a specified number of zeroed elements at a certain index.
		 */

		void insertZeroed(int32 index, int32 count = 1);

		/** \brief Inserts all elements of another array at a certain index.
		 */

		int32 insert(const Array<T>& elements, const int32 inIndex);

		/** \brief insert
		 */

		int32 insert(const T* ptr, int32 count, int32 index);

		/** \brief Checks if a pointer to an element already exists within the array.
		 */

		void checkAddress(const T* addr) const;

		/** \brief Inserts an element at a certain index. ((((((((((((((((((((((((((MOVETEMP))))))))))))))))))))))))))
		 */

		int32 insert(T&& element, int32 index);

		/** \brief Inserts an element at a certain index.
		 */

		int32 insert(const T& element, int32 index);

		/** \brief Removes a specified number of elements at a certain index.
		 *  If 'allowShrinking' is true, the MemoryAllocator can decrease the array's allocated memory.
		 */

		void removeAt(int32 index, int32 count = 1, bool allowShrinking = true);

		/** \brief removeAtSwap
		 * 
		 */

		void removeAtSwap(int32 index, int32 count = 1, bool allowShrinking = true);

		/** \brief reset
		 */

		void reset(int32 newSize = 0);

		/** \brief clear
		 */

		void clear(int32 slack = 0);

		/** \brief Sets the new size of the array.
		 *  Adds default elements if the new size is greater than the current size.
		 *  Removes elements if the new size is less than the current size.
		 *  'allowShrinking' is passed to removeAt if the second condition applies.
		 */

		void setSize(int32 newSize, bool allowShrinking = true);

		/** \brief Sets the new size of the array.
		 *  Adds zeroed elements if the new size is greater than the current size.
		 *  Removes elements if the new size is less than the current size.
		 */

		void setSizeZeroed(int32 newSize);  //does this need allowShrinking

		/** \brief Sets the new size of the array.
		 *  Adds uninitialized elements if the new size is greater than the current size.
		 *  Removes elements if the new size is less than the current size.
		 */

		void setSizeUninitialized(int32 newSize);  //does this need allowShrinking

		/** \brief Appends the elements of another array to the end of this array.
		 */

		void append(const Array<T>& source);

		/** \brief Appends the elements of another array to the end of this array.
		 *  Moves the elements from source to this array.
		 *  The source array ends with a size of zero.
		 */

		void append(Array<T>&& source);

		/** \brief append
		 */

		void append(const T* ptr, int32 count);

		/** \brief Array addition assignment operator.
		 *  Moves the elements from another array to this one.
		 */

		Array<T>& operator+=(Array<T>&& other);

		/** \brief Array addition assignment operator.
		 *  Appends the elements from another array to this one.
		 */

		Array<T>& operator+=(const Array<T>& other);

		int32 add(T&& element);

		int32 add(const T& element);

		int32 addZeroed(int32 count = 1);

		int32 addDefaulted(int32 count = 1);

		template <typename... ArgsType>
		int32 emplace(ArgsType&&... args)
		{
			const int32 index = addUninitialized(1);
			new(getData() + index) T(Forward<ArgsType>(args)...);
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

		bool compareItems(const T* a, const T* b, int32 count) const;

	public:

		int32 addUnique(T&& element);

		int32 addUnique(const T& element);

		void reserve(int32 number);

		void init(const T& element, int32 number);

		int32 removeSingle(const T& element);

		int32 remove(const T& element);

		int32 removeSingleSwap(const T& element, bool allowShrinking = true);

		int32 removeSwap(const T& element);

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
						Memory::memmove(&getData()[writeIndex], &getData()[runStartIndex], sizeof(T)* runLength);
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
			for (const T* start = getData(), *data = start + startIndex; data != start; )
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
			const T* start = getData();
			for (const T* data = start, *dataEnd = start + _arrayNum; data != dataEnd; ++data)
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
			const T* start = getData();
			for (const T* data = start, *dataEnd = start + _arrayNum; data != dataEnd; ++data)
			{
				if (predicate(*data))
				{
					return static_cast<int32>(data - start);
				}
			}
			return INDEX_NONE;
		}

		template <typename KeyType>
		const T* findByKey(const KeyType& key) const
		{
			return const_cast<Array*>(this)->findByKey(key);
		}

		template <typename KeyType>
		T* findByKey(const KeyType& key)
		{
			for (T* data = getData(), *dataEnd = data + _arrayNum; data != dataEnd; ++data)
			{
				if (*data == key)
				{
					return data;
				}
			}

			return nullptr;
		}

		template <typename Predicate>
		const T* findByPredicate(Predicate predicate) const
		{
			return const_cast<Array*>(this)->findByPredicate(predicate);
		}

		template <typename Predicate>
		T* findByPredicate(Predicate predicate)
		{
			for (T* data = getData(), *dataEnd = data + _arrayNum; data != dataEnd; ++data)
			{
				if (predicate(*data))
				{
					return data;
				}
			}

			return nullptr;
		}

		template <typename Predicate>
		Array<T> filterByPredicate(Predicate predicate) const
		{
			Array<T> filterResults;
			for (const T* data = getData(), *dataEnd = data + _arrayNum; data != dataEnd; ++data)
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
			for (const T* data = getData(), *dataEnd = data + _arrayNum; data != dataEnd; ++data)
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
		typedef IndexedContainerIterator<      Array, T, int32> Iterator;
		typedef IndexedContainerIterator<const Array, const T, int32> ConstIterator;

		Iterator createIterator()
		{
			return Iterator(*this);
		}

		ConstIterator createConstIterator() const
		{
			return ConstIterator(*this);
		}

	private:

		void copyToEmpty(const Array<T>& source, int32 extraSlack = 0);

		void moveOrCopy(Array<T>& toArray, Array<T>& fromArray)
		{
			toArray._memoryAllocator.moveToEmpty(fromArray._memoryAllocator);

			toArray._arrayNum = fromArray._arrayNum;
			toArray._arrayMax = fromArray._arrayMax;
			fromArray._arrayNum = 0;
			fromArray._arrayMax = 0;
		}

	protected:

		MemoryAllocator _memoryAllocator;

		int32 _arrayNum;
		int32 _arrayMax;
	};

	template<typename KeyType, typename ValueType>
	using KeyValueArray = Array<KeyValuePair<KeyType, ValueType>>;
}

#include "Array.inl"

#endif
