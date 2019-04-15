#include "Triangle.h"

namespace Arcana
{
	namespace FTL
	{

		Triangle::Triangle() : v1(Vector3d::zero()), type(Diffusive), parent(nullptr)
		{
			calculateNormal();
		}

		Triangle::Triangle(Vector3d a, Vector3d b, Vector3d c, SurfaceType type, double reflection, Color surfaceColor, Color emissiveColor)
			: v1(a), edge1(b - a), edge2(c - a), type(type), reflection(reflection), surfaceColor(surfaceColor), emissiveColor(emissiveColor),
			parent(nullptr)
		{
			calculateNormal();
		}

		Triangle::Triangle(Vector3d a, Vector3d b, Vector3d c, SurfaceType type, double reflection, 
			Vector2f auv, Vector2f buv, Vector2f cuv, Color surfaceColor, Color emissiveColor)
			: v1(a), edge1(b - a), edge2(c - a), type(type), reflection(reflection), 
			auv(auv), buv(buv), cuv(cuv), surfaceColor(surfaceColor), emissiveColor(emissiveColor),
			parent(nullptr)
		{
			calculateNormal();
		}

		Triangle::~Triangle()
		{
		}

		void Triangle::calculateNormal()
		{
			normal = Vector3d::normalize(Vector3d::cross(edge2, edge1));
		}

		bool Triangle::getIntersection(const Ray& ray, double& t) const
		{
			Vector3d pVec = Vector3d::cross(ray.direction, edge2);
			double det = Vector3d::dot(edge1, pVec);

			if (det > -Math::EPSILON && det < Math::EPSILON)
			{
				return false;
			}

			double invDet = 1.0 / det;
			Vector3d tVec = ray.origin - v1;
			double u = Vector3d::dot(tVec, pVec) * invDet;

			if (u < 0.0 || u > 1.0)
			{
				return false;
			}

			Vector3d qVec = Vector3d::cross(tVec, edge1);
			float v = Vector3d::dot(ray.direction, qVec) * invDet;

			if (v < 0.0 || v + u > 1.0)
			{
				return false;
			}

			t = Vector3d::dot(edge2, qVec) * invDet;

			if (t > Math::EPSILON) 
			{
				return true;
			}

			return false;
		}
	}
}
