#include "stdafx.h"

#include "Archive.h"
#include "TestClass.h"

#include "FileOutputStream.h"
#include "FileInputStream.h"

#include <iostream>

//vld
#include <vld.h>

//dependencies
#include "CoreModule.h"
#include "IOModule.h"
#include "ArcanaLog.h"

using namespace Arcana;
using namespace Test;

int main()
{
	bool output = false;

	if (output)
	{
		TestClass c(10230, 253234);
		c.other = OtherClass(593.2f, "Hello, World!");

		Archive archive;

		c.writeObject(archive);

		std::cout << "Archive size: " << archive.size() << std::endl;

		FileOutputStream ostream;
		ostream.open("outfile.dat");
		archive.writeToOutputStream(ostream);
	}
	else
	{
		FileInputStream istream;
		istream.open("outfile.dat");
		Archive archive(istream);

		TestClass c;
		c.readObject(archive);

		std::cout << "Test Class: " << std::endl;
		std::cout << "integer: " << c.integer << std::endl;
		std::cout << "integer2: " << c.integer2 << std::endl;
		std::cout << "other.f: " << c.other.f << std::endl;
		std::cout << "other.str: " << c.other.str << std::endl;
	}

	system("pause");

	return 0;
}