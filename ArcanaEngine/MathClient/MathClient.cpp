#include "stdafx.h"
#include <stdlib.h>

#include <iostream> 

#include "ArcanaMath.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix3.h"
#include "Matrix4.h"
#include "Quaternion.h"
#include "Rect.h"
#include "Equation.h"

using namespace Arcana;

int main()
{
	std::cout << "Arcana Math Library Test" << std::endl;

	//---------------------------------------------------Math---------------------------------------------------//

	std::cout << std::endl;
	
	std::cout << "Math:" << std::endl;
	std::cout << "PI = " << Math::PI << std::endl;
	std::cout << "HALF_PI = " << Math::HALF_PI << std::endl;
	std::cout << "QUARTER_PI = " << Math::QUARTER_PI << std::endl;
	std::cout << "TWO_PI = " << Math::TWO_PI << std::endl;
	std::cout << "EPSILON = " << Math::EPSILON << std::endl;
	std::cout << "DEG_TO_RAD = " << Math::DEG_TO_RAD << std::endl;
	std::cout << "RAD_TO_DEG = " << Math::RAD_TO_DEG << std::endl;

	std::cout << "clamp(-1.4, 0.0, 1.0) = " << Math::clamp(-1.4, 0.0, 1.0) << std::endl;

	std::cout << "range(50.0, 0.0, 100.0, 0.0, 1.0) = " << Math::range(50.0, 0.0, 100.0, 0.0, 1.0) << std::endl;

	std::cout << "lerp(0.0, 10.0, 0.3) = " << Math::lerp(0.0, 10.0, 0.3) << std::endl;

	std::cout << "bilerp(0.0, 1.0, 1.0, 0.5, 0.5) = " << Math::bilerp(0.0, 1.0, 1.0, 0.5, 0.5, 0.3) << std::endl;

	std::cout << "sign(-1.4) = " << Math::sign(-1.4) << std::endl;

	std::cout << "square(4.0) = " << Math::square(4.0) << std::endl;

	double intpart = 0.0;
	std::cout << "fract(4.302943) = " << Math::fract(4.302943, &intpart) << std::endl;

	double rho = 0.0;
	double phi = 0.0;
	double theta = 0.0;
	Math::cartesianToSpherical(0.0, 1.0, 0.0, rho, phi, theta);
	std::cout << "cartesianToSpherical(0.707106781, 0.707106781, 0.707106781, rho, phi, theta) = " << rho << ", " << phi << ", " << theta << std::endl;

	double x = 0.0;
	double y = 0.0;
	double z = 0.0;
	Math::sphericalToCartesian(1.0, Math::HALF_PI, Math::HALF_PI, x, y, z);
	std::cout << "sphericalToCartesian(1.0, HALF_PI, HALF_PI, x, y, z) = " << x << ", " << y << ", " << z << std::endl;

	std::cout << "square(4.0) = " << Math::square(4.0) << std::endl;

	std::cout << "closeEnough(0.99999999, 1.0) = " << Math::closeEnough(0.99999999, 1.0) << std::endl;

	std::cout << "degreesToRadians(90.0) = " << Math::degreesToRadians(90.0) << std::endl;


	//---------------------------------------------------Vector2---------------------------------------------------//

	std::cout << std::endl;

	std::cout << "Vector2:" << std::endl;

	Vector2d vector1 = Vector2d(0.5, 4.0);
	Vector2d vector2 = Vector2d(2.5, 3.2);

	std::cout << "vector1 = <" << vector1.x << ", " << vector1.y << ">" << std::endl;
	std::cout << "vector2 = <" << vector2.x << ", " << vector2.y << ">" << std::endl;

	std::cout << "distance(vector1, vector2) = " << Vector2d::distance(vector1, vector2) << std::endl;

	std::cout << "distanceSq(vector1, vector2) = " << Vector2d::distanceSq(vector1, vector2) << std::endl;

	std::cout << "dot(vector1, vector2) = " << Vector2d::dot(vector1, vector2) << std::endl;

	Vector2d lerp = Vector2d::lerp(vector1, vector2, 0.4);
	std::cout << "lerp(vector1, vector2, 0.4) = <" << lerp.x << ", " << lerp.y << ">" << std::endl;

	Vector2d proj = Vector2d::proj(vector1, vector2);
	std::cout << "proj(vector1, vector2) = <" << proj.x << ", " << proj.y << ">" << std::endl;

	Vector2d perp = Vector2d::perp(vector1, vector2);
	std::cout << "perp(vector1, vector2) = <" << perp.x << ", " << perp.y << ">" << std::endl;

	Vector2d reflect = Vector2d::reflect(vector1, vector2);
	std::cout << "reflect(vector1, vector2) = <" << reflect.x << ", " << reflect.y << ">" << std::endl;

	Vector2d normalize = Vector2d::normalize(vector1);
	std::cout << "normalize(vector1) = <" << normalize.x << ", " << normalize.y << ">" << std::endl;

	std::cout << "|vector1| = " << vector1.magnitude() << std::endl;

	std::cout << "|vector1|^2 = " << vector1.magnitudeSq() << std::endl;

	Vector2d inverse = vector1.inverse();
	std::cout << "-vector1 = <" << inverse.x << ", " << inverse.y << ">" << std::endl;

	vector1.scale(2.0);
	std::cout << "vector1 * 2.0 = <" << vector1.x << ", " << vector1.y << ">" << std::endl;

	Vector2i cast = vector2.cast<int>();
	std::cout << "vector2.cast<int>() = <" << cast.x << ", " << cast.y << ">" << std::endl;


	//---------------------------------------------------Vector3---------------------------------------------------//

	std::cout << std::endl;

	std::cout << "Vector3:" << std::endl;

	Vector3d vector3 = Vector3d(0.5, 4.0, 2.0);
	Vector3d vector4 = Vector3d(2.5, 3.2, 5.7);

	std::cout << "vector3 = <" << vector3.x << ", " << vector3.y << ", " << vector3.z << ">" << std::endl;
	std::cout << "vector4 = <" << vector4.x << ", " << vector4.y << ", " << vector4.z << ">" << std::endl;

	Vector3d cross2 = Vector3d::cross(vector3, vector4);
	std::cout << "cross(vector3, vector4) = <" << cross2.x << ", " << cross2.y << ", " << cross2.z << ">" << std::endl;

	std::cout << "distance(vector3, vector4) = " << Vector3d::distance(vector3, vector4) << std::endl;

	std::cout << "distanceSq(vector3, vector4) = " << Vector3d::distanceSq(vector3, vector4) << std::endl;

	std::cout << "dot(vector3, vector4) = " << Vector3d::dot(vector3, vector4) << std::endl;

	Vector3d lerp2 = Vector3d::lerp(vector3, vector4, 0.4);
	std::cout << "lerp(vector3, vector4, 0.4) = <" << lerp2.x << ", " << lerp2.y << ", " << lerp2.z << ">" << std::endl;

	Vector3d proj2 = Vector3d::proj(vector3, vector4);
	std::cout << "proj(vector3, vector4) = <" << proj2.x << ", " << proj2.y << ", " << proj2.z << ">" << std::endl;

	Vector3d perp2 = Vector3d::perp(vector3, vector4);
	std::cout << "perp(vector3, vector4) = <" << perp2.x << ", " << perp2.y << ", " << perp2.z << ">" << std::endl;

	Vector3d reflect2 = Vector3d::reflect(vector3, vector4);
	std::cout << "reflect(vector3, vector4) = <" << reflect2.x << ", " << reflect2.y << ", " << reflect2.z << ">" << std::endl;

	Vector3d normalize2 = Vector3d::normalize(vector3);
	std::cout << "normalize(vector3) = <" << normalize2.x << ", " << normalize2.y << ", " << normalize2.z << ">" << std::endl;

	std::cout << "|vector3| = " << vector3.magnitude() << std::endl;

	std::cout << "|vector3|^2 = " << vector3.magnitudeSq() << std::endl;

	Vector3d inverse2 = vector3.inverse();
	std::cout << "-vector3 = <" << inverse2.x << ", " << inverse2.y << ", " << inverse2.z << ">" << std::endl;

	vector3.scale(2.0);
	std::cout << "vector3 * 2.0 = <" << vector3.x << ", " << vector3.y << ", " << vector3.z << ">" << std::endl;

	Vector3i cast2 = vector4.cast<int>();
	std::cout << "vector4.cast<int>() = <" << cast2.x << ", " << cast2.y << ", " << cast2.z << ">" << std::endl;


	//---------------------------------------------------Vector4---------------------------------------------------//

	std::cout << std::endl;

	std::cout << "Vector4:" << std::endl;

	Vector4d vector5 = Vector4d(0.5, 4.0, 2.0, 1.5);
	Vector4d vector6 = Vector4d(2.5, 3.2, 5.7, 6.3);

	std::cout << "vector5 = <" << vector5.x << ", " << vector5.y << ", " << vector5.z << ", " << vector5.w << ">" << std::endl;
	std::cout << "vector6 = <" << vector6.x << ", " << vector6.y << ", " << vector6.z << ", " << vector6.w << ">" << std::endl;

	std::cout << "distance(vector5, vector6) = " << Vector4d::distance(vector5, vector6) << std::endl;

	std::cout << "distanceSq(vector5, vector6) = " << Vector4d::distanceSq(vector5, vector6) << std::endl;

	std::cout << "dot(vector5, vector6) = " << Vector4d::dot(vector5, vector6) << std::endl;

	Vector4d lerp3 = Vector4d::lerp(vector5, vector6, 0.4);
	std::cout << "lerp(vector5, vector6, 0.4) = <" << lerp3.x << ", " << lerp3.y << ", " << lerp3.z << ", " << lerp3.w << ">" << std::endl;

	Vector4d proj3 = Vector4d::proj(vector5, vector6);
	std::cout << "proj(vector5, vector6) = <" << proj3.x << ", " << proj3.y << ", " << proj3.z << ", " << proj3.w << ">" << std::endl;

	Vector4d perp3 = Vector4d::perp(vector5, vector6);
	std::cout << "perp(vector5, vector6) = <" << perp3.x << ", " << perp3.y << ", " << perp3.z << ", " << perp3.w << ">" << std::endl;

	Vector4d reflect3 = Vector4d::reflect(vector5, vector6);
	std::cout << "reflect(vector5, vector6) = <" << reflect3.x << ", " << reflect3.y << ", " << reflect3.z << ", " << reflect3.w << ">" << std::endl;

	Vector4d normalize3 = Vector4d::normalize(vector5);
	std::cout << "normalize(vector5) = <" << normalize3.x << ", " << normalize3.y << ", " << normalize3.z << ", " << normalize3.w << ">" << std::endl;

	std::cout << "|vector5| = " << vector5.magnitude() << std::endl;

	std::cout << "|vector5|^2 = " << vector5.magnitudeSq() << std::endl;

	Vector4d inverse3 = vector5.inverse();
	std::cout << "-vector5 = <" << inverse3.x << ", " << inverse3.y << ", " << inverse3.z << ", " << inverse3.w << ">" << std::endl;

	vector5.scale(2.0);
	std::cout << "vector5 * 2.0 = <" << vector5.x << ", " << vector5.y << ", " << vector5.z << ", " << vector5.w << ">" << std::endl;

	Vector4i cast3 = vector6.cast<int>();
	std::cout << "vector6.cast<int>() = <" << cast3.x << ", " << cast3.y << ", " << cast3.z << ", " << cast3.w << ">" << std::endl;


	//---------------------------------------------------Matrix3---------------------------------------------------//

	std::cout << std::endl;

	std::cout << "Matrix3:" << std::endl;

	Matrix3d matrix = Matrix3d(0.4, 3.5, 6.3, 3.0, 2.2, 5.3, 0.0, 7.5, 1.0);

	for (int j = 0; j < 3; j++)
	{
		for (int i = 0; i < 3; i++)
		{
			std::cout << matrix.at(i, j) << " ";
		}
		std::cout << std::endl;

	}

	std::cout << matrix.determinant() << std::endl;

	Matrix3d inverseMatrix = matrix.inverse();

	for (int j = 0; j < 3; j++)
	{
		for (int i = 0; i < 3; i++)
		{
			std::cout << inverseMatrix.at(i, j) << " ";
		}
		std::cout << std::endl;
	}

	Matrix3d identity3 = Matrix3d::IDENTITY;

	Vector3d forward = identity3.getForwardVector();
	std::cout << "Forward: " << forward.x << ", " << forward.y << ", " << forward.z << std::endl;
	Vector3d left = identity3.getLeftVector();
	std::cout << "Left: " << left.x << ", " << left.y << ", " << left.z << std::endl;
	Vector3d up = identity3.getUpVector();
	std::cout << "Up: " << up.x << ", " << up.y << ", " << up.z << std::endl;

	//---------------------------------------------------Matrix4---------------------------------------------------//

	std::cout << std::endl;

	std::cout << "Matrix4:" << std::endl;

	Matrix4d matrix4 = Matrix4d(1, 1, 4, 5, 3, 3, 3, 2, 5, 1, 9, 0, 9, 7, 7, 9);

	for (int j = 0; j < 4; j++)
	{
		for (int i = 0; i < 4; i++)
		{
			std::cout << matrix4.at(i, j) << " ";
		}
		std::cout << std::endl;

	}

	std::cout << matrix4.determinant() << std::endl;

	Matrix4d inverseMatrix4 = matrix4.inverse();

	for (int j = 0; j < 4; j++)
	{
		for (int i = 0; i < 4; i++)
		{
			std::cout << inverseMatrix4.at(i, j) << " ";
		}
		std::cout << std::endl;
	}

	std::cout << std::endl;

	Vector3d rotateVector = Vector3d(2, 0, 0);

	Matrix4d rotateMatrix4 = Matrix4d::createRotation(Vector3d::unitZ(), Math::degreesToRadians(90.0));

	rotateVector = rotateMatrix4 * rotateVector;

	std::cout << "Rotate Vector: " << rotateVector.x << ", " << rotateVector.y << ", " << rotateVector.z << std::endl;

	for (int j = 0; j < 4; j++)
	{
		for (int i = 0; i < 4; i++)
		{
			std::cout << rotateMatrix4.at(i, j) << " ";
		}
		std::cout << std::endl;
	}

	Matrix4d identity4 = Matrix4d::IDENTITY;

	Vector3d forward4 = identity4.getForwardVector();
	std::cout << "Forward: " << forward4.x << ", " << forward4.y << ", " << forward4.z << std::endl;
	Vector3d left4 = identity4.getLeftVector();
	std::cout << "Left: " << left4.x << ", " << left4.y << ", " << left4.z << std::endl;
	Vector3d up4 = identity4.getUpVector();
	std::cout << "Up: " << up4.x << ", " << up4.y << ", " << up4.z << std::endl;

	const float* array = Matrix4f::IDENTITY.getValuePointer();

	for (int i = 0; i < 16; i++)
	{
		std::cout << array[i] << ", ";
	}

	//---------------------------------------------------Quaternion---------------------------------------------------//

	std::cout << std::endl;

	std::cout << "Quaternion:" << std::endl;

	Quaternionf quaternion = Quaternionf::IDENTITY;

	//---------------------------------------------------Rect---------------------------------------------------//

	std::cout << std::endl;

	std::cout << "Rect:" << std::endl;

	Recti rect1 = Recti(-1, -1, 20, 20);
	Recti rect2 = Recti(-2, -2, 4, 4);
	
	rect1.intersect(rect2);

	std::cout << "rect1: " << rect1.getLeft() << ", " << rect1.getTop() << ", " << rect1.getSize().x << ", " << rect1.getSize().y << std::endl;

	std::cout << rect1.intersects(rect2) << std::endl;

	//---------------------------------------------------Equation---------------------------------------------------//

	std::cout << std::endl;

	std::cout << "Equation:" << std::endl;

	std::vector<std::string> v = { "t" };
	Equation equation = Equation(v, "x=t+2");

	system("pause");

    return 0;
}

