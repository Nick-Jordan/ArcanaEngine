// ThreadClient.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "ArcanaLog.h"
#include "Thread.h"
#include "Mutex.h"
#include "Lock.h"
#include "Array.h"

//vld
#include <vld.h>

using namespace Arcana;

void threadFunction()
{
	while (true)
	{
		LOG(Info, CoreEngine, "Thread function called...");
	}
}

void threadFunction2()
{
	while (true)
	{
		LOG(Info, CoreEngine, "Thread function 2 called...");
	}
}

void threadFunction3(int arg1, double arg2)
{
	while (true)
	{
		LOGF(Info, CoreEngine, "Thread function 3 called with arguments: %d, %f...", arg1, arg2);
	}
}


Array<int32> array;

Mutex mutex;

void modvar1()
{
	while (true)
	{
		Lock lock(mutex);
		array.push(1);
		LOGF(Error, CoreEngine, "Size 1: %d", array.size());
	}
}

void modvar2()
{
	while (true)
	{
		Lock lock(mutex);
		array.push(2);
		LOGF(Error, CoreEngine, "Size 2: %d", array.size());
	}
}

class Test
{
public:

	void run()
	{
		while (true)
		{
			LOG(Info, CoreEngine, "Run called from class Test...");
		}
	}
};

struct FunctorTest
{
	void operator()()
	{
		while (true)
		{
			LOG(Info, CoreEngine, "Functor used...");
		}
	}
};

int main()
{
	Thread thread(&threadFunction);
	//thread.start();

	Thread thread2(&threadFunction2);
	//thread2.start();

	Thread thread3(&threadFunction3, 10, 4.5);
	//thread3.start();

	Test test;
	Thread thread4(&Test::run, &test);
	//thread4.start();

	//Thread thread5(FunctorTest());
	//thread5.start();

	Thread thread6(&modvar1);
	thread6.start();

	Thread thread7(&modvar2);
	thread7.start();

    return 0;
}

