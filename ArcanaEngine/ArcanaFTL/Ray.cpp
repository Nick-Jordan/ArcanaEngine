#include "Ray.h"

namespace Arcana
{
	namespace FTL
	{

		Ray::Ray() : origin(Vector3d::zero()), direction(Vector3d::zero()), length(0.0)
		{
		}

		Ray::Ray(const Vector3d& origin, const Vector3d& direction, Color color, double length)
			: origin(origin), direction(direction), color(color), length(length)
		{

		}

		Ray::~Ray()
		{
		}

		Ray Ray::reflect(const Vector3d& axis)
		{
			Vector3d proj = axis * Vector3d::dot(direction, axis);
			Vector3d step = proj - direction;
			return Ray(origin, direction + step * 2, color);
		}
	}
}
