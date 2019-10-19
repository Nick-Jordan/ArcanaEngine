
namespace Arcana
{
	template<typename T>
	bool Intersector<T>::intersect(const Sphere<T>& a, const Sphere<T>& b)
	{
		T distanceSq = Vector3<T>::distanceSq(a.getCenter(), b.getCenter());

		T radiiSq = Math::square(a.getRadius() + b.getRadius());

		return distanceSq <= radiiSq;
	}

	template<typename T>
	bool Intersector<T>::intersect(const Sphere<T>& sphere, const Ray3<T>& ray, IntersectionResult<T>& result)
	{
		Vector3<T> L = ray.getOrigin() - sphere.getCenter();
		T a = Vector3<T>::dot(ray.getDirection(), ray.getDirection());
		T b = (T)2 * Vector3<T>::dot(ray.getDirection(), L);
		T c = Vector3<T>::dot(L, L) - sphere.getRadius() * sphere.getRadius();
		if (!solveQuadratic(a, b, c, result.t0, result.t1))
		{
			return false;
		}
 
		if (result.t0 > result.t1)
		{
			std::swap(result.t0, result.t1);
		}

		if (result.t0 < 0)
		{

			result.t0 = result.t1;
			if (result.t0 < 0)
			{
				return false;
			}
		}

		result.point = ray.getOrigin() + result.t0 * ray.getDirection();
		result.normal = result.point - sphere.getCenter();
		result.normal.normalize();

		return true;
	}

	template<typename T>
	bool Intersector<T>::intersect(const Sphere<T>& sphere, const AABB<T>& aabb)
	{
		Vector3<T> cp = sphere.getCenter();

		const Vector3<T>& boxMin = aabb.getMin();
		const Vector3<T>& boxMax = aabb.getMax();

		if (sphere.getCenter().x < boxMin.x)
		{
			cp.x = boxMin.x;
		}
		else if (sphere.getCenter().x > boxMax.x)
		{
			cp.x = boxMax.x;
		}

		if (sphere.getCenter().y < boxMin.y)
		{
			cp.y = boxMin.y;
		}
		else if (sphere.getCenter().y > boxMax.y)
		{
			cp.y = boxMax.y;
		}

		if (sphere.getCenter().z < boxMin.z)
		{
			cp.z = boxMin.z;
		}
		else if (sphere.getCenter().z > boxMax.z)
		{
			cp.z = boxMax.z;
		}

		cp -= sphere.getCenter();

		return cp.magnitudeSq() <= Math::square(sphere.getRadius());
	}

	template<typename T>
	bool Intersector<T>::intersect(const Sphere<T>& sphere, const OBB<T>& obb)
	{
		//////NOT WORKING
		Vector3<T> p = obb.getRotation().inverse().rotate(sphere.getCenter());
		Sphere<T> s = sphere;
		AABB<T> aabb(obb.getCenter() - obb.getExtents(), obb.getCenter() + obb.getExtents());
		s.setCenter(p);
		return intersect(s, aabb);
	}

	template<typename T>
	bool Intersector<T>::intersect(const Sphere<T>& sphere, const Frustum<T>& frustum)
	{
		return (intersect(sphere, frustum.getNear()) != PLANE_INTERSECTS_BACK &&
			intersect(sphere, frustum.getFar()) != PLANE_INTERSECTS_BACK &&
			intersect(sphere, frustum.getLeft()) != PLANE_INTERSECTS_BACK &&
			intersect(sphere, frustum.getRight()) != PLANE_INTERSECTS_BACK &&
			intersect(sphere, frustum.getBottom()) != PLANE_INTERSECTS_BACK &&
			intersect(sphere, frustum.getTop()) != PLANE_INTERSECTS_BACK);
	}

	template<typename T>
	int Intersector<T>::intersect(const Sphere<T>& sphere, const Plane<T>& plane)
	{
		T distance = plane.signedDistance(sphere.getCenter());

		if (Math::abs(distance) <= sphere.getRadius())
		{
			return PLANE_INTERSECTS;
		}
		else if (distance > (T)0)
		{
			return PLANE_INTERSECTS_FRONT;
		}

		return PLANE_INTERSECTS_BACK;
	}

	template<typename T>
	bool Intersector<T>::intersect(const Ray3<T>& ray, const AABB<T>& aabb, IntersectionResult<T>& result)
	{
		T dnear = (T)0;
		T dfar = (T)0;
		T tmin = (T)0;
		T tmax = (T)0;

		const Vector3<T>& origin = ray.getOrigin();
		const Vector3<T>& direction = ray.getDirection();

		T div = (T)1 / direction.x;
		if (div >= (T)0)
		{
			tmin = (aabb.getMin().x - origin.x) * div;
			tmax = (aabb.getMax().x - origin.x) * div;
		}
		else
		{
			tmin = (aabb.getMax().x - origin.x) * div;
			tmax = (aabb.getMin().x - origin.x) * div;
		}
		dnear = tmin;
		dfar = tmax;

		if (dnear > dfar || dfar < (T)0)
		{
			return false;
		}

		div = (T)1 / direction.y;
		if (div >= (T)0)
		{
			tmin = (aabb.getMin().y - origin.y) * div;
			tmax = (aabb.getMax().y - origin.y) * div;
		}
		else
		{
			tmin = (aabb.getMax().y - origin.y) * div;
			tmax = (aabb.getMin().y - origin.y) * div;
		}

		if (tmin > dnear)
		{
			dnear = tmin;
		}
		if (tmax < dfar)
		{
			dfar = tmax;
		}

		if (dnear > dfar || dfar < (T)0)
		{
			return false;
		}

		div = (T)1 / direction.z;
		if (div >= (T)0)
		{
			tmin = (aabb.getMin().z - origin.z) * div;
			tmax = (aabb.getMax().z - origin.z) * div;
		}
		else
		{
			tmin = (aabb.getMax().z - origin.z) * div;
			tmax = (aabb.getMin().z - origin.z) * div;
		}

		if (tmin > dnear)
		{
			dnear = tmin;
		}
		if (tmax < dfar)
		{
			dfar = tmax;
		}

		if (dnear > dfar || dfar < (T)0)
		{
			return false;
		}


		result.t0 = dnear;
		result.t1 = dfar;
		//result.normal
		result.point = origin + result.t0 * direction;
		return true;
	}

	template<typename T>
	bool Intersector<T>::intersect(const Ray3<T>& ray, const OBB<T>& obb, IntersectionResult<T>& result)
	{
		///obb
		return false;
	}

	template<typename T>
	bool Intersector<T>::intersect(const Ray3<T>& ray, const Frustum<T>& frustum, IntersectionResult<T>& result)
	{
		Plane<T> n = frustum.getNear();
		T nD = intersect(ray, n);
		T nOD = n.distance(ray.getOrigin());

		Plane<T> f = frustum.getFar();
		T fD = intersect(ray, f);
		T fOD = f.distance(ray.getOrigin());

		Plane<T> l = frustum.getLeft();
		T lD = intersect(ray, l);
		T lOD = l.distance(ray.getOrigin());

		Plane<T> r = frustum.getRight();
		T rD = intersect(ray, r);
		T rOD = r.distance(ray.getOrigin());

		Plane<T> b = frustum.getBottom();
		T bD = intersect(ray, b);
		T bOD = b.distance(ray.getOrigin());

		Plane<T> t = frustum.getTop();
		T tD = intersect(ray, t);
		T tOD = t.distance(ray.getOrigin());

		if ((nOD < (T)0 && nD < (T)0) || (fOD < (T)0 && fD < (T)0) ||
			(lOD < (T)0 && lD < (T)0) || (rOD < (T)0 && rD < (T)0) ||
			(bOD < (T)0 && bD < (T)0) || (tOD < (T)0 && tD < (T)0))
		{
			return false;
		}

		T d = (nD > (T)0) ? nD : (T)0;
		d = (fD > (T)0) ? ((d == (T)0) ? fD : std::min(fD, d)) : d;
		d = (lD > (T)0) ? ((d == (T)0) ? lD : std::min(lD, d)) : d;
		d = (rD > (T)0) ? ((d == (T)0) ? rD : std::min(rD, d)) : d;
		d = (tD > (T)0) ? ((d == (T)0) ? bD : std::min(bD, d)) : d;
		d = (bD > (T)0) ? ((d == (T)0) ? tD : std::min(tD, d)) : d;

		result.t0 = d;
		result.point = ray.getOrigin() + d * ray.getDirection();

		return true;
	}

	template<typename T>
	bool Intersector<T>::intersect(const Ray3<T>& ray, const Plane<T>& plane, IntersectionResult<T>& result)
	{
		T alpha = Vector3<T>::dot(ray.getOrigin(), plane.getNormal()) + plane.getDistance();
		if (fabs(alpha) < Math::EPSILON)
		{
			return false;
		}

		T dot = Vector3<T>::dot(ray.getDirection(), plane.getNormal());

		if (dot == (T)0)
		{
			return false;
		}

		T d = -alpha / dot;
		if (d < (T)0)
		{
			return false;
		}

		result.t0 = d;
		result.point = ray.getOrigin() + d * ray.getDirection();
		result.normal = plane.getNormal();

		return true;
	}

	template<typename T>
	bool Intersector<T>::intersect(const AABB<T>& a, const AABB<T>& b)
	{
		return ((a.getMin().x >= b.getMin().x && a.getMin().x <= b.getMax().x) || (b.getMin().x >= a.getMin().x && b.getMin().x <= a.getMax().x)) &&
			((a.getMin().y >= b.getMin().y && a.getMin().y <= b.getMax().y) || (b.getMin().y >= a.getMin().y && b.getMin().y <= a.getMax().y)) &&
			((a.getMin().z >= b.getMin().z && a.getMin().z <= b.getMax().z) || (b.getMin().z >= a.getMin().z && b.getMin().z <= a.getMax().z));
	}

	template<typename T>
	bool Intersector<T>::intersect(const AABB<T>& aabb, const OBB<T>& obb)
	{
		//obb
		return false;
	}

	template<typename T>
	bool Intersector<T>::intersect(const AABB<T>& aabb, const Frustum<T>& frustum)
	{
		return (intersect(aabb, frustum.getNear()) != PLANE_INTERSECTS_BACK &&
			intersect(aabb, frustum.getFar()) != PLANE_INTERSECTS_BACK &&
			intersect(aabb, frustum.getLeft()) != PLANE_INTERSECTS_BACK &&
			intersect(aabb, frustum.getRight()) != PLANE_INTERSECTS_BACK &&
			intersect(aabb, frustum.getBottom()) != PLANE_INTERSECTS_BACK &&
			intersect(aabb, frustum.getTop()) != PLANE_INTERSECTS_BACK);
	}

	template<typename T>
	int Intersector<T>::intersect(const AABB<T>& aabb, const Plane<T>& plane)
	{
		Vector3<T> center((aabb.getMin().x + aabb.getMax().x) / (T)2, 
			(aabb.getMin().y + aabb.getMax().y) / (T)2,
			(aabb.getMin().z + aabb.getMax().z) / (T)2);
		T distance = plane.distance(center);

		T extentX = (aabb.getMax().x - aabb.getMin().x) / (T)2;
		T extentY = (aabb.getMax().y - aabb.getMin().y) / (T)2;
		T extentZ = (aabb.getMax().z - aabb.getMin().z) / (T)2;

		const Vector3<T>& planeNormal = plane.getNormal();
		if (Math::abs(distance) <= (Math::abs(extentX * planeNormal.x) + Math::abs(extentY * planeNormal.y) + Math::abs(extentZ * planeNormal.z)))
		{
			return PLANE_INTERSECTS;
		}

		return (distance > (T)0) ? PLANE_INTERSECTS_FRONT : PLANE_INTERSECTS_BACK;
	}

	template<typename T>
	bool Intersector<T>::intersect(const OBB<T>& a, const OBB<T>& b)
	{
		//obb
		return false;
	}

	template<typename T>
	bool Intersector<T>::intersect(const OBB<T>& obb, const Frustum<T>& frustum)
	{
		//obb
		return false;
	}

	template<typename T>
	bool Intersector<T>::intersect(const OBB<T>& obb, const Plane<T>& plane)
	{
		//obb
		return false;
	}

	template<typename T>
	bool Intersector<T>::intersect(const Frustum<T>& a, const Frustum<T>& b)
	{
		///frustum-frustum
		return false;
	}

	template<typename T>
	int Intersector<T>::intersect(const Frustum<T>& frustum, const Plane<T>& plane)
	{
		std::vector<Vector3<T>> corners;
		frustum.getCorners(corners);

		T d = plane.signedDistance(corners[0]);
		if (d > (T)0)
		{
			if (plane.signedDistance(corners[1]) <= (T) 0 ||
				plane.signedDistance(corners[2]) <= (T) 0 ||
				plane.signedDistance(corners[3]) <= (T) 0 ||
				plane.signedDistance(corners[4]) <= (T) 0 ||
				plane.signedDistance(corners[5]) <= (T) 0 ||
				plane.signedDistance(corners[6]) <= (T) 0 ||
				plane.signedDistance(corners[7]) <= (T) 0)
			{
				return PLANE_INTERSECTS;
			}

			return PLANE_INTERSECTS_FRONT;
		}
		else if (d < (T) 0)
		{
			if (plane.signedDistance(corners[1]) >= (T) 0 ||
				plane.signedDistance(corners[2]) >= (T) 0 ||
				plane.signedDistance(corners[3]) >= (T) 0 ||
				plane.signedDistance(corners[4]) >= (T) 0 ||
				plane.signedDistance(corners[5]) >= (T) 0 ||
				plane.signedDistance(corners[6]) >= (T) 0 ||
				plane.signedDistance(corners[7]) >= (T) 0)
			{
				return PLANE_INTERSECTS;
			}

			return PLANE_INTERSECTS_BACK;
		}

		return PLANE_INTERSECTS;
	}

	template<typename T>
	int Intersector<T>::intersect(const Plane<T>& a, const Plane<T>& b)
	{
		if ((a.getNormal().x == b.getNormal().x && a.getNormal().y == b.getNormal().y && a.getNormal().z == b.getNormal().z) || !a.isParallel(b))
		{
			return PLANE_INTERSECTS;
		}

		Vector3<T> point(b.getNormal().x * -b.getDistance(), b.getNormal().y * -b.getDistance(), b.getNormal().z * -b.getDistance());

		if (a.signedDistance(point) > (T)0)
		{
			return PLANE_INTERSECTS_FRONT;
		}
		
		return PLANE_INTERSECTS_BACK;
	}

	template<typename T>
	bool Intersector<T>::solveQuadratic(const T& a, const T& b, const T& c, T& x0, T& x1)
	{
		T discr = b * b - (T)4 * a * c;
		if (discr < 0)
		{
			return false;
		}
		else if (discr == 0)
		{
			x0 = x1 = -(b / a) / (T)2;
		}
		else 
		{
			T q = (b > 0) ?
				-(b + sqrt(discr)) / (T)2 :
				-(b - sqrt(discr)) / (T)2;
			x0 = q / a;
			x1 = c / q;
		}

		if (x0 > x1)
		{
			std::swap(x0, x1);
		}

		return true;
	}
}