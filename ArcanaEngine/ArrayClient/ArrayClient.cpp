// ArrayClient.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "Array.h"
#include "Types.h"

#include <string>
#include <iostream>

#include <vld.h>

using namespace Arcana;

struct Test
{
	Test() {};
	Test(const Test& test) : array(test.array){};
	~Test() {};

	Test& operator=(const Test& test)
	{
		array = test.array;
		return *this;
	}

	Array<int> array;
};


std::string arrayToString(Array<int> array)
{
	std::string string = "[";

	for (int i = 0; i < array.size() - 1; i++)
	{
	string += std::to_string(array[i]) + ", ";
	}

	return string + std::to_string(array.getLast()) + "]";
}

std::string arrayToString(Array<Test> array)
{
	std::string string = "[";

	for (int i = 0; i < array.size() - 1; i++)
	{
		string += arrayToString(array[i].array) + ", ";
	}

	return string + arrayToString(array.getLast().array) + "]";
}

int main()
{
	{
		Array<int> array;

		array.init(10, 5);
		std::cout << "init(10, 5): " << arrayToString(array) << std::endl;
		std::cout << "Array Pointer: " << array.getData() << std::endl;
		std::cout << "Type size: " << array.getTypeSize() << std::endl;
		std::cout << "Allocated size: " << array.getAllocatedSize() << std::endl;
		std::cout << "Slack: " << array.getSlack() << std::endl;
		//checkInvariants
		array.rangeCheck(4);
		array.rangeCheck(10);
		array.rangeCheck(-1);
		std::cout << "isValidIndex(4): " << array.isValidIndex(4) << std::endl;
		std::cout << "isValidIndex(10): " << array.isValidIndex(10) << std::endl;
		std::cout << "isValidIndex(-1): " << array.isValidIndex(-1) << std::endl;
		std::cout << "Size: " << array.size() << std::endl;
		std::cout << "Max: " << array.max() << std::endl;
		std::cout << "array[4]: " << array[4] << std::endl;
		std::cout << "pop(true): " << array.pop(true) << std::endl;
		std::cout << "array: " << arrayToString(array) << std::endl;
		std::cout << "Size: " << array.size() << std::endl;
		std::cout << "Max: " << array.max() << std::endl;
		std::cout << "pop(false): " << array.pop(false) << std::endl;
		std::cout << "array: " << arrayToString(array) << std::endl;
		std::cout << "Size: " << array.size() << std::endl;
		std::cout << "Max: " << array.max() << std::endl;
		std::cout << "push(3)" << std::endl;
		array.push(3);
		std::cout << "array: " << arrayToString(array) << std::endl;
		std::cout << "Size: " << array.size() << std::endl;
		std::cout << "Max: " << array.max() << std::endl;
		std::cout << "Last: " << array.getLast() << std::endl;
		std::cout << "Top: " << array.getTop() << std::endl;
		array.shrink();
		std::cout << "Size: " << array.size() << std::endl;
		std::cout << "Max: " << array.max() << std::endl;
		int32 index;
		std::cout << "Find(3, index): " << array.find(3, index) << std::endl;
		std::cout << "Find(3, index): " << index << std::endl;
		std::cout << "Find(3): " << array.find(3) << std::endl;
		std::cout << "Find(4): " << array.find(4) << std::endl;
		//findLast
		//findLast


		std::cout << "array: " << arrayToString(array) << std::endl;
		Array<int> array2 = Array<int>(array);
		std::cout << "array2: " << arrayToString(array2) << std::endl;

		std::cout << std::endl;
		std::cout << "creating test1...." << std::endl;
		Test test1;
		test1.array.add(10);
		test1.array.add(1);
		test1.array.add(3);
		test1.array.add(4);
		std::cout << "creating test2...." << std::endl;
		Test test2;
		test2.array.add(8);
		test2.array.add(9);
		test2.array.add(7);
		test2.array.add(5);
		std::cout << "creating test3...." << std::endl;
		Test test3;
		test3.array.add(2);
		test3.array.add(10);
		test3.array.add(1);
		test3.array.add(7);

		std::cout << "creating testArray1...." << std::endl;
		Array<Test> testArray1;
		testArray1.add(test1);
		testArray1.add(test2);
		testArray1.add(test3);

		std::cout << "NeedsDestructor: " << TypeTraits<Test>::NeedsDestructor << std::endl;
		std::cout << "NeedsDestructor2: " << std::is_trivially_destructible<Test>::value << std::endl;
		std::cout << "isPod: " << std::is_pod<Test>::value << std::endl;

		std::cout << "creating testArray2 from copy...." << std::endl;
		std::cout << "testArray1: " << arrayToString(testArray1) << std::endl;
		Array<Test> testArray2 = Array<Test>(MoveTemp(testArray1));
		std::cout << testArray2.size() << std::endl;
		std::cout << "testArray2: " << arrayToString(testArray2) << std::endl;
	}

	system("pause");

	return 0;
}

