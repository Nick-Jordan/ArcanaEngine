// EngineClient.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <cstdlib>

#include "Globals.h"
#include "Array.h"
#include "SmartPtr.h"

#include <iostream>

using namespace Arcana;

int main()
{
	GEngine = new Engine();

	std::cout << GEngine->getType() << std::endl;

	SmartPtr p(new Object());
	p->yee();
	std::cout << p.getReferenceCount() << std::endl;
	{
		SmartPtr q = p;
		std::cout << q.getReferenceCount() << std::endl;
		q->yee();

		SmartPtr r;
		r = p;
		std::cout << r.getReferenceCount() << std::endl;
		r->yee();
	}
	std::cout << p.getReferenceCount() << std::endl;
	p->yee();


	Array<int32> array;

	array.rangeCheck(-1);
	array.add(10);
	array.add(10);
	array.add(10);
	array.add(10);
	array.add(3);
	array.add(2);
	array.add(1);
	array.insert(0, 1);
	array.remove(3);
	//array.init(10, 5);

	for (int i = 0; i < array.size(); i++)
	{
		std::cout << array[i] << " ";
	}
	std::cout << std::endl;

	system("pause");

    return 0;
}

