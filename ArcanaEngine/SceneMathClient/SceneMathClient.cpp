// SceneMathClient.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "Quaternion.h"
#include "Vector3.h"
#include "AxisAlignedBoundingBox.h"
#include "Plane.h"

#include <iostream>

using namespace Arcana;

void printVector(const char* name, const Vector3f& vec)
{
	std::cout << name << " = <" << vec.x << ", " << vec.y << ", " << vec.z << ">" << std::endl;
}

void printQuaternion(const char* name, const Quaternionf& quat)
{
	std::cout << name << " = <" << quat.w << ", " << quat.x << ", " << quat.y << ", " << quat.z << ">" << std::endl;
}

int main()
{
	Quaternionf quat;
	quat.fromAxisAngle(Vector3f::unitX(), 90.0f);

	Vector3f vec = Vector3f(0.0, 0.0, 10.0);
	Vector3f first = quat * vec;
	Vector3f second = first * quat.conjugate();
	Vector3f vecR = quat.rotate(vec);

	printQuaternion("quat", quat);
	printQuaternion("quat.conjugate()", quat.conjugate());
	printVector("vec", vec);
	printVector("first", first);
	printVector("second", second);
	printVector("vecR", vecR);

	std::cout << std::endl << std::endl;

	AxisAlignedBoundingBoxf box1(0.0, 0.0, 0.0, 10.0, 10.0, 10.0);
	AxisAlignedBoundingBoxf box2(2.0, 3.0, 4.0, 3.0, 5.0, 7.0);

	std::cout << box1.intersects(box2) << std::endl;

	std::cout << std::endl << std::endl;
	Planef plane(Vector3f(0.0f, 2.0f, 3.0f), Vector3f(0.0f, 10.0f, -5.0f));
	std::cout << plane.distance(Vector3f(-10, -2, 2)) << std::endl;

	system("pause");

    return 0;
}

