// LogClient.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "ArcanaLog.h"

#include <iostream>

//vld
#include <vld.h>

REGISTER_CATEGORY(CustomCategory, #006600)
INITIALIZE_CATEGORY(, CustomCategory)

using namespace Arcana;

int main()
{
	LOG(Debug, CoreEngine, "Default Debug Message");
	LOG(Debug, CustomCategory, "Debug Message with Custom Category");
	LOG(Warning, CoreEngine, "Default Warning Message");
	LOG(Error, CoreEngine, "Default Error Message");
	LOG(Info, CoreEngine, "Default Info Message");

	Logger::File* customFile = new Logger::File("custom_output.html");
	Logger* customLogger = new Logger(customFile);

	LOG(customLogger, CoreEngine, "Custom Logger Message");

	LOGF(Debug, CoreEngine, "%.2f is a decimal with %i decimal digits.", 0.61f, 2);

	delete customLogger;
	delete customFile;

	system("pause");

    return 0;
}

