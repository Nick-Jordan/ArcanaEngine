#include "stdafx.h"
#include "CppUnitTest.h"

#include "ArcanaMath.h"
#include "AxisAlignedBoundingBox.h"
#include "DynamicField.h"
#include "Ellipsoid.h"
#include "Matrix3.h"
#include "Matrix4.h"
#include "Plane.h"
#include "Rect.h"
#include "Sphere.h"
#include "StaticField.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"

using namespace Arcana;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Microsoft
{
	namespace VisualStudio
	{
		namespace CppUnitTestFramework
		{
			template<> static std::wstring ToString<Vector2f>(const Vector2f& t) {
				std::string s = "Vector2f(" + std::to_string(t.x) + ", " + std::to_string(t.y) + ")";
				return std::wstring(s.begin(), s.end());
			}
			template<> static std::wstring ToString<Vector2i>(const Vector2i& t) {
				std::string s = "Vector2i(" + std::to_string(t.x) + ", " + std::to_string(t.y) + ")";
				return std::wstring(s.begin(), s.end());
			}
			template<> static std::wstring ToString<Vector2d>(const Vector2d& t) {
				std::string s = "Vector2d(" + std::to_string(t.x) + ", " + std::to_string(t.y) + ")";
				return std::wstring(s.begin(), s.end());
			}

			template<> static std::wstring ToString<Vector3f>(const Vector3f& t) {
				std::string s = "Vector3f(" + std::to_string(t.x) + ", " + std::to_string(t.y) + ", " + std::to_string(t.z) + ")";
				return std::wstring(s.begin(), s.end());
			}
			template<> static std::wstring ToString<Vector3i>(const Vector3i& t) {
				std::string s = "Vector3i(" + std::to_string(t.x) + ", " + std::to_string(t.y) + ", " + std::to_string(t.z) + ")";
				return std::wstring(s.begin(), s.end());
			}
			template<> static std::wstring ToString<Vector3d>(const Vector3d& t) {
				std::string s = "Vector3d(" + std::to_string(t.x) + ", " + std::to_string(t.y) + ", " + std::to_string(t.z) + ")";
				return std::wstring(s.begin(), s.end());
			}

			template<> static std::wstring ToString<Vector4f>(const Vector4f& t) {
				std::string s = "Vector4f(" + std::to_string(t.x) + ", " + std::to_string(t.y) + ", " + std::to_string(t.z) + ", " + std::to_string(t.w) + ")";
				return std::wstring(s.begin(), s.end());
			}
			template<> static std::wstring ToString<Vector4i>(const Vector4i& t) {
				std::string s = "Vector4i(" + std::to_string(t.x) + ", " + std::to_string(t.y) + ", " + std::to_string(t.z) + ", " + std::to_string(t.w) + ")";
				return std::wstring(s.begin(), s.end());
			}
			template<> static std::wstring ToString<Vector4d>(const Vector4d& t) {
				std::string s = "Vector4d(" + std::to_string(t.x) + ", " + std::to_string(t.y) + ", " + std::to_string(t.z) + ", " + std::to_string(t.w) + ")";
				return std::wstring(s.begin(), s.end());
			}
		}
	}
}

namespace ResourceUnitTests
{
	
}