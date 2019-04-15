#ifndef FTL_PHOTON_MAP_H_
#define FTL_PHOTON_MAP_H_

#include "FTLDefines.h"

#include "Vector3.h"
#include "Color.h"
#include "Sphere.h"
#include "AxisAlignedBoundingBox.h"
#include "Ray.h"
#include "Triangle.h"

namespace Arcana
{
	namespace FTL
	{
		struct ARCANA_FTL_API Photon
		{
			Vector3d position;
			Vector3d previousPosition;
			Vector3d energy;
			int16 plane;
			uint8 theta, phi;
		};

		struct ARCANA_FTL_API NearestPhotons
		{
			int32 max;
			int32 found;
			int32 heap;
			Vector3d position;
			double* dist2;
			const Photon** index;
		};

		class ARCANA_FTL_API PhotonMap
		{
		public:

			PhotonMap(uint32 maxPhotons);

			~PhotonMap();

			void store(const Vector3d& energy, const Vector3d& position, const Vector3d& direction, const Vector3d& prevPosition);

			void scalePhotonEnergy(const double scale);

			void balance();

			Vector3d estimateIrradiance(const Vector3d& position, const Vector3d& normal, const double maxDistance, const int32 numPhotons) const;

			void locatePhotons(NearestPhotons* np, const int32 index)  const;

			Vector3d photonDirection(const Photon* p) const;

		public:

			int32 storedPhotons;
			int32 maxPhotons;
			Photon* photons;

		private:

			void balanceSegment(Photon** balancedPhotons, Photon** originalPhotons, const int32 index, const int32 start, const int32 end);

			void medianSplit(Photon** photons, const int32 start, const int32 end, const int32 median, const int32 axis);

			template<typename T>
			static T getComponent(const Vector3<T>& vec, int32 i)
			{
				if (i == 0)
					return vec.x;
				if (i == 1)
					return vec.y;
				if (i == 2)
					return vec.z;

				return (T)0;
			}

			int32 halfStoredPhotons;

			int32 previousScale;

			double costheta[256];
			double sintheta[256];
			double cosphi[256];
			double sinphi[256];

			AxisAlignedBoundingBoxf bounds;

		public:

			static void photonTracing(
				PhotonMap& map,
				const Ray& ray,
				std::vector<Triangle>& triangles,
				LinearColor color,
				int32 depth,
				float RI,
				int32 maxLightBounces);

			static void photonTracingCaustics(
				PhotonMap& map,
				const Ray& ray,
				std::vector<Triangle>& triangles,
				LinearColor color,
				int32 depth,
				bool flag,
				float RI,
				int32 maxLightBounces,
				std::vector<Sphered>& bound);
		};

	}
}

#endif // !FTL_PHOTON_MAP_H_

