// LogClient.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "ArcanaLog.h"

#include <iostream>

REGISTER_CATEGORY(TestCategory, "#FF0000")

int main()
{
	Arcana::LOG(Debug, Engine, "test message");
	Arcana::LOG(Debug, TestCategory, "test message");

    return 0;
}

