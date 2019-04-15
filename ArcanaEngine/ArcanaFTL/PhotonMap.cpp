#include "PhotonMap.h"

#include "ArcanaLog.h"
#include "RayTracer.h"
#include "Random.h"

#undef min

namespace Arcana
{
	namespace FTL
	{

		PhotonMap::PhotonMap(uint32 maxPhotons) : maxPhotons(maxPhotons)
		{
			storedPhotons = 0;
			previousScale = 1;

			photons = (Photon*)malloc(sizeof(Photon)*(maxPhotons + 1));

			if (photons == nullptr)
			{
				LOGF(Error, CoreEngine, "Ran out of memory initializing %d photons!", maxPhotons);
			}

			bounds = AxisAlignedBoundingBoxf(1e8f, 1e8f, 1e8f, -1e8f, -1e8f, -1e8f);

			for (int32 i = 0; i < 256; i++)
			{
				double angle = double(i)*(1.0 / 256.0)*Math::PI;
				costheta[i] = cos(angle);
				sintheta[i] = sin(angle);
				cosphi[i] = cos(2.0*angle);
				sinphi[i] = sin(2.0*angle);
			}
		}

		PhotonMap::~PhotonMap()
		{
			free(photons);
		}

		void PhotonMap::store(const Vector3d& energy, const Vector3d& position, const Vector3d& direction, const Vector3d& prevPosition)
		{
			if (storedPhotons > maxPhotons)
			{
				return;
			}

			storedPhotons++;

			Photon* node = &photons[storedPhotons];

			node->position = position;
			node->energy = energy;
			node->previousPosition = prevPosition;

			float minX = Math::min((float)node->position.x, bounds.getMin().x);
			float minY = Math::min((float)node->position.y, bounds.getMin().y);
			float minZ = Math::min((float)node->position.z, bounds.getMin().z);

			float maxX = Math::min((float)node->position.x, bounds.getMax().x);
			float maxY = Math::min((float)node->position.y, bounds.getMax().y);
			float maxZ = Math::min((float)node->position.z, bounds.getMax().z);

			bounds.setMin(Vector3f(minX, minY, minZ));
			bounds.setMax(Vector3f(maxX, maxY, maxZ));

			int32 theta = int32(acos(direction.z) * (256.0 / Math::PI));
			node->theta = Math::min((uint8)theta, (uint8)255);

			int32 phi = int32(atan2(direction.y, direction.x) * (256.0 / (2.0 * Math::PI)));
			if (phi > 255)
			{
				node->phi = 255;
			}
			else if (phi < 0)
			{
				node->phi = (unsigned char)(phi + 256);
			}
			else
			{
				node->phi = (unsigned char)phi;
			}
		}

		void PhotonMap::scalePhotonEnergy(const double scale)
		{
			for (int32 i = previousScale; i <= storedPhotons; i++)
			{
				photons[i].energy *= scale;
			}
			previousScale = storedPhotons;
		}

		void PhotonMap::balance()
		{
			if (storedPhotons > 1)
			{
				Photon** pa1 = (Photon**)malloc(sizeof(Photon*)*(storedPhotons + 1));
				Photon** pa2 = (Photon**)malloc(sizeof(Photon*)*(storedPhotons + 1));

				for (int i = 0; i <= storedPhotons; i++)
				{
					pa2[i] = &photons[i];
				}

				balanceSegment(pa1, pa2, 1, 1, storedPhotons);

				free(pa2);

				int32 d, j = 1, k = 1;
				Photon p = photons[j];

				for (int32 i = 1; i <= storedPhotons; i++)
				{
					d = pa1[j] - photons;
					pa1[j] = nullptr;
					if (d != k)
					{
						photons[j] = photons[d];
					}
					else
					{
						photons[j] = p;
						if (i < storedPhotons) 
						{
							for (; k <= storedPhotons; k++)
							{
								if (pa1[k] != nullptr)
									break;
							}
							p = photons[k];
							j = k;
						}
						continue;
					}
					j = d;
				}
				free(pa1);
			}
			halfStoredPhotons = storedPhotons / 2 - 1;
		}

		Vector3d PhotonMap::estimateIrradiance(const Vector3d& position, const Vector3d& normal, const double maxDistance, const int32 numPhotons) const
		{
			Vector3d irradiance;

			NearestPhotons np;
			np.dist2 = (double*)alloca(sizeof(double)*(numPhotons + 1));
			np.index = (const Photon**)alloca(sizeof(Photon*)*(numPhotons + 1));

			np.position = position;
			np.max = numPhotons;
			np.found = 0;
			np.heap = 0;
			np.dist2[0] = maxDistance * maxDistance;

			locatePhotons(&np, 1);

			if (np.found < 8)
				return Vector3d::zero();

			Vector3d direction;

			for (int32 i = 1; i <= np.found; i++) 
			{
				const Photon* p = np.index[i];

				direction = photonDirection(p);

				if (Vector3d::dot(direction, normal) < 0.0) 
				{
					irradiance += p->energy;
				}
			}

			const double tmp = (1.0 / Math::PI) / (np.dist2[0]);

			return irradiance * tmp;
		}

		void PhotonMap::locatePhotons(NearestPhotons* np, const int32 index)  const
		{
			const Photon *p = &photons[index];
			double dist1;

			if (index < halfStoredPhotons)
			{
				dist1 = getComponent(np->position, p->plane) - getComponent(p->position, p->plane);

				if (dist1 > 0.0)
				{
					locatePhotons(np, 2 * index + 1);
					if (dist1 * dist1 < np->dist2[0])
					{
						locatePhotons(np, 2 * index);
					}
				}
				else 
				{
					locatePhotons(np, 2 * index);
					if (dist1 * dist1 < np->dist2[0])
					{
						locatePhotons(np, 2 * index + 1);
					}
				}
			}

			double dist2 = Vector3d::distanceSq(p->position, np->position);

			if (dist2 < np->dist2[0]) 
			{

				if (np->found < np->max) 
				{
					np->found++;
					np->dist2[np->found] = dist2;
					np->index[np->found] = p;
				}
				else 
				{
					int32 j, parent;
					if (np->heap == 0)
					{
						double dst2;
						const Photon* photon;
						int32 halfFound = np->found >> 1;
						for (int32 k = halfFound; k >= 1; k--)
						{
							parent = k;
							photon = np->index[k];
							dst2 = np->dist2[k];
							while (parent <= halfFound)
							{
								j = parent + parent;
								if (j < np->found && np->dist2[j] < np->dist2[j + 1])
									j++;
								if (dst2 >= np->dist2[j])
									break;
								np->dist2[parent] = np->dist2[j];
								np->index[parent] = np->index[j];
								parent = j;
							}
							np->dist2[parent] = dst2;
							np->index[parent] = photon;
						}
						np->heap = 1;
					}
					parent = 1;
					j = 2;
					while (j <= np->found)
					{
						if (j < np->found && np->dist2[j] < np->dist2[j + 1])
							j++;
						if (dist2 > np->dist2[j])
							break;
						np->dist2[parent] = np->dist2[j];
						np->index[parent] = np->index[j];
						parent = j;
						j += j;
					}
					np->index[parent] = p;
					np->dist2[parent] = dist2;

					np->dist2[0] = np->dist2[1];
				}
			}
		}

		Vector3d PhotonMap::photonDirection(const Photon* p) const
		{
			return Vector3d(sintheta[p->theta] * cosphi[p->phi], sintheta[p->theta] * sinphi[p->phi], costheta[p->theta]);
		}

		void PhotonMap::balanceSegment(Photon** balancedPhotons, Photon** originalPhotons, const int32 index, const int32 start, const int32 end)
		{
			int32 median = 1;
			while ((4 * median) <= (end - start + 1))
			{
				median += median;
			}
			if ((3 * median) <= (end - start + 1))
			{
				median += median;
				median += start - 1;
			}
			else
			{
				median = end - median + 1;
			}

			int32 axis = 2;
			if ((bounds.getMax().x - bounds.getMin().x) > (bounds.getMax().y - bounds.getMin().y) &&
				(bounds.getMax().x - bounds.getMin().x) > (bounds.getMax().z - bounds.getMin().y))
			{
				axis = 0;
			}
			else if ((bounds.getMax().y - bounds.getMin().y) > (bounds.getMax().z - bounds.getMin().z))
			{
				axis = 1;
			}

			medianSplit(originalPhotons, start, end, median, axis);

			balancedPhotons[index] = originalPhotons[median];
			balancedPhotons[index]->plane = axis;


			if (median > start) 
			{
				if (start < median - 1) 
				{
					const float tmp = getComponent(bounds.getMax(), axis);
					double val = getComponent(balancedPhotons[index]->position, axis);
					if (axis == 0)
					{
						bounds.setMax(Vector3f(val, bounds.getMax().y, bounds.getMax().z));
					}
					else if (axis == 1)
					{
						bounds.setMax(Vector3f(bounds.getMax().x, val, bounds.getMax().z));
					}
					else if (axis == 2)
					{
						bounds.setMax(Vector3f(bounds.getMax().x, bounds.getMax().y, val));
					}

					balanceSegment(balancedPhotons, originalPhotons, 2 * index, start, median - 1);

					if (axis == 0)
					{
						bounds.setMax(Vector3f(tmp, bounds.getMax().y, bounds.getMax().z));
					}
					else if (axis == 1)
					{
						bounds.setMax(Vector3f(bounds.getMax().x, tmp, bounds.getMax().z));
					}
					else if (axis == 2)
					{
						bounds.setMax(Vector3f(bounds.getMax().x, bounds.getMax().y, tmp));
					}
				}
				else 
				{
					balancedPhotons[2 * index] = originalPhotons[start];
				}
			}
			if (median < end)
			{
				if (median + 1 < end)
				{
					const float tmp = getComponent(bounds.getMin(), axis);
					double val = getComponent(balancedPhotons[index]->position, axis);
					if (axis == 0)
					{
						bounds.setMin(Vector3f(val, bounds.getMin().y, bounds.getMin().z));
					}
					else if (axis == 1)
					{
						bounds.setMin(Vector3f(bounds.getMin().x, val, bounds.getMin().z));
					}
					else if (axis == 2)
					{
						bounds.setMin(Vector3f(bounds.getMin().x, bounds.getMin().y, val));
					}

					balanceSegment(balancedPhotons, originalPhotons, 2 * index + 1, median + 1, end);

					if (axis == 0)
					{
						bounds.setMin(Vector3f(tmp, bounds.getMin().y, bounds.getMin().z));
					}
					else if (axis == 1)
					{
						bounds.setMin(Vector3f(bounds.getMin().x, tmp, bounds.getMin().z));
					}
					else if (axis == 2)
					{
						bounds.setMin(Vector3f(bounds.getMin().x, bounds.getMin().y, tmp));
					}
				}
				else 
				{
					balancedPhotons[2 * index + 1] = originalPhotons[end];
				}
			}
		}

#define swap(ph,a,b) {Photon *ph2 = ph[a]; ph[a] =ph[b]; ph[b]=ph2;}

		void PhotonMap::medianSplit(Photon** photons, const int32 start, const int32 end, const int32 median, const int32 axis)
		{
			int32 left = start;
			int32 right = end;

			while (right > left) 
			{
				const double v = getComponent(photons[right]->position, axis);
				int32 i = left - 1;
				int32 j = right;
				for (;;)
				{
					while (getComponent(photons[++i]->position, axis) < v)
						;
					while (getComponent(photons[--j]->position, axis) > v && j > left)
						;
					if (i >= j)
						break;
					swap(photons, i, j);
				}
				swap(photons, i, right);
				if (i >= median)
					right = i - 1;
				if (i <= median)
					left = i + 1;
			}
		}


		void PhotonMap::photonTracing(
			PhotonMap& map,
			const Ray& ray,
			std::vector<Triangle>& triangles,
			LinearColor color,
			int32 depth,
			float RI,
			int32 maxLightBounces)
		{

			double t;
			int32 id = 0; 
			if (!RayTracer::intersect(ray, triangles, t, id))
				return;

			const Triangle& tri = triangles[id];

			if (++depth > maxLightBounces)
				return;

			Vector3d originNew = ray.origin + ray.direction * t;
			Vector3d n = tri.normal;
			Vector3d nl = Vector3d::dot(n, ray.direction) < 0 ? n : -n;
			LinearColor f = tri.surfaceColor.asLinear();

			double p = f.R > f.G && f.R > f.B ? f.R : f.G > f.B ? f.G : f.B; // max refl 

			if (tri.type == SurfaceType::Diffusive)
			{           
				if (Random<double>::random() > p) 
				{
					Vector3d c = Vector3d(f.R * color.R * (1.0f / (1.0 - p)), f.G * color.G * (1.0f / (1.0f - p)), f.B * color.B * (1.0f / (1.0f - p)));
					map.store(c, originNew, ray.direction, ray.origin);
					return;

				}
				else 
				{
					Vector3d c = Vector3d(f.R * color.R * (1.0f / (1.0 - p)), f.G * color.G * (1.0f / (1.0f - p)), f.B * color.B * (1.0f / (1.0f - p)));
					map.store(c, originNew, ray.direction, ray.origin);

					double r1 = Math::TWO_PI * Random<double>::random();
					double r2 = Random<double>::random();
					double r2s = sqrt(r2);

					Vector3d w = nl;
					Vector3d u = Vector3d::normalize(Vector3d::cross(fabs(w.x) > 0.1 ? Vector3d::unitY() : Vector3d::unitX(), w));
					Vector3d v = Vector3d::cross(w, u);
					Vector3d d = Vector3d::normalize(u*cos(r1)*r2s + v * sin(r1)*r2s + w * sqrt(1 - r2));
					Ray newRay(originNew, d);
					photonTracing(map, newRay, triangles, f * color * (1.0f / p), depth, RI, maxLightBounces);
				}
			}
			else if (tri.type == SurfaceType::PureSpecular)
			{
				Vector3d c = Vector3d(f.R * color.R * (1.0f / (1.0 - p)), f.G * color.G * (1.0f / (1.0f - p)), f.B * color.B * (1.0f / (1.0f - p)));
				map.store(c, originNew, ray.direction, ray.origin);

				Ray newRay(originNew, ray.direction - n * 2 * Vector3d::dot(n, ray.direction));
				photonTracing(map, newRay, triangles, f * color * (1.0f / p), depth, RI, maxLightBounces);
			}
			else if (tri.type == SurfaceType::Transparent)
			{
				Vector3d c0 = Vector3d(f.R * color.R * (1.0f / (1.0 - p)), f.G * color.G * (1.0f / (1.0f - p)), f.B * color.B * (1.0f / (1.0f - p)));
				map.store(c0, originNew, ray.direction, ray.origin);

				Ray reflRay(originNew, ray.direction - n * 2 * Vector3d::dot(n, ray.direction));
				bool into = Vector3d::dot(n, nl) > 0;
				double nc = 1;
				double nt = RI;
				double nnt = into ? nc / nt : nt / nc;
				double ddn = Vector3d::dot(ray.direction, nl);
				double cos2t;
				if ((cos2t = 1 - nnt * nnt*(1 - ddn * ddn)) < 0)
				{
					photonTracing(map, reflRay, triangles, color, depth, RI, maxLightBounces);
				}
				Ray tdir(originNew, Vector3d::normalize(ray.direction * nnt - n * ((into ? 1 : -1)*(ddn*nnt + sqrt(cos2t)))));
				double a = nt - nc;
				double b = nt + nc;
				double R0 = a * a / (b*b);
				double c = 1.0 - (into ? -ddn : Vector3d::dot(tdir.direction, n));
				double Re = R0 + (1 - R0)*c*c*c*c*c, Tr = 1 - Re, P = .25 + .5*Re, RP = Re / P, TP = Tr / (1 - P);
				Random<double>::random() < P ? 
					photonTracing(map, reflRay, triangles, color*RP, depth, RI, maxLightBounces) :
					photonTracing(map, tdir, triangles, color*TP, depth, RI, maxLightBounces);
			}
		}

		void PhotonMap::photonTracingCaustics(
			PhotonMap& map,
			const Ray& ray,
			std::vector<Triangle>& triangles,
			LinearColor color,
			int32 depth,
			bool flag,
			float RI,
			int32 maxLightBounces,
			std::vector<Sphered>& bound)
		{
			if (!flag) 
			{
				for (int i = 0; i < bound.size(); i++) 
				{
					if (!RayTracer::intersect(bound[i], ray)) return;
				}
			}

			double t;
			int32 id = 0;
			if (!RayTracer::intersect(ray, triangles, t, id))
			{
				return;
			}

			if (++depth > maxLightBounces)
			{
				return;
			}

			const Triangle& tri = triangles[id];

			Vector3d originNew = ray.origin + ray.direction * t;
			Vector3d n = tri.normal;
			Vector3d nl = Vector3d::dot(n, ray.direction) < 0 ? n : -n;
			LinearColor f = tri.surfaceColor.asLinear();

			double p = f.R > f.G && f.R > f.B ? f.R : f.G > f.B ? f.G : f.R; // max refl 

			if (flag == false && tri.type != SurfaceType::Transparent)
			{
				return;
			}

			flag = true;

			if (tri.type == SurfaceType::Diffusive)
			{
				if (depth > maxLightBounces || Random<double>::random() > p) 
				{
					Vector3d c = Vector3d(f.R * color.R * (1.0f / (1.0f - p)), f.G * color.G * (1.0f / (1.0f - p)), f.B * color.B * (1.0f / (1.0f - p)));
					map.store(c, originNew, ray.direction, ray.origin);
					return;

				}
				else
				{
					double r1 = Math::TWO_PI * Random<double>::random();
					double r2 = Random<double>::random();
					double r2s = sqrt(r2);
					Vector3d w = nl;
					Vector3d u = Vector3d::normalize(Vector3d::cross(fabs(w.x) > 0.1 ? Vector3d::unitY() : Vector3d::unitX(), w));
					Vector3d v = Vector3d::cross(w, u);
					Vector3d d = Vector3d::normalize(u*cos(r1)*r2s + v * sin(r1)*r2s + w * sqrt(1 - r2));
					Ray newRay(originNew, d);
					photonTracingCaustics(map, newRay, triangles, f * color * (1.0f / p), depth, flag, RI, maxLightBounces, bound);
				}
			}
			else if (tri.type == SurfaceType::PureSpecular)
			{
				Ray newRay(originNew, ray.direction - n * 2 * Vector3d::dot(n, ray.direction));
				photonTracing(map, newRay, triangles, f * color * (1.0f / p), depth, RI, maxLightBounces);
			}
			else if (tri.type == SurfaceType::Transparent)
			{
				Ray reflRay(originNew, ray.direction - n * 2 * Vector3d::dot(n, ray.direction));
				bool into = Vector3d::dot(n, nl) > 0;
				double nc = 1;
				double nt = RI;
				double nnt = into ? nc / nt : nt / nc;
				double ddn = Vector3d::dot(ray.direction, nl);
				double cos2t;
				if ((cos2t = 1 - nnt * nnt*(1 - ddn * ddn)) < 0) 
				{
					photonTracing(map, reflRay, triangles, color, depth, RI, maxLightBounces);
				}
				Ray tdir(originNew, Vector3d::normalize(ray.direction*nnt - n * ((into ? 1 : -1)*(ddn*nnt + sqrt(cos2t)))));

				double a = nt - nc;
				double b = nt + nc;
				double R0 = a * a / (b*b);
				double c = 1.0 - (into ? -ddn : Vector3d::dot(tdir.direction, n));
				double Re = R0 + (1 - R0)*c*c*c*c*c, Tr = 1 - Re, P = .25 + .5*Re, RP = Re / P, TP = Tr / (1 - P);
				Random<double>::random() < P ?
					photonTracingCaustics(map, reflRay, triangles, color * RP, depth, flag, RI, maxLightBounces, bound) :
					photonTracingCaustics(map, tdir, triangles, color * TP, depth, flag, RI, maxLightBounces, bound);
			}
		}
	}
}
