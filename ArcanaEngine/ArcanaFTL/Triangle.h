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
		enum SurfaceType
		{
			Transparent,
			Diffusive,
			PureSpecular,
			LightSource
		};

		class ARCANA_FTL_API Triangle
		{
		public:

			Triangle();

			Triangle(Vector3d a, Vector3d b, Vector3d c, SurfaceType type, double reflection, Color surfaceColor = Color(), Color emissiveColor = Color());

			Triangle(Vector3d a, Vector3d b, Vector3d c, SurfaceType type, double reflection, 
				Vector2f auv, Vector2f buv, Vector2f cuv, Color surfaceColor = Color(), Color emissiveColor = Color());

			~Triangle();

			bool getIntersection(const Ray& ray, double& t) const;

		private:

			void calculateNormal();

		public:

			StaticMeshComponent* parent;

			Vector3d v1;
			Vector3d edge1;
			Vector3d edge2;
			Vector3d normal;
			Color surfaceColor;
			Color emissiveColor;
			double reflection;
			SurfaceType type;

			Vector2f auv;
			Vector2f buv;
			Vector2f cuv;
		};
	}
}

#endif // !FTL_TRIANGLE_H_

