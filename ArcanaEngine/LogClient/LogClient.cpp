// LogClient.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "ArcanaLog.h"

#include <iostream>

REGISTER_CATEGORY(CustomCategory, #006600)
INITIALIZE_CATEGORY(, CustomCategory)

using namespace Arcana;

int main()
{
	LOG(Debug, Engine, "Default Debug Message");
	LOG(Debug, CustomCategory, "Debug Message with Custom Category");
	LOG(Warning, Engine, "Default Warning Message");
	LOG(Error, Engine, "Default Error Message");
	LOG(Info, Engine, "Default Info Message");

	Logger::File* customFile = new Logger::File("custom_output.html");
	Logger* customLogger = new Logger(customFile);

	LOG(customLogger, Engine, "Custom Logger Message");

	LOGF(Debug, Engine, "%.2f is a decimal with %i decimal digits.", 0.61f, 2);

	system("pause");

    return 0;
}

