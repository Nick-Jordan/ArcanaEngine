#include "RayTracer.h"

#include "Random.h"
#include "ArcanaLog.h"

namespace Arcana
{
	namespace FTL
	{
		RayTracer::RayTracer(const std::vector<Triangle>& triangles, const std::vector<PointLight>& lights) : _triangles(triangles)
		{

		}

		RayTracer::~RayTracer()
		{

		}

		LinearColor RayTracer::irradiance(
			PhotonMap& photonMap,
			const Ray& ray,
			const std::vector<Triangle>& triangleList,
			int32 bounce,
			int32 maxLightBounces,
			float refractionIndex)
		{
			if (bounce > maxLightBounces)
			{
				return LinearColor(0.0f, 0.0f, 0.0f);
			}

			HitResult hit;
			int32 id = 0;

			if (!intersect(ray, triangleList, hit, id))
			{
				return LinearColor(0.0f, 0.0f, 0.0f);
			}

			const Triangle& obj = triangleList[id];

			Vector3d origin_new = ray.origin + ray.direction * hit.t;
			Vector3d n = hit.normal;
			Vector3d nl = Vector3d::dot(n, ray.direction) < 0 ? n : n * -1;
			LinearColor f = obj.surfaceColor;

			if (obj.type == SurfaceType::Diffusive)
			{
				LinearColor color(0, 0, 0);

				Vector3d c = photonMap.estimateIrradiance(origin_new, n, 0.1, 100);
				color.R = c.x;
				color.G = c.y;
				color.B = c.z;
				return color;
			}
			else if (obj.type == SurfaceType::PureSpecular)
			{
				Ray rayNew;
				rayNew.origin = origin_new;
				rayNew.direction = ray.direction - n * 2 * Vector3d::dot(n, ray.direction);
				return f * irradiance(photonMap, rayNew, triangleList, bounce + 1, maxLightBounces, refractionIndex);
			}
			else if (obj.type == SurfaceType::Transparent)
			{
				Ray reflRay;
				reflRay.origin = origin_new;
				reflRay.direction = ray.direction - n * 2 * Vector3d::dot(n, ray.direction);
				bool into = Vector3d::dot(n, nl) > 0; 
				float nc = 1.0f;
				float nt = refractionIndex;
				float nnt = into ? nc / nt : nt / nc;
				float ddn = Vector3d::dot(ray.direction, nl);
				float cos2t;
				if ((cos2t = 1.0 - nnt * nnt * (1 - ddn * ddn)) < 0)
				{
					return f * irradiance(photonMap, reflRay, triangleList, bounce + 1, maxLightBounces, refractionIndex);
				}

				Vector3d tdir = Vector3d::normalize(ray.direction * nnt - n * ((into ? 1 : -1) * (ddn * nnt + sqrt(cos2t))));
				Ray tdirRay;
				tdirRay.origin = origin_new;
				tdirRay.direction = tdir;
				float a = nt - nc;
				float b = nt + nc;
				float R0 = a * a / (b * b);
				float c = 1.0f - (into ? -ddn : Vector3d::dot(tdir, n));
				float Re = R0 + (1 - R0) * c * c * c * c * c;
				float Tr = 1 - Re;
				return obj.emissiveColor.asLinear() + f * (irradiance(photonMap, reflRay, triangleList, bounce + 1, maxLightBounces, refractionIndex) * Re
					+ irradiance(photonMap, tdirRay, triangleList, bounce + 1, maxLightBounces, refractionIndex) * Tr);
			}

			return LinearColor(0.0f, 0.0f, 0.0f);
		}

		LinearColor RayTracer::raytrace(
			PhotonMap& photonMap,
			PhotonMap& photonMapCaustic,
			const Ray& ray,
			const std::vector<Triangle>& triangleList,
			int32 bounce,
			int32 maxLightBounces,
			float refractionIndex
		)
		{
			if (bounce > maxLightBounces)
			{
				return LinearColor(0.0f, 0.0f, 0.0f);
			}

			HitResult hit;
			int32 id = 0; 

			if (!intersect(ray, triangleList, hit, id))
			{
				return LinearColor(0.0f, 0.0f, 0.0f); // if miss, return black
			}

			const Triangle& obj = triangleList[id];        // the hit object

			Vector3d origin_new = ray.origin + ray.direction * hit.t;
			Vector3d n = hit.normal;
			Vector3d nl = Vector3d::dot(n, ray.direction) < 0 ? n : n * -1;
			LinearColor f = obj.surfaceColor.asLinear();

			if (obj.type == SurfaceType::LightSource)
			{
				return obj.emissiveColor.asLinear();
			}
			else if (obj.type == SurfaceType::Diffusive)
			{
				LinearColor color(0, 0, 0);

				Vector3d c = photonMapCaustic.estimateIrradiance(origin_new, n, 0.1, 100);
				color.R = c.x;
				color.G = c.y;
				color.B = c.z;

				Ray rayNew;
				rayNew.origin = origin_new;

				int32 nsamps = 200;
				for (int32 i = 0; i < nsamps; i++)
				{
					double r1 = Random<double>::random(0, Math::TWO_PI);
					double r2 = Random<double>::random(0, 1);
					double r2s = sqrt(r2);
					Vector3d w = nl;
					Vector3d u = Vector3d::normalize(Vector3d::cross((fabs(w.x) > .1 ? Vector3d(0, 1, 0) : Vector3d(1, 0, 0)), w));
					Vector3d v = Vector3d::cross(w, u);
					Vector3d d = Vector3d::normalize(u * cos(r1) * r2s + v * sin(r1) * r2s + w * sqrt(1 - r2));
					rayNew.direction = d;
					color = color + irradiance(photonMap, rayNew, triangleList, 0, maxLightBounces, refractionIndex) * (1.0f / (float)nsamps);
				}
				
				//if (obj.token == 'G')
				{
					rayNew.direction = ray.direction - n * 2 * Vector3d::dot(n, ray.direction);
					color = color + f * raytrace(photonMap, photonMapCaustic, rayNew, triangleList, bounce + 1, maxLightBounces, refractionIndex);
				}

				return color;

			}
			else if (obj.type == SurfaceType::PureSpecular)
			{
				Ray rayNew;
				rayNew.origin = origin_new;
				rayNew.direction = ray.direction - n * 2 * Vector3d::dot(n, ray.direction);
				return f * raytrace(photonMap, photonMapCaustic, rayNew, triangleList, bounce + 1, maxLightBounces, refractionIndex);
			}

			Ray reflRay;
			reflRay.origin = origin_new;
			reflRay.direction = ray.direction - n * 2 * Vector3d::dot(n, ray.direction);
			bool into = Vector3d::dot(n, nl) > 0;
			float nc = 1.0f;
			float nt = refractionIndex;
			float nnt = into ? nc / nt : nt / nc;
			float ddn = Vector3d::dot(ray.direction, nl);
			float cos2t;
			if ((cos2t = 1 - nnt * nnt * (1 - ddn * ddn)) < 0) 
			{
				return obj.emissiveColor.asLinear() + f * raytrace(photonMap, photonMapCaustic, reflRay, triangleList, bounce + 1, maxLightBounces, refractionIndex);
			}

			Vector3d tdir = Vector3d::normalize(ray.direction * nnt - n * ((into ? 1 : -1) * (ddn * nnt + sqrt(cos2t))));
			Ray tdirRay;
			tdirRay.direction = tdir;
			tdirRay.origin = origin_new;
			float a = nt - nc;
			float b = nt + nc;
			float R0 = a * a / (b * b);
			float c = 1.0f - (into ? -ddn : Vector3d::dot(tdir, n));
			float Re = R0 + (1 - R0) * c * c * c * c * c, Tr = 1 - Re;
			return obj.emissiveColor.asLinear() + f * (raytrace(photonMap, photonMapCaustic, reflRay, triangleList, bounce + 1, maxLightBounces, refractionIndex) * Re
				+ raytrace(photonMap, photonMapCaustic, tdirRay, triangleList, bounce + 1, maxLightBounces, refractionIndex) * Tr);
		}

		Vector3d RayTracer::randomDirection()
		{
			double phi = Random<double>::random(0.0, Math::TWO_PI);
			double costheta = Random<double>::random(-1.0, 1.0);

			double theta = acos(costheta);
		
			return Vector3d(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));
		}

		bool RayTracer::intersect(const Sphered& sphere, const Ray& ray)
		{
			double t0, t1;

			Vector3d L = sphere.getCenter() - ray.origin;
			double tca = Vector3d::dot(L, ray.direction);
			// if (tca < 0) return false;
			double d2 = Vector3d::dot(L, L) - tca * tca;
			double radius2 = sphere.getRadius() * sphere.getRadius();
			if (d2 > radius2) return false;
			double thc = sqrt(radius2 - d2);
			t0 = tca - thc;
			t1 = tca + thc;

			if (t0 > t1) std::swap(t0, t1);

			if (t0 < 0) 
			{
				t0 = t1; 
				if (t0 < 0) return false; 
			}

			//t = t0;

			return true;
		}

		bool RayTracer::intersect(const Ray& ray, const std::vector<Triangle>& triangles, HitResult& hit, int32& id)
		{
			bool h = false;
			for (int32 i = 0; i < triangles.size(); i++)
			{
				if (triangles[i].getIntersection(ray, hit))
				{
					id = i;
					h = true;
				}
			}
			return h;
		}
	}
}
