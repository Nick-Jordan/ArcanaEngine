#include "Triangle.h"

namespace Arcana
{
	namespace FTL
	{

		Triangle::Triangle() : v1(Vector3d::zero()), type(SurfaceType::Diffusive), parent(nullptr)
		{
		}

		Triangle::Triangle(Vector3d a, Vector3d b, Vector3d c, Vector3d anorm, Vector3d bnorm, Vector3d cnorm,
			SurfaceType type, double reflection, Color surfaceColor, Color emissiveColor)
			: v1(a), edge1(b - a), edge2(c - a), anorm(anorm), bnorm(bnorm), cnorm(cnorm),
			type(type), reflection(reflection), surfaceColor(surfaceColor), emissiveColor(emissiveColor),
			parent(nullptr)
		{
		}

		Triangle::Triangle(Vector3d a, Vector3d b, Vector3d c, Vector3d anorm, Vector3d bnorm, Vector3d cnorm,
			Vector2f auv, Vector2f buv, Vector2f cuv,
			SurfaceType type, double reflection,
			Color surfaceColor, Color emissiveColor)
			: v1(a), edge1(b - a), edge2(c - a), type(type), reflection(reflection),
			auv(auv), buv(buv), cuv(cuv), anorm(anorm), bnorm(bnorm), cnorm(cnorm),
			surfaceColor(surfaceColor), emissiveColor(emissiveColor),
			parent(nullptr)
		{
		}

		Triangle::~Triangle()
		{
		}

		bool Triangle::getIntersection(const Ray& ray, HitResult& hit) const
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
			double v = Vector3d::dot(ray.direction, qVec) * invDet;

			if (v < 0.0 || v + u > 1.0)
			{
				return false;
			}

			double t = Vector3d::dot(edge2, qVec) * invDet;

			if (t >= hit.t)
			{
				return false;
			}

			if (t > Math::EPSILON)
			{
				hit.t = t;
				hit.normal = getNormal(1.0 - u - v, u, v);
				return true;
			}

			return false;
		}

		Vector3d Triangle::getNormal(double a, double b, double c) const
		{
			return Vector3d::normalize(a * anorm + b * bnorm + c * cnorm);
		}

		Vector3d Triangle::getPosition(double a, double b, double c) const
		{
			return a * v1 + b * (v1 + edge1) + c * (v1 + edge2);
		}
	}
}
