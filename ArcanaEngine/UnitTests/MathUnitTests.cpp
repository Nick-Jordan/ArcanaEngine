#include "stdafx.h"
#include "CppUnitTest.h"

#include "ArcanaMath.h"
#include "AxisAlignedBoundingBox.h"
#include "DynamicField.h"
#include "Ellipsoid.h"
#include "Matrix3.h"
#include "Matrix4.h"
#include "OrientedBoundingBox.h"
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

			template<> static std::wstring ToString<Rectf>(const Rectf& t) {
				std::string s = "Rectf(" + std::to_string(t.getLeft()) + ", " + std::to_string(t.getTop()) + ", " + std::to_string(t.getSize().x) + ", " + std::to_string(t.getSize().y) + ")";
				return std::wstring(s.begin(), s.end());
			}
			template<> static std::wstring ToString<Recti>(const Recti& t) {
				std::string s = "Recti(" + std::to_string(t.getLeft()) + ", " + std::to_string(t.getTop()) + ", " + std::to_string(t.getSize().x) + ", " + std::to_string(t.getSize().y) + ")";
				return std::wstring(s.begin(), s.end());
			}
			template<> static std::wstring ToString<Rectd>(const Rectd& t) {
				std::string s = "Rectd(" + std::to_string(t.getLeft()) + ", " + std::to_string(t.getTop()) + ", " + std::to_string(t.getSize().x) + ", " + std::to_string(t.getSize().y) + ")";
				return std::wstring(s.begin(), s.end());
			}
		}
	}
}

namespace MathUnitTests
{		
	TEST_CLASS(ArcanaMathUnitTests)
	{
	public:
		
		TEST_METHOD(Clamp)
		{
			float x = 1.5f;
			Assert::AreEqual(1.5f, Math::clamp(x, 0.0f, 2.0f));
			Assert::AreEqual(1.0f, Math::clamp(x, 0.0f, 1.0f));
		}
		
		TEST_METHOD(RangeConversion)
		{
			float x = 1.5f;
			Assert::AreEqual(0.5f, Math::range(x, 0.0f, 3.0f, 0.0f, 1.0f));
		}

		TEST_METHOD(LinearInterpolation)
		{
			Assert::AreEqual(0.4, Math::lerp(0.0, 2.0, 0.2));
		}

		TEST_METHOD(BilinearInterpolation)
		{
			Assert::AreEqual(0.5, Math::bilerp(0.0, 1.0, 0.0, 1.0, 0.5, 0.5));
		}

		TEST_METHOD(Sign)
		{
			Assert::AreEqual(-1, Math::sign(-4.5));
			Assert::AreEqual(0, Math::sign(0.0f));
			Assert::AreEqual(1, Math::sign(4.5));
		}

		TEST_METHOD(Square)
		{
			Assert::AreEqual(16, Math::square(4));
		}

		TEST_METHOD(Minimum)
		{
			Assert::AreEqual(4.5f, Math::min(4.5f, 6.7f));
		}

		TEST_METHOD(Maximum)
		{
			Assert::AreEqual(6.7f, Math::max(4.5f, 6.7f));
		}

		TEST_METHOD(AbsoluteValue)
		{
			Assert::AreEqual(4.5f, Math::abs(-4.5f));
		}

		TEST_METHOD(Fract)
		{
			double num = 5.234;
			double i;
			Math::fract(num, &i);
			Assert::AreEqual(0.234, Math::fract(num, &i), Math::EPSILON);
			Assert::AreEqual(5.0, i, Math::EPSILON);
		}

		TEST_METHOD(CartesianToSpherical)
		{
			double x = 2.0;
			double y = 5.0;
			double z = 3.0;

			double rho;
			double phi;
			double theta;

			Math::cartesianToSpherical(x, y, z, rho, phi, theta);

			Assert::AreEqual(6.164414002969, rho, Math::EPSILON);
			Assert::AreEqual(1.1902899496825, theta, Math::EPSILON);
			Assert::AreEqual(1.0625290806236, phi, Math::EPSILON);
		}

		TEST_METHOD(SphericalToCartesian)
		{
			double rho = 4.0;
			double phi = 1.2;
			double theta = 2.3;

			double x;
			double y;
			double z;

			Math::sphericalToCartesian(rho, phi, theta, x, y, z);

			Assert::AreEqual(-2.483981176, x);
			Assert::AreEqual(2.780105617, y);
			Assert::AreEqual(1.449431018, z);
		}

		TEST_METHOD(CloseEnough)
		{
			Assert::AreEqual(true, Math::closeEnough(1.0, 1.0000001));
		}

		TEST_METHOD(NotCloseEnough)
		{
			Assert::AreEqual(false, Math::closeEnough(1.0, 1.0001));
		}

		TEST_METHOD(DegreesToRadians)
		{
			Assert::AreEqual(0.750492, Math::degreesToRadians(43.0), Math::EPSILON);
		}

		TEST_METHOD(RadiansToDegrees)
		{
			Assert::AreEqual(28.6478898, Math::radiansToDegrees(0.5), Math::EPSILON);
		}


		//smooth and smoothstep...
	};

	TEST_CLASS(AxisAlignedBoundingBoxUnitTests)
	{
	public:

		TEST_METHOD(Accessors)
		{
			AxisAlignedBoundingBoxf box(0.0f, 1.0f, 2.0f, 10.0f, 11.0f, 12.0f);

			Assert::AreEqual(Vector3f(0.0f, 1.0f, 2.0f), box.getMin());
			Assert::AreEqual(Vector3f(10.0f, 11.0f, 12.0f), box.getMax());
		}

		TEST_METHOD(GetSize)
		{
			AxisAlignedBoundingBoxf box(0.0f, 1.0f, 2.0f, 10.0f, 11.0f, 12.0f);

			Assert::AreEqual(Vector3f(10.0f, 10.0f, 10.0f), box.getSize());
			Assert::AreEqual(10.0f, box.getWidth());
			Assert::AreEqual(10.0f, box.getHeight());
			Assert::AreEqual(10.0f, box.getDepth());
		}

		TEST_METHOD(ContainsPoint)
		{
			AxisAlignedBoundingBoxf box(0.0f, 1.0f, 2.0f, 10.0f, 11.0f, 12.0f);

			Assert::AreEqual(true, box.contains(5.0f, 5.0f, 5.0f));
			Assert::AreEqual(false, box.contains(20.0f, 20.0f, 20.0f));
		}

		TEST_METHOD(ContainsAABB)
		{
			AxisAlignedBoundingBoxf box(0.0f, 1.0f, 2.0f, 10.0f, 11.0f, 12.0f);
			AxisAlignedBoundingBoxf inside(5.0f, 5.0f, 5.0f, 6.0f, 7.0f, 8.0f);
			AxisAlignedBoundingBoxf outside(20.0f, 21.0f, 22.0f, 30.0f, 31.0f, 32.0f);
			AxisAlignedBoundingBoxf partiallyInside(5.0f, 5.0f, 5.0f, 12.0f, 13.0f, 14.0f);

			Assert::AreEqual(true, box.contains(inside));
			Assert::AreEqual(false, box.contains(outside));
			Assert::AreEqual(false, box.contains(partiallyInside));
		}

		TEST_METHOD(IntersectsAABB)
		{
			AxisAlignedBoundingBoxf box(0.0f, 1.0f, 2.0f, 10.0f, 11.0f, 12.0f);
			AxisAlignedBoundingBoxf inside(5.0f, 5.0f, 5.0f, 6.0f, 7.0f, 8.0f);
			AxisAlignedBoundingBoxf outside(20.0f, 21.0f, 22.0f, 30.0f, 31.0f, 32.0f);
			AxisAlignedBoundingBoxf partiallyInside(5.0f, 5.0f, 5.0f, 12.0f, 13.0f, 14.0f);

			Assert::AreEqual(true, box.intersects(inside));
			Assert::AreEqual(false, box.intersects(outside));
			Assert::AreEqual(true, box.intersects(partiallyInside));
		}

		TEST_METHOD(IsEmpty)
		{
			AxisAlignedBoundingBoxf box1(0.0f, 1.0f, 2.0f, 10.0f, 11.0f, 12.0f);
			AxisAlignedBoundingBoxf box2(10.0f, 2.0f, 3.0f, 10.0f, 2.0f, 3.0f);

			Assert::AreEqual(false, box1.isEmpty());
			Assert::AreEqual(true, box2.isEmpty());
		}
	};

	TEST_CLASS(CurveUnitTests)
	{
	public:

	};

	double func(double x, double y, double z)
	{
		return x + y + z;
	}

	TEST_CLASS(DynamicFieldUnitTests)
	{
	public:

		TEST_METHOD(Get)
		{
			DynamicScalarField field(&func);

			Assert::AreEqual(6.0, field.get(1.0, 2.0, 3.0));
			Assert::AreEqual(5.9, field.get(1.2, 1.5, 3.2));
		}
	};

	TEST_CLASS(EllipsoidUnitTests)
	{
	public:

		TEST_METHOD(Accessors)
		{
			Ellipsoidf ellipsoid(Vector3f(1.0f, 1.0f, 1.0f), Vector3f(2.0f, 3.0f, 4.0f));

			Assert::AreEqual(Vector3f::one(), ellipsoid.getCenter());
			Assert::AreEqual(Vector3f(2.0f, 3.0f, 4.0f), ellipsoid.getAxes());
		}

		TEST_METHOD(ContainsPoint)
		{
			Ellipsoidf ellipsoid(Vector3f(1.0f, 1.0f, 1.0f), Vector3f(2.0f, 2.0f, 2.0f));

			Assert::AreEqual(true, ellipsoid.contains(1.5f, 1.5f, 1.5f));
			Assert::AreEqual(false, ellipsoid.contains(3.0f, 4.0f, 3.0f));
		}
	};

	/*TEST_CLASS(EquationUnitTests)
	{
	public:

	};??????????????*/

	TEST_CLASS(Matrix3UnitTests)
	{
	public:

	};

	TEST_CLASS(Matrix4UnitTests)
	{
	public:

	};

	TEST_CLASS(OrientedBoundingBoxUnitTests)
	{
	public:

		TEST_METHOD(Accessors)
		{
			Quaternionf rotation;
			OrientedBoundingBoxf box(0.0f, 1.0f, 2.0f, 10.0f, 11.0f, 12.0f, rotation);

			Assert::AreEqual(Vector3f(0.0f, 1.0f, 2.0f), box.getMin());
			Assert::AreEqual(Vector3f(10.0f, 11.0f, 12.0f), box.getMax());
			//Assert::AreEqual(transformation, box.getTransformation());
		}

		TEST_METHOD(GetSize)
		{
			Quaternionf rotation;
			OrientedBoundingBoxf box(0.0f, 1.0f, 2.0f, 10.0f, 11.0f, 12.0f, rotation);

			Assert::AreEqual(Vector3f(10.0f, 10.0f, 10.0f), box.getSize());
			Assert::AreEqual(10.0f, box.getWidth());
			Assert::AreEqual(10.0f, box.getHeight());
			Assert::AreEqual(10.0f, box.getDepth());
		}

		TEST_METHOD(ContainsPoint)
		{
			Quaternionf rotation;
			rotation.fromAxisAngle(Vector3f::unitZ(), 45.0f);
			OrientedBoundingBoxf box(0.0f, 1.0f, 2.0f, 10.0f, 11.0f, 12.0f, rotation);

			Assert::AreEqual(true, box.contains(5.0f, 5.0f, 5.0f));
			Assert::AreEqual(false, box.contains(20.0f, 20.0f, 20.0f));
		}

		TEST_METHOD(ContainsAABB)
		{
			Quaternionf rotation;
			OrientedBoundingBoxf box(0.0f, 1.0f, 2.0f, 10.0f, 11.0f, 12.0f, rotation);
			OrientedBoundingBoxf inside(5.0f, 5.0f, 5.0f, 6.0f, 7.0f, 8.0f, rotation);
			OrientedBoundingBoxf outside(20.0f, 21.0f, 22.0f, 30.0f, 31.0f, 32.0f, rotation);
			OrientedBoundingBoxf partiallyInside(5.0f, 5.0f, 5.0f, 12.0f, 13.0f, 14.0f, rotation);

			//Assert::AreEqual(true, box.contains(inside));
			Assert::AreEqual(false, box.contains(outside));
			Assert::AreEqual(false, box.contains(partiallyInside));
		}

		TEST_METHOD(IntersectsOBB)
		{
			Quaternionf rotation;
			OrientedBoundingBoxf box(0.0f, 1.0f, 2.0f, 10.0f, 11.0f, 12.0f, rotation);
			OrientedBoundingBoxf inside(5.0f, 5.0f, 5.0f, 6.0f, 7.0f, 8.0f, rotation);
			OrientedBoundingBoxf outside(20.0f, 21.0f, 22.0f, 30.0f, 31.0f, 32.0f, rotation);
			OrientedBoundingBoxf partiallyInside(5.0f, 5.0f, 5.0f, 12.0f, 13.0f, 14.0f, rotation);

			//Assert::AreEqual(true, box.intersects(inside));
			Assert::AreEqual(false, box.intersects(outside));
			//Assert::AreEqual(true, box.intersects(partiallyInside));
		}

		TEST_METHOD(IsEmpty)
		{
			Quaternionf rotation;
			OrientedBoundingBoxf box1(0.0f, 1.0f, 2.0f, 10.0f, 11.0f, 12.0f, rotation);
			OrientedBoundingBoxf box2(10.0f, 2.0f, 3.0f, 10.0f, 2.0f, 3.0f, rotation);

			Assert::AreEqual(false, box1.isEmpty());
			//Assert::AreEqual(true, box2.isEmpty());
		}
	};

	TEST_CLASS(PlaneUnitTests)
	{
	public:

		TEST_METHOD(Accessors)
		{
			Planef plane1(Vector3f(0.0f, 1.0f, 0.0f), Vector3f(1.0f, 0.0f, 0.0f));
			Planef plane2(Vector3f(0.0f, 1.0f, 0.0f), 0.0f);
			Planef plane3(0.0f, Vector3f(1.0f, 0.0f, 0.0f));

			Assert::AreEqual(0.0f, plane1.getDistance());
			Assert::AreEqual(Vector3f(1.0f, 0.0f, 0.0f), plane1.getNormal());

			//Assert::AreEqual(0.0f, plane2.getDistance());
			//Assert::AreEqual(Vector3f(1.0f, 0.0f, 0.0f), plane2.getNormal());

			Assert::AreEqual(0.0f, plane3.getDistance());
			Assert::AreEqual(Vector3f(1.0f, 0.0f, 0.0f), plane3.getNormal());
		}

		TEST_METHOD(IsFacing)
		{
			Planef plane1(Vector3f(0.0f, 1.0f, 0.0f), Vector3f(1.0f, 0.0f, 0.0f));

			Assert::AreEqual(true, plane1.isFacing(Vector3f(-0.5, 0.5, 0.0)));
			Assert::AreEqual(false, plane1.isFacing(Vector3f(0.5, 0.5, 0.0)));
		}

		TEST_METHOD(Distance)
		{
			Planef plane1(Vector3f(0.0f, 1.0f, 0.0f), Vector3f(1.0f, 0.0f, 0.0f));

			Assert::AreEqual(2.0f, plane1.distance(Vector3f(2.0f, 3.0f, 4.0f)));
		}

		TEST_METHOD(SignedDistance)
		{
			Planef plane1(Vector3f(0.0f, 1.0f, 0.0f), Vector3f(1.0f, 0.0f, 0.0f));

			Assert::AreEqual(2.0f, plane1.signedDistance(Vector3f(2.0f, 3.0f, 4.0f)));
		}

		TEST_METHOD(IsParallel)
		{
			Planef plane1(Vector3f(0.0f, 1.0f, 0.0f), Vector3f(1.0f, 0.0f, 0.0f));
			Planef plane2(Vector3f(0.0f, 1.0f, 4.0f), Vector3f(4.0f, 0.0f, 0.0f));
			Planef plane3(Vector3f(2.0f, 1.0f, 0.0f), Vector3f(0.5f, 1.0f, 0.0f));

			Assert::AreEqual(true, plane1.isParallel(plane2));
			Assert::AreEqual(false, plane1.isParallel(plane3));
		}

		TEST_METHOD(Normalize)
		{
			Planef plane1(Vector3f(0.0f, 1.0f, 0.0f), Vector3f(1.0f, 1.0f, 0.0f));
			plane1.normalize();

			Assert::AreEqual(Vector3f(0.707106781f, 0.707106781f, 0.0f), plane1.getNormal());
			Assert::AreEqual(-0.707106781f, plane1.getDistance());
		}

		//transform by matrix test
	};

	TEST_CLASS(QuaternionUnitTests)
	{
	public:

	};

	/*TEST_CLASS(RandomUnitTests)
	{
	public:

	};??????????????*/

	TEST_CLASS(RectUnitTests)
	{
	public:

		TEST_METHOD(Accessor)
		{
			Rectf rect(1.0f, 2.0f, 3.0f, 3.0f);

			Assert::AreEqual(1.0f, rect.getLeft());
			Assert::AreEqual(2.0f, rect.getTop());
			Assert::AreEqual(3.0f, rect.getSize().x);
			Assert::AreEqual(3.0f, rect.getSize().y);
			Assert::AreEqual(4.0f, rect.getRight());
			Assert::AreEqual(5.0f, rect.getBottom());
		}

		TEST_METHOD(ContainsPoint)
		{
			Rectf rect(1.0f, 2.0f, 3.0f, 3.0f);

			Assert::AreEqual(true, rect.contains(1.5f, 2.5f));
			Assert::AreEqual(false, rect.contains(0.0f, 0.0f));
		}

		TEST_METHOD(ContainsRect)
		{
			Rectf rect(1.0f, 2.0f, 3.0f, 3.0f);
			Rectf inside(1.5f, 2.5f, 1.0f, 0.2f);
			Rectf outside(4.0f, 4.0f, 1.0f, 1.0f);
			Rectf partiallyInside(1.5f, 2.5f, 4.0f, 4.0f);

			Assert::AreEqual(true, rect.contains(inside));
			Assert::AreEqual(false, rect.contains(outside));
			Assert::AreEqual(false, rect.contains(partiallyInside));
		}

		TEST_METHOD(Resize)
		{
			Rectf rect(1.0f, 2.0f, 3.0f, 3.0f);
			rect.resize(1.0f, 1.0f);
			Assert::AreEqual(Rectf(0.0f, 1.0f, 4.0f, 4.0f), rect);
		}

		TEST_METHOD(Offset)
		{
			Rectf rect(1.0f, 2.0f, 3.0f, 3.0f);
			rect.offset(1.0f, 1.0f);
			Assert::AreEqual(Rectf(2.0f, 3.0f, 3.0f, 3.0f), rect);
		}

		TEST_METHOD(Intersect)
		{
			Rectf rect(1.0f, 2.0f, 3.0f, 3.0f);
			Rectf reset(1.0f, 2.0f, 3.0f, 3.0f);
			Rectf inside(1.5f, 2.5f, 1.0f, 0.2f);
			Rectf outside(4.0f, 4.0f, 1.0f, 1.0f);
			Rectf partiallyInside(1.5f, 2.5f, 4.0f, 4.0f);

			rect.intersect(inside);
			Assert::AreEqual(inside, rect);
			rect = reset;

			rect.intersect(outside);
			Assert::AreEqual(true, rect.isEmpty());
			rect = reset;

			rect.intersect(partiallyInside);
			Assert::AreEqual(Rectf(1.5f, 2.5f, 2.5f, 2.5f), rect);
		}

		TEST_METHOD(Intersects)
		{
			Rectf rect(1.0f, 2.0f, 3.0f, 3.0f);
			Rectf inside(1.5f, 2.5f, 1.0f, 0.2f);
			Rectf outside(4.0f, 4.0f, 1.0f, 1.0f);
			Rectf partiallyInside(1.5f, 2.5f, 4.0f, 4.0f);

			Assert::AreEqual(true, rect.intersects(inside));
			Assert::AreEqual(false, rect.intersects(outside));
			Assert::AreEqual(true, rect.intersects(partiallyInside));
		}

		TEST_METHOD(IsEmpty)
		{
			Rectf rect1(1.0f, 2.0f, 0.0f, 1.0f);
			Rectf rect2(1.5f, 2.5f, 1.0f, 0.2f);
			
			Assert::AreEqual(true, rect1.isEmpty());
			Assert::AreEqual(false, rect2.isEmpty());
		}

		TEST_METHOD(Union)
		{
			Rectf rect(1.0f, 2.0f, 3.0f, 3.0f);
			Rectf reset(1.0f, 2.0f, 3.0f, 3.0f);
			Rectf inside(1.5f, 2.5f, 1.0f, 0.2f);
			Rectf outside(4.0f, 4.0f, 1.0f, 1.0f);
			Rectf partiallyInside(1.5f, 2.5f, 4.0f, 4.0f);

			rect.rectUnion(inside);
			Assert::AreEqual(reset, rect);
			rect = reset;

			rect.rectUnion(outside);
			Assert::AreEqual(Rectf(1.0f, 2.0f, 4.0f, 3.0f), rect);
			rect = reset;

			rect.rectUnion(partiallyInside);
			Assert::AreEqual(Rectf(1.0f, 2.0f, 4.5f, 4.5f), rect);
		}

		TEST_METHOD(Cast)
		{
			Rectf rect(1.3f, 2.2f, 3.4f, 3.7f);
			
			Assert::AreEqual(Recti(1, 2, 3, 3), rect.cast<int>());
		}
	};

	TEST_CLASS(SphereUnitTests)
	{
	public:

		TEST_METHOD(Accessors)
		{
			Spheref sphere(Vector3f(1.0f, 1.0f, 1.0f), 2.0f);

			Assert::AreEqual(Vector3f::one(), sphere.getCenter());
			Assert::AreEqual(2.0f, sphere.getRadius());
		}

		TEST_METHOD(ContainsPoint)
		{
			Ellipsoidf ellipsoid(Vector3f(1.0f, 1.0f, 1.0f), Vector3f(2.0f, 2.0f, 2.0f));

			Assert::AreEqual(true, ellipsoid.contains(1.5f, 1.5f, 1.5f));
			Assert::AreEqual(false, ellipsoid.contains(3.0f, 3.0f, 3.0f));
		}
	};

	TEST_CLASS(StaticFieldUnitTests)
	{
	public:

		TEST_METHOD(Accessor)
		{
			StaticScalarField field(10, 10, 10, 3.0);
			field.setBoundaryValue(1.0);

			Assert::AreEqual(1.0, field.getBoundaryValue());
			Assert::AreEqual((unsigned int)10, field.getWidth());
			Assert::AreEqual((unsigned int)10, field.getHeight());
			Assert::AreEqual((unsigned int)10, field.getDepth());
		}

		TEST_METHOD(GetValue)
		{
			StaticScalarField field(10, 10, 10, 3.0);
			field.setValue(4, 4, 4, 2.0);

			Assert::AreEqual(3.0, field.getValue(2, 3, 4));
			Assert::AreEqual(2.0, field.getValue(4, 4, 4));
		}

		TEST_METHOD(Get)
		{
			StaticScalarField field(10, 10, 10, 3.0);
			field.setBoundaryValue(10.0);
			field.setValue(4, 4, 4, 2.0);

			Assert::AreEqual(10.0, field.get(20.0, 20.0, 20.0));
			Assert::AreEqual(3.0, field.get(8.0, 8.0, 8.0));
			Assert::AreEqual(2.0, field.get(4.0, 4.0, 4.0));
			Assert::AreEqual(2.5, field.get(4.0, 4.5, 4.0));
		}
	};

	TEST_CLASS(Vector2UnitTests)
	{
	public:

		TEST_METHOD(Distance)
		{
			Vector2f v1(0.0f, 1.0f);
			Vector2f v2(1.5f, 2.4f);

			Assert::AreEqual(2.05182845f, Vector2f::distance(v1, v2));
		}

		TEST_METHOD(DistanceSquared)
		{
			Vector2f v1(0.0f, 1.0f);
			Vector2f v2(1.5f, 2.4f);

			Assert::AreEqual(4.21f, Vector2f::distanceSq(v1, v2));
		}

		TEST_METHOD(DotProduct)
		{
			Vector2f v1(0.0f, 1.0f);
			Vector2f v2(1.5f, 2.4f);

			Assert::AreEqual(2.4f, Vector2f::dot(v1, v2));
		}

		TEST_METHOD(LinearInterpolation)
		{
			Vector2f v1(0.0f, 1.0f);
			Vector2f v2(2.0f, 2.4f);

			Assert::AreEqual(Vector2f(1.0f, 1.7f), Vector2f::lerp(v1, v2, 0.5));
		}

		TEST_METHOD(Orthogonalize)
		{
			Vector2f v1(3.0f, 1.0f);
			Vector2f v2(2.0f, 4.0f);

			Vector2f::orthogonalize(v1, v2);

			Assert::AreEqual(Vector2f(0.948683298f, 0.316227766f), v1);
			Assert::AreEqual(Vector2f(-0.316227766f, 0.948683298f), v2);
		}

		TEST_METHOD(Project)
		{
			Vector2f v1(2.0f, 2.4f);
			Vector2f v2(0.0f, 1.0f);

			Assert::AreEqual(Vector2f(0.0f, 2.4f), Vector2f::proj(v1, v2));
		}

		TEST_METHOD(Perpendicular)
		{
			Vector2f v1(2.0f, 2.4f);
			Vector2f v2(0.0f, 1.0f);

			Assert::AreEqual(Vector2f(2.0f, 0.0f), Vector2f::perp(v1, v2));
		}

		TEST_METHOD(Reflection)
		{
			Vector2f i(2.0f, 1.0f);
			Vector2f n(0.0f, 1.0f);

			Assert::AreEqual(Vector2f(2.0f, -1.0f), Vector2f::reflect(i, n));
		}

		TEST_METHOD(NormalizeStatic)
		{
			Vector2f v(2.0f, 1.0f);

			Assert::AreEqual(Vector2f(0.894427191f, 0.447213595f), Vector2f::normalize(v));
		}

		TEST_METHOD(Smooth)
		{
			//TODO
			Assert::AreEqual(true, false);
		}

		TEST_METHOD(Magnitude)
		{
			Vector2f v(2.0f, 1.0f);
 			Assert::AreEqual(sqrt(5.0f), v.magnitude());
		}

		TEST_METHOD(MagnitudeSquared)
		{
			Vector2f v(2.0f, 1.0f);
			Assert::AreEqual(5.0f, v.magnitudeSq());
		}

		TEST_METHOD(Inverse)
		{
			Vector2f v(2.0f, 1.0f);

			Assert::AreEqual(Vector2f(-2.0f, -1.0f), v.inverse());
		}

		TEST_METHOD(Normalize)
		{
			Vector2f v(2.0f, 1.0f);
			v.normalize();

			Assert::AreEqual(Vector2f(0.894427191f, 0.447213595f), v);
		}

		TEST_METHOD(Scale)
		{
			Vector2f v(2.0f, 1.0f);
			v.scale(10.0f);

			Assert::AreEqual(Vector2f(20.0f, 10.0f), v);
		}
		
		TEST_METHOD(Cast)
		{
			Vector2f v(2.6f, 1.9f);
			Assert::AreEqual(Vector2i(2, 1), v.cast<int>());
		}

		TEST_METHOD(OperatorEquals)
		{
			Vector2f v1(2.0f, 1.0f);
			Vector2f v2(2.0000001f, 1.0000001f);

			Assert::AreEqual(true, v1 == v2);
		}

		TEST_METHOD(OperatorNotEqual)
		{
			Vector2f v1(2.0f, 1.0f);
			Vector2f v2(4.0f, 2.3f);

			Assert::AreEqual(true, v1 != v2);
		}

		TEST_METHOD(OperatorAddAssign)
		{
			Vector2f v1(2.0f, 1.0f);
			Vector2f v2(4.0f, 2.3f);

			v1 += v2;

			Assert::AreEqual(Vector2f(6.0f, 3.3f), v1);
		}

		TEST_METHOD(OperatorAdd)
		{
			Vector2f v1(2.0f, 1.0f);
			Vector2f v2(4.0f, 2.3f);

			Assert::AreEqual(Vector2f(6.0f, 3.3f), v1 + v2);
		}

		TEST_METHOD(OperatorSubtractAssign)
		{
			Vector2f v1(2.0f, 1.0f);
			Vector2f v2(4.0f, 2.3f);

			v1 -= v2;

			Assert::AreEqual(Vector2f(-2.0f, -1.3f), v1);
		}

		TEST_METHOD(OperatorSubtract)
		{
			Vector2f v1(2.0f, 1.0f);
			Vector2f v2(4.0f, 2.3f);

			Assert::AreEqual(Vector2f(-2.0f, -1.3f), v1 - v2);
		}

		TEST_METHOD(OperatorMultiplyAssign)
		{
			Vector2f v1(2.0f, 1.0f);

			v1 *= 2;

			Assert::AreEqual(Vector2f(4.0f, 2.0f), v1);
		}

		TEST_METHOD(OperatorMultiply)
		{
			Vector2f v1(2.0f, 1.0f);

			Assert::AreEqual(Vector2f(4.0f, 2.0f), v1 * 2);
		}

		TEST_METHOD(OperatorDivideAssign)
		{
			Vector2f v1(2.0f, 1.0f);

			v1 /= 2;

			Assert::AreEqual(Vector2f(1.0f, 0.5f), v1);
		}

		TEST_METHOD(OperatorDivide)
		{
			Vector2f v1(2.0f, 1.0f);

			Assert::AreEqual(Vector2f(1.0f, 0.5f), v1 / 2);
		}

		TEST_METHOD(LessThanLess)
		{
			Vector2f v1(2.0f, 1.0f);
			Vector2f v2(1.0f, 0.5f);

			Assert::AreEqual(true, v2 < v1);
		}

		TEST_METHOD(LessThanOneCompGreater)
		{
			Vector2f v1(2.0f, 1.0f);
			Vector2f v2(4.0f, 0.5f);

			Assert::AreEqual(false, v2 < v1);
		}
	};

	TEST_CLASS(Vector3UnitTests)
	{
	public:

		TEST_METHOD(Distance)
		{
			Vector3f v1(0.0f, 1.0f, 2.0f);
			Vector3f v2(1.5f, 2.4f, 4.0f);

			Assert::AreEqual(2.86530976f, Vector3f::distance(v1, v2));
		}

		TEST_METHOD(DistanceSquared)
		{
			Vector3f v1(0.0f, 1.0f, 2.0f);
			Vector3f v2(1.5f, 2.4f, 4.0f);

			Assert::AreEqual(8.21f, Vector3f::distanceSq(v1, v2));
		}

		TEST_METHOD(DotProduct)
		{
			Vector3f v1(0.0f, 1.0f, 2.0f);
			Vector3f v2(1.5f, 2.4f, 1.0f);

			Assert::AreEqual(4.4f, Vector3f::dot(v1, v2));
		}

		TEST_METHOD(LinearInterpolation)
		{
			Vector3f v1(0.0f, 1.0f, 1.0f);
			Vector3f v2(2.0f, 2.4f, 2.0f);

			Assert::AreEqual(Vector3f(1.0f, 1.7f, 1.5f), Vector3f::lerp(v1, v2, 0.5));
		}

		TEST_METHOD(Orthogonalize)
		{
			//TODO
			Assert::AreEqual(true, false);
		}

		TEST_METHOD(Project)
		{
			Vector3f v1(2.0f, 2.4f, 0.0f);
			Vector3f v2(0.0f, 1.0f, 1.0f);

			Assert::AreEqual(Vector3f(0.0f, 1.2f, 1.2f), Vector3f::proj(v1, v2));
		}

		TEST_METHOD(Perpendicular)
		{
			Vector3f v1(2.0f, 2.4f, 0.0f);
			Vector3f v2(0.0f, 1.0f, 0.0f);

			Assert::AreEqual(Vector3f(2.0f, 0.0f, 0.0f), Vector3f::perp(v1, v2));
		}

		TEST_METHOD(Reflection)
		{
			Vector3f i(2.0f, 1.0f, 1.0f);
			Vector3f n(0.0f, 1.0f, 0.0f);

			Assert::AreEqual(Vector3f(2.0f, -1.0f, 1.0f), Vector3f::reflect(i, n));
		}

		TEST_METHOD(NormalizeStatic)
		{
			Vector3f v(2.0f, 1.0f, 1.0f);

			Assert::AreEqual(Vector3f(0.81649658f, 0.40824829f, 0.40824829f), Vector3f::normalize(v));
		}

		TEST_METHOD(Smooth)
		{
			//TODO
			Assert::AreEqual(true, false);
		}

		TEST_METHOD(Magnitude)
		{
			Vector3f v(2.0f, 1.0f, 1.0f);
			Assert::AreEqual(sqrt(6.0f), v.magnitude());
		}

		TEST_METHOD(MagnitudeSquared)
		{
			Vector3f v(2.0f, 1.0f, 1.0f);
			Assert::AreEqual(6.0f, v.magnitudeSq());
		}

		TEST_METHOD(Inverse)
		{
			Vector3f v(2.0f, 1.0f, 1.0f);

			Assert::AreEqual(Vector3f(-2.0f, -1.0f, -1.0f), v.inverse());
		}

		TEST_METHOD(Normalize)
		{
			Vector3f v(2.0f, 1.0f, 1.0f);
			v.normalize();

			Assert::AreEqual(Vector3f(0.81649658f, 0.40824829f, 0.40824829f), v);
		}

		TEST_METHOD(Scale)
		{
			Vector3f v(2.0f, 1.0f, 1.0f);
			v.scale(10.0f);

			Assert::AreEqual(Vector3f(20.0f, 10.0f, 10.0f), v);
		}

		TEST_METHOD(Cast)
		{
			Vector3f v(2.6f, 1.9f, 1.2f);

			Assert::AreEqual(Vector3i(2, 1, 1), v.cast<int>());
		}

		TEST_METHOD(OperatorEquals)
		{
			Vector3f v1(2.0f, 1.0f, 2.2f);
			Vector3f v2(2.0000001f, 1.0000001f, 2.20000001f);

			Assert::AreEqual(true, v1 == v2);
		}

		TEST_METHOD(OperatorNotEqual)
		{
			Vector3f v1(2.0f, 1.0f, 2.2f);
			Vector3f v2(4.0f, 2.3f, 2.2f);

			Assert::AreEqual(true, v1 != v2);
		}

		TEST_METHOD(OperatorAddAssign)
		{
			Vector3f v1(2.0f, 1.0f, 1.0f);
			Vector3f v2(4.0f, 2.3f, 3.2f);

			v1 += v2;

			Assert::AreEqual(Vector3f(6.0f, 3.3f, 4.2f), v1);
		}

		TEST_METHOD(OperatorAdd)
		{
			Vector3f v1(2.0f, 1.0f, 1.0f);
			Vector3f v2(4.0f, 2.3f, 3.2f);

			Assert::AreEqual(Vector3f(6.0f, 3.3f, 4.2f), v1 + v2);
		}

		TEST_METHOD(OperatorSubtractAssign)
		{
			Vector3f v1(2.0f, 1.0f, 1.0f);
			Vector3f v2(4.0f, 2.3f, 3.2f);

			v1 -= v2;

			Assert::AreEqual(Vector3f(-2.0f, -1.3f, -2.2f), v1);
		}

		TEST_METHOD(OperatorSubtract)
		{
			Vector3f v1(2.0f, 1.0f, 1.0f);
			Vector3f v2(4.0f, 2.3f, 3.2f);

			Assert::AreEqual(Vector3f(-2.0f, -1.3f, -2.2f), v1 - v2);
		}

		TEST_METHOD(OperatorMultiplyAssign)
		{
			Vector3f v1(2.0f, 1.0f, 1.0f);

			v1 *= 2;

			Assert::AreEqual(Vector3f(4.0f, 2.0f, 2.0f), v1);
		}

		TEST_METHOD(OperatorMultiply)
		{
			Vector3f v1(2.0f, 1.0f, 1.0f);

			Assert::AreEqual(Vector3f(4.0f, 2.0f, 2.0f), v1 * 2);
		}

		TEST_METHOD(OperatorDivideAssign)
		{
			Vector3f v1(2.0f, 1.0f, 1.0f);

			v1 /= 2;

			Assert::AreEqual(Vector3f(1.0f, 0.5f, 0.5f), v1);
		}

		TEST_METHOD(OperatorDivide)
		{
			Vector3f v1(2.0f, 1.0f, 1.0f);

			Assert::AreEqual(Vector3f(1.0f, 0.5f, 0.5f), v1 / 2);
		}

		TEST_METHOD(LessThanLess)
		{
			Vector3f v1(2.0f, 1.0f, 2.0f);
			Vector3f v2(1.0f, 0.5f, 1.0f);

			Assert::AreEqual(true, v2 < v1);
		}

		TEST_METHOD(LessThanOneCompGreater)
		{
			Vector3f v1(2.0f, 1.0f, 2.0f);
			Vector3f v2(4.0f, 0.5f, 1.0f);

			Assert::AreEqual(false, v2 < v1);
		}
	};

	TEST_CLASS(Vector4UnitTests)
	{
	public:

		TEST_METHOD(Distance)
		{
			Vector4f v1(0.0f, 1.0f, 2.0f, 2.0f);
			Vector4f v2(1.5f, 2.4f, 4.0f, 0.0f);

			Assert::AreEqual(3.49428104193f, Vector4f::distance(v1, v2));
		}

		TEST_METHOD(DistanceSquared)
		{
			Vector4f v1(0.0f, 1.0f, 2.0f, 2.0f);
			Vector4f v2(1.5f, 2.4f, 4.0f, 0.0f);

			Assert::AreEqual(12.21f, Vector4f::distanceSq(v1, v2));
		}

		TEST_METHOD(DotProduct)
		{
			Vector4f v1(0.0f, 1.0f, 2.0f, 2.0f);
			Vector4f v2(1.5f, 2.4f, 1.0f, 0.0f);

			Assert::AreEqual(4.4f, Vector4f::dot(v1, v2));
		}

		TEST_METHOD(LinearInterpolation)
		{
			Vector4f v1(0.0f, 1.0f, 1.0f, 2.0f);
			Vector4f v2(2.0f, 2.4f, 2.0f, -2.0f);

			Assert::AreEqual(Vector4f(1.0f, 1.7f, 1.5f, 0.0f), Vector4f::lerp(v1, v2, 0.5));
		}

		TEST_METHOD(Orthogonalize)
		{
			//TODO
			Assert::AreEqual(true, false);
		}

		TEST_METHOD(Project)
		{
			Vector4f v1(2.0f, 2.4f, 0.0f, 2.0f);
			Vector4f v2(0.0f, 1.0f, 1.0f, 0.0f);

			Assert::AreEqual(Vector4f(0.0f, 1.2f, 1.2f, 0.0f), Vector4f::proj(v1, v2));
		}

		TEST_METHOD(Perpendicular)
		{
			Vector4f v1(2.0f, 2.4f, 0.0f, 0.0f);
			Vector4f v2(0.0f, 1.0f, 0.0f, 0.0f);

			Assert::AreEqual(Vector4f(2.0f, 0.0f, 0.0f, 0.0f), Vector4f::perp(v1, v2));
		}

		TEST_METHOD(Reflection)
		{
			Vector4f i(2.0f, 1.0f, 1.0f, 3.0f);
			Vector4f n(0.0f, 1.0f, 0.0f, 0.0f);

			Assert::AreEqual(Vector4f(2.0f, -1.0f, 1.0f, 3.0f), Vector4f::reflect(i, n));
		}

		TEST_METHOD(NormalizeStatic)
		{
			Vector4f v(2.0f, 1.0f, 1.0f, 3.0f);

			Assert::AreEqual(Vector4f(0.516397778f, 0.258198889f, 0.258198889f, 0.774596667), Vector4f::normalize(v));
		}

		TEST_METHOD(Smooth)
		{
			//TODO
			Assert::AreEqual(true, false);
		}

		TEST_METHOD(Magnitude)
		{
			Vector4f v(2.0f, 1.0f, 1.0f, 3.0f);
			Assert::AreEqual(sqrt(15.0f), v.magnitude());
		}

		TEST_METHOD(MagnitudeSquared)
		{
			Vector4f v(2.0f, 1.0f, 1.0f, 3.0f);
			Assert::AreEqual(15.0f, v.magnitudeSq());
		}

		TEST_METHOD(Inverse)
		{
			Vector4f v(2.0f, 1.0f, 1.0f, 3.0f);

			Assert::AreEqual(Vector4f(-2.0f, -1.0f, -1.0f, -3.0f), v.inverse());
		}

		TEST_METHOD(Normalize)
		{
			Vector4f v(2.0f, 1.0f, 1.0f, 3.0f);
			v.normalize();

			Assert::AreEqual(Vector4f(0.516397778f, 0.258198889f, 0.258198889f, 0.774596667), v);
		}

		TEST_METHOD(Scale)
		{
			Vector4f v(2.0f, 1.0f, 1.0f, 3.0f);
			v.scale(10.0f);

			Assert::AreEqual(Vector4f(20.0f, 10.0f, 10.0f, 30.0f), v);
		}

		TEST_METHOD(Cast)
		{
			Vector4f v(2.6f, 1.9f, 1.2f, 3.3f);

			Assert::AreEqual(Vector4i(2, 1, 1, 3), v.cast<int>());
		}

		TEST_METHOD(OperatorEquals)
		{
			Vector4f v1(2.0f, 1.0f, 2.2f, 3.3f);
			Vector4f v2(2.0000001f, 1.0000001f, 2.20000001f, 3.2999999999f);

			Assert::AreEqual(true, v1 == v2);
		}

		TEST_METHOD(OperatorNotEqual)
		{
			Vector4f v1(2.0f, 1.0f, 2.2f, 3.3f);
			Vector4f v2(4.0f, 2.3f, 2.2f, 3.2f);

			Assert::AreEqual(true, v1 != v2);
		}

		TEST_METHOD(OperatorAddAssign)
		{
			Vector4f v1(2.0f, 1.0f, 1.0f, 3.0f);
			Vector4f v2(4.0f, 2.3f, 3.2f, 1.0f);

			v1 += v2;

			Assert::AreEqual(Vector4f(6.0f, 3.3f, 4.2f, 4.0f), v1);
		}

		TEST_METHOD(OperatorAdd)
		{
			Vector4f v1(2.0f, 1.0f, 1.0f, 3.0f);
			Vector4f v2(4.0f, 2.3f, 3.2f, 1.0f);

			Assert::AreEqual(Vector4f(6.0f, 3.3f, 4.2f, 4.0f), v1 + v2);
		}

		TEST_METHOD(OperatorSubtractAssign)
		{
			Vector4f v1(2.0f, 1.0f, 1.0f, 3.0f);
			Vector4f v2(4.0f, 2.3f, 3.2f, 1.0f);

			v1 -= v2;

			Assert::AreEqual(Vector4f(-2.0f, -1.3f, -2.2f, 2.0f), v1);
		}

		TEST_METHOD(OperatorSubtract)
		{
			Vector4f v1(2.0f, 1.0f, 1.0f, 3.0f);
			Vector4f v2(4.0f, 2.3f, 3.2f, 1.0f);

			Assert::AreEqual(Vector4f(-2.0f, -1.3f, -2.2f, 2.0f), v1 - v2);
		}

		TEST_METHOD(OperatorMultiplyAssign)
		{
			Vector4f v1(2.0f, 1.0f, 1.0f, 3.0f);

			v1 *= 2;

			Assert::AreEqual(Vector4f(4.0f, 2.0f, 2.0f, 6.0f), v1);
		}

		TEST_METHOD(OperatorMultiply)
		{
			Vector4f v1(2.0f, 1.0f, 1.0f, 3.0f);

			Assert::AreEqual(Vector4f(4.0f, 2.0f, 2.0f, 6.0f), v1 * 2);
		}

		TEST_METHOD(OperatorDivideAssign)
		{
			Vector4f v1(2.0f, 1.0f, 1.0f, 3.0f);

			v1 /= 2;

			Assert::AreEqual(Vector4f(1.0f, 0.5f, 0.5f, 1.5f), v1);
		}

		TEST_METHOD(OperatorDivide)
		{
			Vector4f v1(2.0f, 1.0f, 1.0f, 3.0f);

			Assert::AreEqual(Vector4f(1.0f, 0.5f, 0.5f, 1.5f), v1 / 2);
		}

		TEST_METHOD(LessThanLess)
		{
			Vector4f v1(2.0f, 1.0f, 2.0f, 3.0f);
			Vector4f v2(1.0f, 0.5f, 1.0f, 2.0f);

			Assert::AreEqual(true, v2 < v1);
		}

		TEST_METHOD(LessThanOneCompGreater)
		{
			Vector4f v1(2.0f, 1.0f, 2.0f, 3.0f);
			Vector4f v2(4.0f, 0.5f, 1.0f, 2.0f);

			Assert::AreEqual(false, v2 < v1);
		}
	};
}