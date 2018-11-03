// InputClient.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "Key.h"

#include <iostream>

//vld
#include <vld.h>

//dependencies
#include "CoreModule.h"
#include "InputModule.h"
 
using namespace Arcana;

int main()
{
	std::cout << "A" << std::endl;
	std::cout << Keys::A.isModifierKey() << std::endl;
	std::cout << Keys::A.getGlobalObjectID().getName() << std::endl;
	std::cout << Keys::A.getGlobalObjectID().getId() << std::endl;
	std::cout << "Left Shift" << std::endl;
	std::cout << Keys::LeftShift.isModifierKey() << std::endl;
	std::cout << Keys::LeftShift.getGlobalObjectID().getName() << std::endl;
	std::cout << Keys::LeftShift.getGlobalObjectID().getId() << std::endl;

	Key key = 9;

	std::cout << key.isModifierKey() << std::endl;
	std::cout << key.getGlobalObjectID().getName() << std::endl;
	std::cout << key.getGlobalObjectID().getId() << std::endl;

	system("pause");

    return 0;
}

