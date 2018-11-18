// SceneMathClient.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "Quaternion.h"
#include "Vector3.h"

#include <iostream>

using namespace Arcana;

void printVector(const char* name, const Vector3f& vec)
{
	std::cout << name << " = <" << vec.x << ", " << vec.y << ", " << vec.z << ">" << std::endl;
}

int main()
{
	Quaternionf quat;
	quat.fromAxisAngle(Vector3f::unitX(), 90.0f);

	Vector3f vec = Vector3f(0.0, 0.0, 10.0);
	Vector3f first = quat * vec;
	Vector3f second = first * quat.conjugate();
	Vector3f vecR = quat.rotate(vec);

	std::cout << "quat = <" << quat.w << ", " << quat.x << ", " << quat.y << ", " << quat.z << ">" << std::endl;
	std::cout << "quat.conjugate() = <" << quat.conjugate().w << ", " << quat.conjugate().x << ", " << quat.conjugate().y << ", " << quat.conjugate().z << ">" << std::endl;
	printVector("vec", vec);
	printVector("first", first);
	printVector("second", second);
	printVector("vecR", vecR);

	system("pause");

    return 0;
}

