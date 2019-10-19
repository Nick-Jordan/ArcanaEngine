#ifndef INTERSECTOR_H_
#define INTERSECTOR_H_

#include "Sphere.h"
#include "Ray3.h"
#include "AxisAlignedBoundingBox.h"
#include "OrientedBoundingBox.h"
#include "Frustum.h"
#include "Plane.h"

namespace Arcana
{
	template<typename T>
	struct IntersectionResult
	{
		T t0, t1;
		//T distance;//need these?
		//T penetrationDepth;//need these?

		Vector3<T> normal;
		Vector3<T> point;

		IntersectionResult() : t0((T)0), t1((T)0),// distance((T)0), penetrationDepth((T)0),
			normal((T)0, (T)0, (T)0), point((T)0, (T)0, (T)0) {}
	};

	const int PLANE_INTERSECTS = 0;
	const int PLANE_INTERSECTS_FRONT = 1;
	const int PLANE_INTERSECTS_BACK = -1;

	template<typename T>
	class Intersector
	{
	public:

		static bool intersect(const Sphere<T>& a, const Sphere<T>& b);

		static bool intersect(const Sphere<T>& sphere, const Ray3<T>& ray, IntersectionResult<T>& result = IntersectionResult<T>());

		static bool intersect(const Sphere<T>& sphere, const AABB<T>& aabb);

		static bool intersect(const Sphere<T>& sphere, const OBB<T>& obb);

		static bool intersect(const Sphere<T>& sphere, const Frustum<T>& frustum);

		static int intersect(const Sphere<T>& sphere, const Plane<T>& plane);

		static bool intersect(const Ray3<T>& ray, const AABB<T>& aabb, IntersectionResult<T>& result = IntersectionResult<T>());

		static bool intersect(const Ray3<T>& ray, const OBB<T>& obb, IntersectionResult<T>& result = IntersectionResult<T>());

		static bool intersect(const Ray3<T>& ray, const Frustum<T>& frustum, IntersectionResult<T>& result = IntersectionResult<T>());

		static bool intersect(const Ray3<T>& ray, const Plane<T>& plane, IntersectionResult<T>& result = IntersectionResult<T>());

		static bool intersect(const AABB<T>& a, const AABB<T>& b);

		static bool intersect(const AABB<T>& aabb, const OBB<T>& obb);

		static bool intersect(const AABB<T>& aabb, const Frustum<T>& frustum);

		static int intersect(const AABB<T>& aabb, const Plane<T>& plane);

		static bool intersect(const OBB<T>& a, const OBB<T>& b);

		static bool intersect(const OBB<T>& obb, const Frustum<T>& frustum);

		static bool intersect(const OBB<T>& obb, const Plane<T>& plane);

		static bool intersect(const Frustum<T>& a, const Frustum<T>& b);

		static int intersect(const Frustum<T>& frustum, const Plane<T>& plane);

		static int intersect(const Plane<T>& a, const Plane<T>& b);

	private:

		static bool solveQuadratic(const T& a, const T& b, const T& c, T& x0, T& x1);
	};
}

#include "Intersector.inl"

#endif // !INTERSECTOR_H_