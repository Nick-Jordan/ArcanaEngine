// EngineClient.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <cstdlib>

#include "Globals.h"

#include "SmartPtr.h"

#include <iostream>

using namespace Arcana;

int main()
{
	ArcanaEngine = new Engine();

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

	system("pause");

    return 0;
}

