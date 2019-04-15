#ifndef FTL_RAY_H_
#define FTL_RAY_H_

#include "FTLDefines.h"

#include "Vector3.h"
#include "Color.h"

namespace Arcana
{
	namespace FTL
	{
		class Ray
		{
		public:

			Ray();
			Ray(const Vector3d& origin, const Vector3d& direction, Color color = Color(), double length = 0.0);
			~Ray();

			Ray reflect(const Vector3d& axis);

		public:

			Vector3d origin;
			Vector3d direction;
			Color color;
			double length;
		};
	}
}

#endif // !FTL_RAY_H_

