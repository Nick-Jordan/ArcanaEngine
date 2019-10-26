#ifndef FTL_TRIANGLE_H_
#define FTL_TRIANGLE_H_

#include "FTLDefines.h"

#include "Ray.h"
#include "Color.h"

#include "StaticMeshComponent.h"

namespace Arcana
{
	namespace FTL
	{
		enum class SurfaceType
		{
			Transparent,
			Diffusive,
			PureSpecular,
			LightSource
		};

		struct ARCANA_FTL_API HitResult
		{
			double t;
			Vector3d normal;

			HitResult() : t(10000000000.0) {}
		};

		class ARCANA_FTL_API Triangle
		{
		public:

			Triangle();

			Triangle(Vector3d a, Vector3d b, Vector3d c, Vector3d anorm, Vector3d bnorm, Vector3d cnorm,
				SurfaceType type, double reflection, Color surfaceColor = Color(), Color emissiveColor = Color());

			Triangle(Vector3d a, Vector3d b, Vector3d c, Vector3d anorm, Vector3d bnorm, Vector3d cnorm,
				Vector2f auv, Vector2f buv, Vector2f cuv,
				SurfaceType type, double reflection, 
				Color surfaceColor = Color(), Color emissiveColor = Color());

			~Triangle();

			bool getIntersection(const Ray& ray, HitResult& hit) const;

			Vector3d getNormal(double a, double b, double c) const;

			Vector3d getPosition(double a, double b, double c) const;

		public:

			StaticMeshComponent* parent;

			Vector3d v1;
			Vector3d edge1;
			Vector3d edge2;
			Color surfaceColor;
			Color emissiveColor;
			double reflection;
			SurfaceType type;

			Vector2f auv;
			Vector2f buv;
			Vector2f cuv;

			Vector3d anorm;
			Vector3d bnorm;
			Vector3d cnorm;
		};
	}
}

#endif // !FTL_TRIANGLE_H_

