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

		void RayTracer::raytrace(uint32 raysPerLight, uint32 numLightBounces, std::vector<Ray>& rays)
		{
			Color lightColor = Color(255, 255, 255);//get from light
			Vector3d lightPosition = Vector3d(0.0, 4.0, 0.0); // get from light

			for (int i = 0; i < raysPerLight; i++)
			{
				Ray ray;
				ray.origin = lightPosition;
				ray.direction = randomDirection();
				ray.color = lightColor;

				raytrace(ray, 0, numLightBounces, rays);
			}
		}

		void RayTracer::raytrace(Ray& ray, uint32 depth, uint32 numLightBounces, std::vector<Ray>& rays)
		{
			double t;
			int32 id;
			if (!intersect(ray, _triangles, t, id))
			{
				ray.length = 10.0;

				if (depth > 0)
				{
					rays.push_back(ray);
				}

				return;
			}

			if (ray.color.asLinear().isNearBlack())
			{
				return;
			}

			if (depth > 0)
			{
				rays.push_back(ray);
			}

			LinearColor surfaceColor;
			Vector3d phit = ray.origin + ray.direction * t;

			Triangle tri = _triangles[id];

			Vector3d nhit = tri.normal;

			double bias = 1e-4;
			bool inside = false;
			if (Vector3d::dot(ray.direction, nhit) > 0)
			{
				nhit = -nhit;
				inside = true;
			}

			if ((tri.surfaceColor.A < 255 || tri.reflection > 0.0) && depth < numLightBounces)
			{
				double facingratio = -Vector3d::dot(ray.direction, nhit);

				double fresneleffect = Math::lerp(pow(1.0 - facingratio, 3), 1.0, 0.1);

				Vector3d refldir = ray.direction - nhit * 2 * Vector3d::dot(ray.direction, nhit);
				refldir.normalize();

				surfaceColor = tri.surfaceColor.asLinear() * fresneleffect * tri.reflection; //initial color????

				Ray rayReflect = Ray(phit + nhit * bias, refldir, surfaceColor.toColor(false));
				raytrace(rayReflect, depth + 1, numLightBounces, rays);

				if (tri.surfaceColor.A < 255)
				{
					double ior = 1.1, eta = (inside) ? ior : 1.0 / ior;
					double cosi = -Vector3d::dot(nhit, ray.direction);
					double k = 1 - eta * eta * (1.0 - cosi * cosi);
					Vector3d refrdir = ray.direction * eta + nhit * (eta *  cosi - sqrt(k));
					refrdir.normalize();

					LinearColor surface = tri.surfaceColor.asLinear();
					LinearColor refractColor = surface * (float)((1.0 - fresneleffect) * surface.A);// *10.0;
					refractColor = refractColor.clamp();

					Ray rayRefract = Ray(phit - nhit * bias, refrdir, refractColor.toColor(false));
					raytrace(rayRefract, depth + 1, numLightBounces, rays);
				}

				//surfaceColor = (
				//	reflection * fresneleffect +
				//	refraction * (1 - fresneleffect) * sphere->transparency) * sphere->surfaceColor;
			}
			else 
			{
				/*for (unsigned i = 0; i < spheres.size(); ++i) {
					if (spheres[i].emissionColor.x > 0) {
						// this is a light
						Vec3f transmission = 1;
						Vec3f lightDirection = spheres[i].center - phit;
						lightDirection.normalize();
						for (unsigned j = 0; j < spheres.size(); ++j) {
							if (i != j) {
								float t0, t1;
								if (spheres[j].intersect(phit + nhit * bias, lightDirection, t0, t1)) {
									transmission = 0;
									break;
								}
							}
						}
						surfaceColor += sphere->surfaceColor * transmission *
							std::max(float(0), nhit.dot(lightDirection)) * spheres[i].emissionColor;
					}
				}*/
			}
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

		bool RayTracer::intersect(Ray& ray, const std::vector<Triangle>& triangles, double& t, int32& id)
		{
			double d = 0.0f;
			double inf = t = 1e20;
			for (int i = 0; i < triangles.size(); i++)
			{
				if (triangles[i].getIntersection(ray, d) && d < t)
				{ 
					t = d;
					ray.length = d;
					id = i; 
				}
			}
			return t < inf;
		}

		bool RayTracer::intersect(const Ray& ray, const std::vector<Triangle>& triangles, double& t, int32& id)
		{
			double d = 0.0f;
			double inf = t = 1e20;
			for (int i = 0; i < triangles.size(); i++)
			{
				if (triangles[i].getIntersection(ray, d) && d < t)
				{
					t = d;
					id = i;
				}
			}
			return t < inf;
		}
	}
}
