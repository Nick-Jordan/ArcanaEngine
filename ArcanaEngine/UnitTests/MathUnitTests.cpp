#include "stdafx.h"
#include "CppUnitTest.h"

#include "ArcanaMath.h"
#include "AxisAlignedBoundingBox.h"
#include "DynamicField.h"
#include "Ellipsoid.h"
#include "Frustum.h"
#include "Intersector.h"
#include "Matrix3.h"
#include "Matrix4.h"
#include "OrientedBoundingBox.h"
#include "Plane.h"
#include "Ray3.h"
#include "Sphere.h"
#include "StaticField.h"

#include "UnitTestToString.h"

using namespace Arcana;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

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
			Assert::IsTrue(Math::closeEnough(1.0, 1.0000001));
		}

		TEST_METHOD(NotCloseEnough)
		{
			Assert::IsFalse(Math::closeEnough(1.0, 1.0001));
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

			Assert::IsTrue(box.contains(5.0f, 5.0f, 5.0f));
			Assert::IsFalse(box.contains(20.0f, 20.0f, 20.0f));
		}

		TEST_METHOD(ContainsAABB)
		{
			AxisAlignedBoundingBoxf box(0.0f, 1.0f, 2.0f, 10.0f, 11.0f, 12.0f);
			AxisAlignedBoundingBoxf inside(5.0f, 5.0f, 5.0f, 6.0f, 7.0f, 8.0f);
			AxisAlignedBoundingBoxf outside(20.0f, 21.0f, 22.0f, 30.0f, 31.0f, 32.0f);
			AxisAlignedBoundingBoxf partiallyInside(5.0f, 5.0f, 5.0f, 12.0f, 13.0f, 14.0f);

			Assert::IsTrue(box.contains(inside));
			Assert::IsFalse(box.contains(outside));
			Assert::IsFalse(box.contains(partiallyInside));
		}

		TEST_METHOD(IntersectsAABB)
		{
			AxisAlignedBoundingBoxf box(0.0f, 1.0f, 2.0f, 10.0f, 11.0f, 12.0f);
			AxisAlignedBoundingBoxf inside(5.0f, 5.0f, 5.0f, 6.0f, 7.0f, 8.0f);
			AxisAlignedBoundingBoxf outside(20.0f, 21.0f, 22.0f, 30.0f, 31.0f, 32.0f);
			AxisAlignedBoundingBoxf partiallyInside(5.0f, 5.0f, 5.0f, 12.0f, 13.0f, 14.0f);

			Assert::IsTrue(box.intersects(inside));
			Assert::IsFalse(box.intersects(outside));
			Assert::IsTrue(box.intersects(partiallyInside));
		}

		TEST_METHOD(IsEmpty)
		{
			AxisAlignedBoundingBoxf box1(0.0f, 1.0f, 2.0f, 10.0f, 11.0f, 12.0f);
			AxisAlignedBoundingBoxf box2(10.0f, 2.0f, 3.0f, 10.0f, 2.0f, 3.0f);

			Assert::IsFalse(box1.isEmpty());
			Assert::IsTrue(box2.isEmpty());
		}

		TEST_METHOD(Resize)
		{
			AxisAlignedBoundingBoxf box(0.0f, 1.0f, 2.0f, 10.0f, 11.0f, 12.0f);

			box.resize(10.0, 10.0, 10.0);
			Assert::AreEqual(Vector3f(-10.0f, -9.0f, -8.0f), box.getMin());
			Assert::AreEqual(Vector3f(20.0f, 21.0f, 22.0f), box.getMax());

			box.resize(Vector3f::one());
			Assert::AreEqual(Vector3f(-11.0f, -10.0f, -9.0f), box.getMin());
			Assert::AreEqual(Vector3f(21.0f, 22.0f, 23.0f), box.getMax());
		}

		TEST_METHOD(Offset)
		{
			AxisAlignedBoundingBoxf box(0.0f, 1.0f, 2.0f, 10.0f, 11.0f, 12.0f);

			box.offset(10.0, 10.0, 10.0);
			Assert::AreEqual(Vector3f(10.0f, 11.0f, 12.0f), box.getMin());
			Assert::AreEqual(Vector3f(20.0f, 21.0f, 22.0f), box.getMax());

			box.offset(Vector3f::one());
			Assert::AreEqual(Vector3f(11.0f, 12.0f, 13.0f), box.getMin());
			Assert::AreEqual(Vector3f(21.0f, 22.0f, 23.0f), box.getMax());
		}

		TEST_METHOD(Merge)
		{
			AxisAlignedBoundingBoxf box1(0.0f, 1.0f, 2.0f, 10.0f, 11.0f, 12.0f);
			AxisAlignedBoundingBoxf box2(1.0f, 2.0f, 0.0f, 11.0f, 4.0f, 5.0f);

			box1.merge(box2);

			Assert::AreEqual(Vector3f(0.0f, 1.0f, 0.0f), box1.getMin());
			Assert::AreEqual(Vector3f(11.0f, 11.0f, 12.0f), box1.getMax());

			Assert::AreEqual(Vector3f(1.0f, 2.0f, 0.0f), box2.getMin());
			Assert::AreEqual(Vector3f(11.0f, 4.0f, 5.0f), box2.getMax());
		}

		TEST_METHOD(Cast)
		{
			AxisAlignedBoundingBoxf box(0.5f, 1.2f, 2.3f, 10.2f, 11.1f, 12.06f);

			AxisAlignedBoundingBoxi boxInt = box.cast<int>();

			Assert::AreEqual(Vector3i(0, 1, 2), boxInt.getMin());
			Assert::AreEqual(Vector3i(10, 11, 12), boxInt.getMax());
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

			Assert::IsTrue(ellipsoid.contains(1.5f, 1.5f, 1.5f));
			Assert::IsFalse(ellipsoid.contains(3.0f, 4.0f, 3.0f));
		}
	};

	/*TEST_CLASS(EquationUnitTests)
	{
	public:

	};??????????????*/

	TEST_CLASS(FrustumUnitTests)
	{
	public:

		TEST_METHOD(Accessors)
		{
			Frustumd frustum(Matrix4d::IDENTITY);

			Assert::AreEqual(Matrix4d::IDENTITY, frustum.getMatrix());

			Assert::AreEqual(Vector3d(1, 0, 0), frustum.getLeft().getNormal());
			Assert::AreEqual(Vector3d(-1, 0, 0), frustum.getRight().getNormal());
			Assert::AreEqual(Vector3d(0, -1, 0), frustum.getTop().getNormal());
			Assert::AreEqual(Vector3d(0, 1, 0), frustum.getBottom().getNormal());
			Assert::AreEqual(Vector3d(0, 0, 1), frustum.getNear().getNormal());
			Assert::AreEqual(Vector3d(0, 0, -1), frustum.getFar().getNormal());

			Assert::AreEqual(1.0, frustum.getLeft().getDistance());
			Assert::AreEqual(1.0, frustum.getRight().getDistance());
			Assert::AreEqual(1.0, frustum.getTop().getDistance());
			Assert::AreEqual(1.0, frustum.getBottom().getDistance());
			Assert::AreEqual(0.0, frustum.getNear().getDistance());
			Assert::AreEqual(1.0, frustum.getFar().getDistance());
		}
	};

	TEST_CLASS(IntersectorUnitTests)
	{
	public:

		TEST_METHOD(SphereSphereIntersection)
		{
			Spheref sphere0(Vector3f::zero(), 10.0);
			Spheref sphere1(Vector3f(30.0, 30.0, 30.0), 1.0);

			Assert::IsFalse(Intersector<float>::intersect(sphere0, sphere1));

			sphere1 = Spheref(Vector3f(30.0, 0.0, 0.0), 20.0);
			Assert::IsTrue(Intersector<float>::intersect(sphere0, sphere1));
		}

		TEST_METHOD(SphereRayIntersection)
		{
			Spheref sphere(Vector3f::zero(), 10.0);
			Ray3f ray(Vector3f(0.0, 0.0, 20.0), Vector3f(1.0, 0.0, 0.0));

			Assert::IsFalse(Intersector<float>::intersect(sphere, ray));

			ray = Ray3f(Vector3f(0.0, 0.0, 20.0), Vector3f(0.0, 0.0, -1.0));
			Assert::IsTrue(Intersector<float>::intersect(sphere, ray));
		}

		TEST_METHOD(SphereAABBIntersection)
		{
			Spheref sphere(Vector3f::zero(), 10.0);
			AxisAlignedBoundingBox aabb(Vector3f(-10.0, -10.0, -10.0), Vector3f(-9.0, -9.0, -9.0));

			Assert::IsFalse(Intersector<float>::intersect(sphere, aabb));

			aabb = AxisAlignedBoundingBox(Vector3f(-10.0, -10.0, -10.0), Vector3f::zero());
			Assert::IsTrue(Intersector<float>::intersect(sphere, aabb));
		}

		TEST_METHOD(SphereOBBIntersection)
		{
			Spheref sphere(Vector3f::zero(), 16.0);
			Quaternionf quat;
			quat.fromAxisAngle(Vector3f::unitZ(), 45.0f);
			OrientedBoundingBoxf obb(Vector3f(-10.0, -10.0, -10.0), Vector3f::one(), quat);

			Assert::IsFalse(Intersector<float>::intersect(sphere, obb));

			obb.setRotation(Quaternionf::IDENTITY);
			Assert::IsTrue(Intersector<float>::intersect(sphere, obb));
		}

		TEST_METHOD(SphereFrustumIntersection)
		{
			Spheref sphere(Vector3f(0.0, 0.0, 100.0), 2.0);
			Matrix4f perspective = Matrix4f::createPerspective(90.0, 1.0, 1.0, 1000.0);
			Frustumf frustum(perspective);

			Assert::IsFalse(Intersector<float>::intersect(sphere, frustum));

			sphere.setCenter(Vector3f(0.0, 0.0, -100.0));
			Assert::IsTrue(Intersector<float>::intersect(sphere, frustum));
		}

		TEST_METHOD(SpherePlaneIntersection)
		{
			Spheref sphere(Vector3f(0.0, 100.0, 0.0), 2.0);
			Planef plane(0, Vector3f(0, 1, 0));

			Assert::AreEqual(PLANE_INTERSECTS_FRONT, Intersector<float>::intersect(sphere, plane));

			plane.setNormal(Vector3f(1, 0, 0));
			Assert::AreEqual(PLANE_INTERSECTS, Intersector<float>::intersect(sphere, plane));
		}

		TEST_METHOD(RayBoxIntersection)
		{
			Ray3f ray(Vector3f(0.0, 0.0, 20.0), Vector3f(1.0, 0.0, 0.0));
			AxisAlignedBoundingBoxf aabb(Vector3f(0.0, 0.0, 0.0), Vector3f(10, 10, 10));

			Assert::IsFalse(Intersector<float>::intersect(ray, aabb));

			ray.setDirection(Vector3f(0, 0, -1));
			Assert::IsTrue(Intersector<float>::intersect(ray, aabb));
		}

		TEST_METHOD(RayFrustumIntersection)
		{
			Ray3f ray(Vector3f(0.0, 0.0, 100.0), Vector3f(1.0, 0.0, 0.0));
			Matrix4f perspective = Matrix4f::createPerspective(90.0, 1.0, 1.0, 1000.0);
			Frustumf frustum(perspective);

			Assert::IsFalse(Intersector<float>::intersect(ray, frustum));

			//ray.setDirection(Vector3f(0, 0, 1));
			//Assert::IsTrue(Intersector<float>::intersect(ray, frustum));
		}
		TEST_METHOD(RayPlaneIntersection)
		{
			Ray3f ray(Vector3f(0.0, 10.0, 20.0), Vector3f(1.0, 0.0, 0.0));
			Planef plane(0, Vector3f(0, 1, 0));

			Assert::IsFalse(Intersector<float>::intersect(ray, plane));

			ray.setDirection(Vector3f(0, -1, 0));
			Assert::IsTrue(Intersector<float>::intersect(ray, plane));
		}

		TEST_METHOD(BoxBoxIntersection)
		{
			AxisAlignedBoundingBoxf a(Vector3f(0.0, 0.0, 0.0), Vector3f(1.0, 1.0, 1.0));
			AxisAlignedBoundingBoxf b(Vector3f(-2, -2, -2), Vector3f(-1.0, -1.0, -1.0));

			Assert::IsFalse(Intersector<float>::intersect(a, b));

			b.setMax(Vector3f(1, 1, 1));
			Assert::IsTrue(Intersector<float>::intersect(a, b));
		}

		TEST_METHOD(BoxFrustumIntersection)
		{
			AxisAlignedBoundingBoxf aabb(Vector3f(-2.0, -2.0, 98.0), Vector3f(2.0, 2.0, 102.0));
			Matrix4f perspective = Matrix4f::createPerspective(90.0, 1.0, 1.0, 1000.0);
			Frustumf frustum(perspective);

			Assert::IsFalse(Intersector<float>::intersect(aabb, frustum));

			aabb.set(Vector3f(-2.0, -2.0, -102.0), Vector3f(2.0, 2.0, -98.0));
			Assert::IsTrue(Intersector<float>::intersect(aabb, frustum));
		}

		TEST_METHOD(BoxPlaneIntersection)
		{
			AxisAlignedBoundingBoxf aabb(Vector3f(-2.0, 98.0, -2.0), Vector3f(2.0, 102.0, 2.0));
			Planef plane(0, Vector3f(0, 1, 0));

			Assert::AreEqual(PLANE_INTERSECTS_FRONT, Intersector<float>::intersect(aabb, plane));

			plane.setNormal(Vector3f(1, 0, 0));
			Assert::AreEqual(PLANE_INTERSECTS, Intersector<float>::intersect(aabb, plane));
		}

		TEST_METHOD(FrustumPlaneIntersection)
		{
			Planef plane(10, Vector3f(0, 0, -1));
			Matrix4f perspective = Matrix4f::createPerspective(90.0, 1.0, 1.0, 1000.0);
			Frustumf frustum(perspective);

			Assert::AreEqual(PLANE_INTERSECTS_FRONT, Intersector<float>::intersect(frustum, plane));

			plane.setNormal(Vector3f(0, 1, 0));
			plane.setDistance(0);
			Assert::AreEqual(PLANE_INTERSECTS, Intersector<float>::intersect(frustum, plane));
		}

		TEST_METHOD(PlanePlaneIntersection)
		{
			Planef a(0, Vector3f(0, 1, 0));
			Planef b(10, Vector3f(0, 1, 0));

			Assert::AreEqual(PLANE_INTERSECTS_FRONT, Intersector<float>::intersect(a, b));

			a.setNormal(Vector3f(1, 0, 0));
			Assert::AreEqual(PLANE_INTERSECTS, Intersector<float>::intersect(a, b));
		}
	};

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

			Assert::AreEqual(Vector3f(0.0f, 1.0f, 2.0f), box.getCenter());
			Assert::AreEqual(Vector3f(10.0f, 11.0f, 12.0f), box.getExtents());
			//Assert::AreEqual(transformation, box.getTransformation());
		}

		TEST_METHOD(GetSize)
		{
			Quaternionf rotation;
			OrientedBoundingBoxf box(0.0f, 1.0f, 2.0f, 10.0f, 11.0f, 12.0f, rotation);

			Assert::AreEqual(Vector3f(20.0f, 22.0f, 24.0f), box.getSize());
			Assert::AreEqual(20.0f, box.getWidth());
			Assert::AreEqual(22.0f, box.getHeight());
			Assert::AreEqual(24.0f, box.getDepth());
		}

		TEST_METHOD(ContainsPoint)
		{
			Quaternionf rotation;
			rotation.fromAxisAngle(Vector3f::unitZ(), 45.0f);
			OrientedBoundingBoxf box(0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, rotation);

			Assert::IsTrue(box.contains(sqrt(2.0f), 0.0f, 0.0f));
			Assert::IsFalse(box.contains(1.0f, 1.0f, 0.0f));
		}

		TEST_METHOD(ContainsOBB)
		{
			Quaternionf rotation;
			rotation.fromAxisAngle(Vector3f::unitZ(), 45.0f);
			OrientedBoundingBoxf box(0.0f, 0.0f, 0.0f, 2.0f, 2.0f, 2.0f, rotation);
			OrientedBoundingBoxf contained(0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, rotation);
			OrientedBoundingBoxf notContained(0.0f, 0.0f, 0.0f, 3.0f, 1.0f, 1.0f, rotation);
			OrientedBoundingBoxf wouldBeContained(0.0f, 0.0f, 0.0f, 2.0f, 2.0f, 2.0f);

			Assert::IsTrue(box.contains(contained));
			Assert::IsFalse(box.contains(notContained));
			Assert::IsFalse(box.contains(wouldBeContained));
		}

		TEST_METHOD(IntersectsOBB)
		{
			
		}

		TEST_METHOD(IsEmpty)
		{
			Quaternionf rotation;
			OrientedBoundingBoxf box1(0.0f, 1.0f, 2.0f, 10.0f, 11.0f, 12.0f, rotation);
			OrientedBoundingBoxf box2(10.0f, 2.0f, 3.0f, 10.0f, 2.0f, 3.0f, rotation);

			Assert::IsFalse(box1.isEmpty());
			//Assert::IsTrue(box2.isEmpty());
		}

		TEST_METHOD(Resize)
		{
			OrientedBoundingBoxf box(0.0f, 1.0f, 2.0f, 10.0f, 11.0f, 12.0f);

			box.resize(10.0, 10.0, 10.0);
			Assert::AreEqual(Vector3f(0.0f, 1.0f, 2.0f), box.getCenter());
			Assert::AreEqual(Vector3f(20.0f, 21.0f, 22.0f), box.getExtents());

			box.resize(Vector3f::one());
			Assert::AreEqual(Vector3f(0.0f, 1.0f, 2.0f), box.getCenter());
			Assert::AreEqual(Vector3f(21.0f, 22.0f, 23.0f), box.getExtents());
		}

		TEST_METHOD(Offset)
		{
			OrientedBoundingBoxf box(0.0f, 1.0f, 2.0f, 10.0f, 11.0f, 12.0f);

			box.offset(10.0, 10.0, 10.0);
			Assert::AreEqual(Vector3f(10.0f, 11.0f, 12.0f), box.getCenter());
			Assert::AreEqual(Vector3f(10.0f, 11.0f, 12.0f), box.getExtents());

			box.offset(Vector3f::one());
			Assert::AreEqual(Vector3f(11.0f, 12.0f, 13.0f), box.getCenter());
			Assert::AreEqual(Vector3f(10.0f, 11.0f, 12.0f), box.getExtents());
		}

		TEST_METHOD(Cast)
		{
			OrientedBoundingBoxf box(0.5f, 1.2f, 2.3f, 10.2f, 11.1f, 12.06f);

			OrientedBoundingBoxi boxInt = box.cast<int>();

			Assert::AreEqual(Vector3i(0, 1, 2), boxInt.getCenter());
			Assert::AreEqual(Vector3i(10, 11, 12), boxInt.getExtents());
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

			Assert::IsTrue(plane1.isFacing(Vector3f(-0.5, 0.5, 0.0)));
			Assert::IsFalse(plane1.isFacing(Vector3f(0.5, 0.5, 0.0)));
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

			Assert::IsTrue(plane1.isParallel(plane2));
			Assert::IsFalse(plane1.isParallel(plane3));
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

	TEST_CLASS(Ray3UnitTests)
	{
	public:

		TEST_METHOD(Constructor)
		{
			Ray3d ray;
			Assert::AreEqual(Vector3d::zero(), ray.getOrigin());
			Assert::AreEqual(Vector3d::zero(), ray.getDirection());

			Ray3d ray2(Vector3d(1, 2, 3), Vector3d(4, 5, 6));
			Assert::AreEqual(Vector3d(1, 2, 3), ray2.getOrigin());
			Assert::AreEqual(Vector3d(4, 5, 6), ray2.getDirection());
		}

		TEST_METHOD(Accessors)
		{
			Ray3d ray;
			ray.setOrigin(Vector3d(1, 2, 3));
			ray.setDirection(Vector3d(4, 5, 6));
			Assert::AreEqual(Vector3d(1, 2, 3), ray.getOrigin());
			Assert::AreEqual(Vector3d(4, 5, 6), ray.getDirection());

			Ray3d ray2;
			ray2.set(Vector3d(1, 2, 3), Vector3d(4, 5, 6));
			Assert::AreEqual(Vector3d(1, 2, 3), ray2.getOrigin());
			Assert::AreEqual(Vector3d(4, 5, 6), ray2.getDirection());
		}

		TEST_METHOD(Transform)
		{
			Ray3d ray(Vector3d(1, 2, 3), Vector3d(4, 5, 6));

			Matrix4d matrix4 = Matrix4d::createTranslation(1, 2, 3);

			ray.transform(matrix4);
			Assert::AreEqual(Vector3d(2, 4, 6), ray.getOrigin());
			Assert::AreEqual(Vector3d(4, 5, 6), ray.getDirection());

			Matrix3d matrix3 = Matrix3d::createScale(1, 2, 3);

			ray.transform(matrix3);
			Assert::AreEqual(Vector3d(2, 8, 18), ray.getOrigin());
			Assert::AreEqual(Vector3d(4, 10, 18), ray.getDirection());
		}
	};

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

			Assert::IsTrue(rect.contains(1.5f, 2.5f));
			Assert::IsFalse(rect.contains(0.0f, 0.0f));
		}

		TEST_METHOD(ContainsRect)
		{
			Rectf rect(1.0f, 2.0f, 3.0f, 3.0f);
			Rectf inside(1.5f, 2.5f, 1.0f, 0.2f);
			Rectf outside(4.0f, 4.0f, 1.0f, 1.0f);
			Rectf partiallyInside(1.5f, 2.5f, 4.0f, 4.0f);

			Assert::IsTrue(rect.contains(inside));
			Assert::IsFalse(rect.contains(outside));
			Assert::IsFalse(rect.contains(partiallyInside));
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
			Assert::IsTrue(rect.isEmpty());
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

			Assert::IsTrue(rect.intersects(inside));
			Assert::IsFalse(rect.intersects(outside));
			Assert::IsTrue(rect.intersects(partiallyInside));
		}

		TEST_METHOD(IsEmpty)
		{
			Rectf rect1(1.0f, 2.0f, 0.0f, 1.0f);
			Rectf rect2(1.5f, 2.5f, 1.0f, 0.2f);
			
			Assert::IsTrue(rect1.isEmpty());
			Assert::IsFalse(rect2.isEmpty());
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

			Assert::IsTrue(ellipsoid.contains(1.5f, 1.5f, 1.5f));
			Assert::IsFalse(ellipsoid.contains(3.0f, 3.0f, 3.0f));
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
			Assert::IsTrue(false);
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

			Assert::IsTrue(v1 == v2);
		}

		TEST_METHOD(OperatorNotEqual)
		{
			Vector2f v1(2.0f, 1.0f);
			Vector2f v2(4.0f, 2.3f);

			Assert::IsTrue(v1 != v2);
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

			Assert::IsTrue(v2 < v1);
		}

		TEST_METHOD(LessThanOneCompGreater)
		{
			Vector2f v1(2.0f, 1.0f);
			Vector2f v2(4.0f, 0.5f);

			Assert::IsFalse(v2 < v1);
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
			Assert::IsTrue(false);
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
			Assert::IsTrue(false);
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

			Assert::IsTrue(v1 == v2);
		}

		TEST_METHOD(OperatorNotEqual)
		{
			Vector3f v1(2.0f, 1.0f, 2.2f);
			Vector3f v2(4.0f, 2.3f, 2.2f);

			Assert::IsTrue(v1 != v2);
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

			Assert::IsTrue(v2 < v1);
		}

		TEST_METHOD(LessThanOneCompGreater)
		{
			Vector3f v1(2.0f, 1.0f, 2.0f);
			Vector3f v2(4.0f, 0.5f, 1.0f);

			Assert::IsFalse(v2 < v1);
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
			Assert::IsTrue(false);
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
			Assert::IsTrue(false);
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

			Assert::IsTrue(v1 == v2);
		}

		TEST_METHOD(OperatorNotEqual)
		{
			Vector4f v1(2.0f, 1.0f, 2.2f, 3.3f);
			Vector4f v2(4.0f, 2.3f, 2.2f, 3.2f);

			Assert::IsTrue(v1 != v2);
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

			Assert::IsTrue(v2 < v1);
		}

		TEST_METHOD(LessThanOneCompGreater)
		{
			Vector4f v1(2.0f, 1.0f, 2.0f, 3.0f);
			Vector4f v2(4.0f, 0.5f, 1.0f, 2.0f);

			Assert::IsFalse(v2 < v1);
		}
	};
}