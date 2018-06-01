// ArrayClient.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "Array.h"
#include "Types.h"

#include <string>
#include <iostream>

using namespace Arcana;

std::string arrayToString(Array<int> array)
{
	std::string string = "[";

	for (int i = 0; i < array.size() - 1; i++)
	{
	string += std::to_string(array[i]) + ", ";
	}

	return string + std::to_string(array.getLast()) + "]";
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
	}

	system("pause");

	return 0;
}

