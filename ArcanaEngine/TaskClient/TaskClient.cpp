// TaskClient.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>

#include "Scheduler.h"
#include "Task.h"

using namespace Arcana;

struct TestTask : public Task
{
	std::string name;

	TestTask(std::string name) : Task("TestTask"), name(name)
	{

	}

	virtual void run() override
	{
		std::cout << "Task: " << name.c_str() << std::endl;
	}
};

int main()
{
	Scheduler scheduler(3);
	scheduler.initialize();

	scheduler.schedule(new TestTask("number 1"));
	scheduler.schedule(new TestTask("number 2"));
	scheduler.schedule(new TestTask("number 3"));
	scheduler.schedule(new TestTask("number 4"));
	scheduler.schedule(new TestTask("number 5"));

	std::this_thread::sleep_for(std::chrono::seconds(3));

	scheduler.shutdown();
}
