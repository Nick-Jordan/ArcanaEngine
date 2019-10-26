#include "FasterThanLight.h"

#include "TextureAttachment.h"
#include "DepthStencilAttachment.h"
#include "MeshRenderProcedure.h"
#include "MeshIndexComponent.h"
#include "FileOutputStream.h"
#include "Random.h"

#define DEBUG_TRIANGLES
//#define DEBUG_RAYS

namespace Arcana
{
	namespace FTL
	{
		uint32 NumLightBounces = 2;
		uint32 NumCausticsLightBounces = 3;
		uint32 NumPhotons = 100000;
		uint32 NumCausticsPhotons = 100000;
		uint32 MaxIterations = 1000000;
		uint32 MaxCausticsIterations = 2000000;
		uint32 TextureBlurX = 3;
		uint32 TextureBlurY = 3;
		uint32 TextureBlurZ = 3;
		double TextureScaleFactor = 3.2;


		Mesh* LightProcessor::DebugMesh = nullptr;

		LightProcessor::LightProcessor(World* world)
		{
			LOG(Info, CoreEngine, "Running FasterThanLight!");

			for (uint32 i = 0; i < world->getNumActors(); i++)
			{
				Actor* actor = world->getActor(i);

				if (actor->getMobility() == Actor::Mobility::Static)
				{
					Array<BaseLightComponent*> lightComponents;
					actor->getComponents(lightComponents);

					bool c = false;

					for (auto iter = lightComponents.createConstIterator(); iter; iter++)
					{
						BaseLightComponent* comp = *iter;

						if (comp && comp->isActive())
						{
							addLight(comp);
							c = true;
						}
					}

					if (c) continue;

					Array<StaticMeshComponent*> geometryComponents;
					actor->getComponents(geometryComponents);

					for (auto iter = geometryComponents.createConstIterator(); iter; iter++)
					{
						StaticMeshComponent* comp = *iter;

						if (comp && comp->isActive() && comp->hasRenderProcedure() && comp->getStaticMesh())
						{
							addGeometry(comp);
						}
					}
				}
				else
				{
					Array<StaticMeshComponent*> geometryComponents;
					actor->getComponents(geometryComponents);

					for (auto iter = geometryComponents.createConstIterator(); iter; iter++)
					{
						StaticMeshComponent* comp = *iter;

						if (comp && comp->isActive() && comp->hasRenderProcedure() && comp->getStaticMesh())
						{
							addGeometry(comp, true);
						}
					}
				}
			}
		}

		LightProcessor::~LightProcessor()
		{

		}

		void LightProcessor::run()
		{
			//run3DRaytracer();
			runPhotonMapping();
			cleanup();
		}

		void LightProcessor::read()
		{
			cleanup();
		}

		void LightProcessor::runPhotonMapping()
		{
			if (_geometry.size() == 0 || (_directionalLights.size() == 0 && _pointLights.size() == 0 && _spotLights.size()))
			{
				return;
			}

			//PHOTON MAPPING

			std::vector<Triangle> triangles;

			generateTriangleList(triangles);

			LOGF(Info, CoreEngine, "triangles: %d", triangles.size());

			//test
			double lightArea = 0.25 * 6;
			double totalArea = 0.25 * 6;
			Vector3d lightPosition = Vector3d(0.0, 4.0, 0.0);
			//test

			PhotonMap photonMapR(NumPhotons);
			PhotonMap photonMapG(NumPhotons);
			PhotonMap photonMapB(NumPhotons);

			PhotonMap photonMapCausticsR(NumCausticsPhotons);
			PhotonMap photonMapCausticsG(NumCausticsPhotons);
			PhotonMap photonMapCausticsB(NumCausticsPhotons);

			for (int n = 0; n < 3; n++)
			{
				PhotonMap& map = n == 0 ? photonMapR : n == 1 ? photonMapG : photonMapB;

				uint32 iteration = 0;

				while (map.storedPhotons < (float)map.maxPhotons * lightArea / totalArea && (iteration < MaxIterations || MaxIterations == 0))
				{
					Vector3d position = lightPosition;
					Vector3d direction = RayTracer::randomDirection();
					LinearColor color(0.06f, 0.06f, 0.06f);
					Ray ray(position, direction);
					PhotonMap::photonTracing(map, ray, triangles, color, 0, 2.40 + 0.03 * (double)n, NumLightBounces);

					iteration++;
				}

				LOGF(Info, CoreEngine, "Photon map, %d, finished", n);

				map.balance();
				map.scalePhotonEnergy(1.0 / (double)map.storedPhotons);
			}

			std::vector<Sphered> sphereBounds;

			sphereBounds.push_back(Sphered(Vector3d(2.0, 0.0, -2.0), sqrt(3.0)));

			for (int n = 0; n < 3; n++)
			{
				PhotonMap& map = n == 0 ? photonMapCausticsR : n == 1 ? photonMapCausticsG : photonMapCausticsB;

				uint32 iteration = 0;

				while (map.storedPhotons < (float)map.maxPhotons * lightArea / totalArea && (iteration < MaxCausticsIterations || MaxCausticsIterations == 0))
				{
					Vector3d position = lightPosition;
					Vector3d direction = RayTracer::randomDirection();
					LinearColor color(0.004f, 0.004f, 0.004f);
					Ray ray(position, direction);
					PhotonMap::photonTracingCaustics(map, ray, triangles, color, 0, false, 2.40 + 0.03 * (double)n, NumCausticsLightBounces, sphereBounds);

					iteration++;
				}

				map.balance();
				map.scalePhotonEnergy(1.0 / (double)map.storedPhotons);

				LOGF(Info, CoreEngine, "Caustics photon map, %d, finished", n);
			}

			std::map<StaticMeshComponent*, uint8*> textureData;

			for (auto i = _geometry.createConstIterator(); i; i++)
			{
				StaticMeshComponent* comp = *i;

				int32 size = comp->getStaticMesh()->getLightMapResolution();

				uint8* data = new uint8[size * size * 3];

				for (int j = 0; j < size * size * 3; j++)
				{
					data[j] = 0;
				}

				textureData.insert(std::make_pair(comp, data));
			}

			std::vector<Ray> shadowRays;

			for (int32 i = 0; i < triangles.size(); i++)
			{
				Triangle& triangle = triangles[i];

				if (!triangle.parent)
					continue;

				int32 w, h;
				w = h = triangle.parent->getStaticMesh()->getLightMapResolution();

				uint8* data = textureData[triangle.parent];

				Vector2f uvA = triangle.auv;
				Vector2f uvB = triangle.buv;
				Vector2f uvC = triangle.cuv;

				for (float u = 0.0f; u <= 1.0f; u += 1.0f / (float)w)
				{
					for (float v = 0.0f; v <= 1.0f - u; v += 1.0f / (float)h)
					{
						float t = 1.0f - u - v;

						Vector2f uv = t * uvA + u * uvB + v * uvC;
						Vector2i position = (uv * Vector2f(w, h)).cast<int32>();
						position.x = Math::clamp(position.x, 0, w);
						position.y = Math::clamp(position.y, 0, h);

						int32 index = position.x + position.y * w;

						Vector3d normal = triangle.getNormal(t, u, v);
						Vector3d pos = triangle.getPosition(t, u, v);

						Ray ray;
						ray.direction = normal;
						ray.origin = pos;


						//test
						Vector3d dirToLight = Vector3d(0, 4, 0) - triangle.getPosition(t, u, v);
						double distToLight = dirToLight.magnitude();
						dirToLight = dirToLight / distToLight;

						Ray shadowRay;
						shadowRay.origin = pos + normal * Math::EPSILON;
						shadowRay.direction = dirToLight;

						HitResult hit;
						int32 id = 0;
						bool skip = false;
						if (RayTracer::intersect(shadowRay, triangles, hit, id))
						{
							skip = hit.t < distToLight;
						}
						
						LinearColor color(0, 0, 0);
						if (!skip)
						{
							float diffuse = Vector3d::dot(dirToLight, triangle.getNormal(t, u, v));
							diffuse = Math::clamp(diffuse, 0.0f, 1.0f);

							color = triangle.surfaceColor.asLinear() * (diffuse * 1.0f);
						}
						else
						{
							//shadowRays.push_back(shadowRay);
						}
						//lights
						color.R += RayTracer::raytrace(photonMapR, photonMapCausticsR, ray, triangles, 0, NumLightBounces, 2.40).R;
						color.G += RayTracer::raytrace(photonMapG, photonMapCausticsG, ray, triangles, 0, NumLightBounces, 2.43).G;
						color.B += RayTracer::raytrace(photonMapB, photonMapCausticsB, ray, triangles, 0, NumLightBounces, 2.46).B;

						Color finalColor = color.toColor(false);

						data[index * 3 + 0] = finalColor.R;
						data[index * 3 + 1] = finalColor.G;
						data[index * 3 + 2] = finalColor.B;
					}
				}

				LOGF(Info, CoreEngine, "Finished triangle: %d lightmap", i);
			}

			for (auto i = _geometry.createIterator(); i; i++)
			{
				StaticMeshComponent* comp = *i;

				int32 size = comp->getStaticMesh()->getLightMapResolution();

				Texture::Parameters params;
				params.setWrapS(TextureWrap::ClampToEdge);
				params.setWrapT(TextureWrap::ClampToEdge);
				params.setMinFilter(TextureFilter::Linear);
				params.setMagFilter(TextureFilter::Linear);

				uint8* data = textureData[comp];

				Texture* texture = Texture::create2D(Texture::RGB, size, size, Texture::RGB8, Texture::UnsignedByte, data, params, false);

				comp->setLightMap(texture);

				AE_DELETE_ARRAY(data);
			}

			std::vector<Vector3f> vertices;

#ifdef DEBUG_TRIANGLES
			for (int i = 0; i < triangles.size(); i++)
			{
				Vector3f a = triangles[i].v1.cast<float>();
				Vector3f b = triangles[i].v1.cast<float>() + triangles[i].edge1.cast<float>();
				Vector3f c = triangles[i].v1.cast<float>() + triangles[i].edge2.cast<float>();

				LinearColor lc = triangles[i].surfaceColor.asLinear();
				Vector3f color = Vector3f(lc.R, lc.G, lc.B);

				vertices.push_back(a);
				vertices.push_back(color);
				vertices.push_back(b);
				vertices.push_back(color);
				vertices.push_back(a);
				vertices.push_back(color);
				vertices.push_back(c);
				vertices.push_back(color);
				vertices.push_back(b);
				vertices.push_back(color);
				vertices.push_back(c);
				vertices.push_back(color);

				Vector3f middle = (a + b + c) / 3.0f;

				vertices.push_back(middle);
				vertices.push_back(Vector3f::unitX());
				vertices.push_back(middle + triangles[i].getNormal(0.33, 0.33, 0.33).cast<float>());
				vertices.push_back(Vector3f::unitX());
			}

			for (int i = 0; i < shadowRays.size(); i++)
			{
				vertices.push_back(shadowRays[i].origin.cast<float>());
				vertices.push_back(Vector3f::unitZ());
				vertices.push_back(shadowRays[i].origin.cast<float>() + shadowRays[i].direction.cast<float>() * 10);
				vertices.push_back(Vector3f::unitZ());
			}
#endif

			//////////////////////DEBUG///////////////////////
			if (vertices.size() > 1)
			{
				VertexFormat::Attribute attribs[] =
				{
					VertexFormat::Attribute(VertexFormat::Semantic::Position, 3),
					VertexFormat::Attribute(VertexFormat::Semantic::Color, 3),
				};
				VertexFormat format(2, attribs);
				DebugMesh = new Mesh(format, Mesh::Lines);

				DebugMesh->setVertexBuffer(format, vertices.size() / 2)->setVertexData(&vertices[0].x);
			}
			//////////////////////DEBUG///////////////////////


			/*lightData = blur(lightData, size, Vector3i(TextureBlurX, TextureBlurY, TextureBlurZ), 3);

			data = Texture::create3D(Texture::RGB, size.x, size.y, size.z, Texture::RGB8, Texture::UnsignedByte, lightData, params);

			FileOutputStream output;
			output.open("resources/arcana/ftl/light_data_photon_mapping.ftl");
			output.write(lightData, size.x * size.y * size.z * 3, 0);



			for (auto i = _geometry.createIterator(); i; i++)
			{
				StaticMeshComponent* component = *i;

				component->setFTLGlobalIlluminationTexture(data, bounds);
			}
			for (auto i = _dynamicGeometry.createIterator(); i; i++)
			{
				StaticMeshComponent* component = *i;

				component->setFTLGlobalIlluminationTexture(data, bounds);
			}*/
		}

		void LightProcessor::addGeometry(StaticMeshComponent* geometry, bool dynamic)
		{
			if (geometry)
			{
				geometry->reference();
				if (dynamic)
				{
					_dynamicGeometry.add(geometry);
				}
				else
				{
					_geometry.add(geometry);
				}
			}
		}

		void LightProcessor::addLight(BaseLightComponent* light)
		{
			if (light)
			{
				LightType type = light->getLightType();

				if (type == Directional)
				{
					light->reference();
					_directionalLights.add(dynamic_cast<DirectionalLightComponent*>(light));
				}
				else if (type == Point)
				{
					light->reference();
					_pointLights.add(dynamic_cast<PointLightComponent*>(light));
				}
				else if (type == Spot)
				{
					light->reference();
					_spotLights.add(dynamic_cast<SpotLightComponent*>(light));
				}
				/*else if (type == Rect)
				{
					light->reference();
					_rectLights.add(dynamic_cast<RectLightComponent*>(light));
				}*/
			}
		}

		void LightProcessor::generateTriangleList(std::vector<Triangle>& triangles)
		{
			/*triangles = ////////////////////JUST A HARDCODED TEST
			{
				Triangle(Vector3d(5.0, -5.0, -5.0), Vector3d(5.0, -5.0, 5.0), Vector3d(-5.0, -5.0, 5.0), SurfaceType::Diffusive, 0.5, Vector2f(1.0, 0.0), Vector2f(1.0, 1.0), Vector2f(0.0, 1.0), Color(128, 128, 128)), //FLOOR
				Triangle(Vector3d(5.0, -5.0, -5.0), Vector3d(-5.0, -5.0, 5.0), Vector3d(-5.0, -5.0, -5.0), SurfaceType::Diffusive, 0.5, Vector2f(1.0, 0.0), Vector2f(0.0, 1.0), Vector2f(0.0, 0.0), Color(128, 128, 128)),

				Triangle(Vector3d(-5.0, 5.0, 5.0), Vector3d(5.0, 5.0, 5.0), Vector3d(5.0, 5.0, -5.0), SurfaceType::Diffusive, 0.5, Vector2f(0.0, 1.0), Vector2f(1.0, 1.0), Vector2f(1.0, 0.0), Color(128, 128, 128)), //Roof
				Triangle(Vector3d(-5.0, 5.0, -5.0), Vector3d(-5.0, 5.0, 5.0), Vector3d(5.0, 5.0, -5.0), SurfaceType::Diffusive, 0.5, Vector2f(0.0, 0.0), Vector2f(0.0, 1.0), Vector2f(1.0, 0.0), Color(128, 128, 128)),

				Triangle(Vector3d(5.0, 5.0, -5.0), Vector3d(5.0, 5.0, 5.0), Vector3d(5.0, -5.0, 5.0), SurfaceType::Diffusive, 0.5, Vector2f(1.0, 0.0), Vector2f(1.0, 1.0), Vector2f(0.0, 1.0), Color(255, 0, 0)), //RED WALL
				Triangle(Vector3d(5.0, -5.0, 5.0), Vector3d(5.0, -5.0, -5.0), Vector3d(5.0, 5.0, -5.0), SurfaceType::Diffusive, 0.5, Vector2f(0.0, 1.0), Vector2f(0.0, 0.0), Vector2f(1.0, 0.0), Color(255, 0, 0)),

				Triangle(Vector3d(-5.0, -5.0, 5.0), Vector3d(-5.0, 5.0, 5.0), Vector3d(-5.0, 5.0, -5.0), SurfaceType::Diffusive, 0.5, Vector2f(0.0, 1.0), Vector2f(1.0, 1.0), Vector2f(1.0, 0.0), Color(0, 255, 0)), //GREEN WALL
				Triangle(Vector3d(-5.0, 5.0, -5.0), Vector3d(-5.0, -5.0, -5.0), Vector3d(-5.0, -5.0, 5.0), SurfaceType::Diffusive, 0.5, Vector2f(1.0, 0.0), Vector2f(0.0, 0.0), Vector2f(0.0, 1.0), Color(0, 255, 0)),

				Triangle(Vector3d(-5.0, -5.0, -5.0), Vector3d(-5.0, 5.0, -5.0), Vector3d(5.0, 5.0, -5.0), SurfaceType::Diffusive, 0.5, Vector2f(0.0, 0.0), Vector2f(1.0, 1.0), Vector2f(0.0, 1.0), Color(128, 128, 128)), //WHITE WALL
				Triangle(Vector3d(5.0, 5.0, -5.0), Vector3d(5.0, -5.0, -5.0), Vector3d(-5.0, -5.0, -5.0), SurfaceType::Diffusive, 0.5, Vector2f(1.0, 0.0), Vector2f(1.0, 1.0), Vector2f(0.0, 1.0), Color(128, 128, 128)),

				//Vector3d(2.8, -5.1 + 1.4, 1.5), Vector3d(1.4, 1.4, 1.4)

				Triangle(Vector3d(1.4, -5.1 + 2.8, 0.1), Vector3d(2.8 + 1.4, -5.1 + 2.8, 0.1), Vector3d(2.8 + 1.4, -5.1 + 2.8, 0.1 + 2.8), SurfaceType::Diffusive, 0.5, Color(128, 128, 128)), //RIGHT BOX TOP
				Triangle(Vector3d(2.8 + 1.4, -5.1 + 2.8, 0.1 + 2.8), Vector3d(1.4, -5.1 + 2.8, 0.1 + 2.8), Vector3d(1.4, -5.1 + 2.8, 0.1), SurfaceType::Diffusive, 0.5, Color(128, 128, 128)),

				Triangle(Vector3d(1.4, -5.1 + 2.8, 2.9), Vector3d(2.8 + 1.4, -5.1 + 2.8, 2.9), Vector3d(2.8 + 1.4, -5.1, 2.9), SurfaceType::Diffusive, 0.5, Color(128, 128, 128)), //RIGHT BOX front
				Triangle(Vector3d(2.8 + 1.4, -5.1, 2.9), Vector3d(1.4, -5.1, 2.9), Vector3d(1.4, -5.1 + 2.8, 2.9), SurfaceType::Diffusive, 0.5, Color(128, 128, 128)),

				Triangle(Vector3d(2.8 + 1.4, -5.1, 0.1), Vector3d(2.8 + 1.4, -5.1 + 2.8, 0.1), Vector3d(1.4, -5.1 + 2.8, 0.1), SurfaceType::Diffusive, 0.5, Color(128, 128, 128)), //RIGHT BOX back
				Triangle(Vector3d(1.4, -5.1 + 2.8, 0.1), Vector3d(1.4, -5.1, 0.1), Vector3d(2.8 + 1.4, -5.1, 0.1), SurfaceType::Diffusive, 0.5, Color(128, 128, 128)),

				Triangle(Vector3d(1.4, -5.1 + 2.8, 0.1), Vector3d(1.4, -5.1 + 2.8, 2.9), Vector3d(1.4, -5.1, 2.9), SurfaceType::Diffusive, 0.5, Color(128, 128, 128)), //RIGHT BOX left
				Triangle(Vector3d(1.4, -5.1, 2.9), Vector3d(1.4, -5.1, 0.1), Vector3d(1.4, -5.1 + 2.8, 0.1), SurfaceType::Diffusive, 0.5, Color(128, 128, 128)),

				Triangle(Vector3d(2.8 + 1.4, -5.1, 2.9), Vector3d(2.8 + 1.4, -5.1 + 2.8, 2.9), Vector3d(2.8 + 1.4, -5.1 + 2.8, 0.1), SurfaceType::Diffusive, 0.5, Color(128, 128, 128)), //RIGHT BOX right
				Triangle(Vector3d(2.8 + 1.4, -5.1 + 2.8, 0.1), Vector3d(2.8 + 1.4, -5.1, 0.1), Vector3d(2.8 + 1.4, -5.1, 2.9), SurfaceType::Diffusive, 0.5, Color(128, 128, 128)),

				//Vector3d(2.0, 0.0, -2.0), Vector3d(1.0, 1.0, 1.0)


				Triangle(Vector3d(3.0, 1.0, -3.0), Vector3d(3.0, 1.0, -1.0), Vector3d(1.0, 1.0, -1.0), SurfaceType::Transparent, 0.1, Color(25, 25, 178, 77)), //TRANSPARENT BOX TOP
				Triangle(Vector3d(1.0, 1.0, -1.0), Vector3d(1.0, 1.0, -3.0), Vector3d(3.0, 1.0, -3.0), SurfaceType::Transparent, 0.1, Color(25, 25, 178, 77)),

				Triangle(Vector3d(1.0, -1.0, -1.0), Vector3d(3.0, -1.0, -1.0), Vector3d(3.0, -1.0, -3.0), SurfaceType::Transparent, 0.1, Color(25, 25, 178, 77)), //TRANSPARENT BOX bottom
				Triangle(Vector3d(3.0, -1.0, -3.0), Vector3d(1.0, -1.0, -3.0), Vector3d(1.0, -1.0, -1.0), SurfaceType::Transparent, 0.1, Color(25, 25, 178, 77)),

				Triangle(Vector3d(1.0, -1.0, -1.0), Vector3d(1.0, 1.0, -1.0), Vector3d(3.0, 1.0, -1.0), SurfaceType::Transparent, 0.1, Color(25, 25, 178, 77)), //TRANSPARENT BOX front
				Triangle(Vector3d(3.0, 1.0, -1.0), Vector3d(3.0, -1.0, -1.0), Vector3d(1.0, -1.0, -1.0), SurfaceType::Transparent, 0.1, Color(25, 25, 178, 77)),

				Triangle(Vector3d(3.0, 1.0, -3.0), Vector3d(1.0, 1.0, -3.0), Vector3d(1.0, -1.0, -3.0), SurfaceType::Transparent, 0.1, Color(25, 25, 178, 77)), //TRANSPARENT BOX back
				Triangle(Vector3d(1.0, -1.0, -3.0), Vector3d(3.0, -1.0, -3.0), Vector3d(3.0, 1.0, -3.0), SurfaceType::Transparent, 0.1, Color(25, 25, 178, 77)),

				Triangle(Vector3d(1.0, 1.0, -3.0), Vector3d(1.0, 1.0, -1.0), Vector3d(1.0, -1.0, -1.0), SurfaceType::Transparent, 0.1, Color(25, 25, 178, 77)), //TRANSPARENT BOX left
				Triangle(Vector3d(1.0, -1.0, -1.0), Vector3d(1.0, -1.0, -3.0), Vector3d(1.0, 1.0, -3.0), SurfaceType::Transparent, 0.1, Color(25, 25, 178, 77)),

				Triangle(Vector3d(3.0, -1.0, -1.0), Vector3d(3.0, 1.0, -1.0), Vector3d(3.0, 1.0, -3.0), SurfaceType::Transparent, 0.1, Color(25, 25, 178, 77)), //TRANSPARENT BOX right
				Triangle(Vector3d(3.0, 1.0, -3.0), Vector3d(3.0, -1.0, -3.0), Vector3d(3.0, -1.0, -1.0), SurfaceType::Transparent, 0.1, Color(25, 25, 178, 77)),
			};*/

			for (auto i = _geometry.createIterator(); i; i++)
			{
				StaticMeshComponent* component = *i;

				LOGF(Info, CoreEngine, "component: %s", component->getOwner()->getName().c_str());

				Matrix4d transform = component->getWorldTransform().getMatrix();
				Matrix4d normalTransform = transform.inverse().transpose();

				Matrix4d p;
				Vector4d v1;
				Vector4d v = p * v1;

				Mesh* mesh = component->getStaticMesh()->getMesh();
				VertexBuffer* vbo = mesh->getVertexBuffer();

				std::vector<float> vertexData;

				VertexFormat format = mesh->getVertexFormat();
				uint32 numVertices = mesh->getNumVertices();
				uint32 vertexFloats = format.getVertexSize() / sizeof(float);

				uint32 posPosition = 0;
				uint32 normalPosition = 3;
				uint32 uvPosition = 6;
				uint32 lightmapUVPosition = 8;

				uint32 pos = 0;
				for (int j = 0; j < format.getNumAttributes(); j++)
				{
					const VertexFormat::Attribute& attr = format.getAttribute(j);
					if (attr.getType() == VertexFormat::Semantic::Position)
					{
						posPosition = pos;
					}
					else if (attr.getType() >= VertexFormat::Semantic::TexCoord0
						&& attr.getType() <= VertexFormat::Semantic::TexCoord6)
					{
						uvPosition = pos;
					}
					else if (attr.getType() == VertexFormat::Semantic::LightmapUVs)
					{
						lightmapUVPosition = pos;
					}
					else if (attr.getType() == VertexFormat::Semantic::Normal)
					{
						normalPosition = pos;
					}

					pos += format.getAttribute(j).getSize();
				}

				vertexData.resize(numVertices * vertexFloats);

				vbo->bind();
				glGetBufferSubData(GL_ARRAY_BUFFER, 0, numVertices * format.getVertexSize(), &vertexData[0]);
				vbo->unbind();

				std::vector<std::vector<float>> vertices;

				for (int j = 0; j < numVertices * vertexFloats; j += vertexFloats)
				{
					std::vector<float> vertex;
					for (int k = 0; k < vertexFloats; k++)
					{
						vertex.push_back(vertexData[j + k]);
					}
					vertices.push_back(vertex);
				}

				for (uint32 j = 0; j < mesh->getNumIndexComponents(); j++)
				{
					MeshIndexComponent* indexComponent = mesh->getIndexComponent(j);

					IndexBuffer* ibo = indexComponent->getIndexBuffer();

					std::vector<uint32> indexData;

					indexData.resize(ibo->getIndexCount());

					ibo->bind();
					glGetBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, ibo->getIndexCount() * sizeof(uint32), &indexData[0]);
					ibo->unbind();

					for (int k = 0; k < ibo->getIndexCount(); k += 3)
					{
						std::vector<float> vertexA = vertices[indexData[k + 2]];
						std::vector<float> vertexB = vertices[indexData[k + 1]];
						std::vector<float> vertexC = vertices[indexData[k + 0]];

						Vector4d a = transform * Vector4d(vertexA[posPosition], vertexA[posPosition + 1], vertexA[posPosition + 2], 1.0);
						Vector4d b = transform * Vector4d(vertexB[posPosition], vertexB[posPosition + 1], vertexB[posPosition + 2], 1.0);
						Vector4d c = transform * Vector4d(vertexC[posPosition], vertexC[posPosition + 1], vertexC[posPosition + 2], 1.0);

						Vector2f auv = Vector2f(vertexA[uvPosition], vertexA[uvPosition + 1]);
						Vector2f buv = Vector2f(vertexB[uvPosition], vertexB[uvPosition + 1]);
						Vector2f cuv = Vector2f(vertexC[uvPosition], vertexC[uvPosition + 1]);

						Vector4d anorm = normalTransform * Vector4d(vertexA[normalPosition], vertexA[normalPosition + 1], vertexA[normalPosition + 2], 0.0);
						Vector4d bnorm = normalTransform * Vector4d(vertexB[normalPosition], vertexB[normalPosition + 1], vertexB[normalPosition + 2], 0.0);
						Vector4d cnorm = normalTransform * Vector4d(vertexC[normalPosition], vertexC[normalPosition + 1], vertexC[normalPosition + 2], 0.0);

						Material* m = component->getStaticMeshMaterial();
						LinearColor lc = LinearColor(0.5f, 0.5f, 0.5f);
						LinearColor ec = LinearColor(0.0f, 0.0f, 0.0f, 0.0f);
						double reflectivity = 0.5;

						if (m)
						{
							for (int l = 0; l < m->getTechniqueCount(); l++)
							{
								MaterialAttribute* attr = m->getBaseColor(l);

								if (attr)
								{
									if (attr->getType() == MaterialAttribute::Type::Vector3)
									{
										Vector3f v = attr->getVector3Value();

										lc.R = v.x;
										lc.G = v.y;
										lc.B = v.z;

										break;
									}
									else if (attr->getType() == MaterialAttribute::Type::Vector4)
									{
										Vector4f v = attr->getVector4Value();

										lc.R = v.x;
										lc.G = v.y;
										lc.B = v.z;
										lc.A = v.w;

										break;
									}
								}

								attr = m->getRoughness(l);

								if (attr)
								{
									if (attr->getType() == MaterialAttribute::Type::Number)
									{
										float v = attr->getFloatValue();

										reflectivity = 1.0 - (double)v;

										break;
									}
								}

								attr = m->getEmissive(l);

								if (attr)
								{
									if (attr->getType() == MaterialAttribute::Type::Vector3)
									{
										Vector3f v = attr->getVector3Value();

										ec.R = v.x;
										ec.G = v.y;
										ec.B = v.z;

										break;
									}
									else if (attr->getType() == MaterialAttribute::Type::Vector4)
									{
										Vector4f v = attr->getVector4Value();

										ec.R = v.x;
										ec.G = v.y;
										ec.B = v.z;
										ec.A = v.w;

										break;
									}
								}
							}
						}

						SurfaceType type = component->getStaticMesh()->isTransparent() ? SurfaceType::Transparent : SurfaceType::Diffusive;

						Triangle triangle(a.xyz(), b.xyz(), c.xyz(), anorm.xyz(), bnorm.xyz(), cnorm.xyz(), auv, buv, cuv, type, reflectivity, lc.toColor(false), ec.toColor(false));
						triangle.parent = component;
						triangles.push_back(triangle);
					}
				}
			}

			LOG(Info, CoreEngine, "done");
		}

		void LightProcessor::cleanup()
		{
			for (auto iter = _geometry.createIterator(); iter; iter++)
			{
				AE_RELEASE(*iter);
			}

			for (auto iter = _directionalLights.createIterator(); iter; iter++)
			{
				AE_RELEASE(*iter);
			}

			for (auto iter = _pointLights.createIterator(); iter; iter++)
			{
				AE_RELEASE(*iter);
			}

			for (auto iter = _spotLights.createIterator(); iter; iter++)
			{
				AE_RELEASE(*iter);
			}

			//for (auto iter = _rectLights.createIterator(); iter; iter++)
			//{
			//	AE_RELEASE(*iter);
			//}
		}

		void LightProcessor::finalize()
		{
		}

		void LightProcessor::setColorFloat(uint8* data, Vector3d position, Vector3i size, const Color& color, const AxisAlignedBoundingBoxf& bounds, uint32 components)
		{
			if (position.x > bounds.getMax().x
				|| position.y > bounds.getMax().y
				|| position.z > bounds.getMax().z
				|| position.x < bounds.getMin().x
				|| position.y < bounds.getMin().y
				|| position.z < bounds.getMin().z)
			{
				return;
			}

			position.x = Math::range(position.x, (double)bounds.getMin().x, (double)bounds.getMax().x, 0.0, (double)size.x);
			position.y = Math::range(position.y, (double)bounds.getMin().y, (double)bounds.getMax().y, 0.0, (double)size.y);
			position.z = Math::range(position.z, (double)bounds.getMin().z, (double)bounds.getMax().z, 0.0, (double)size.z);

			Vector3i integer = position.cast<int>();

			if (integer.x >= 0 && integer.y >= 0 && integer.z >= 0
				&& integer.x < size.x && integer.y < size.y && integer.z < size.z)
			{
				double distance0 = Vector3d::distance(position, integer.cast<double>());
				//setColor(data, integer, size, LinearColor::lerp(color.asLinear(), getLinearColor(data, integer, size, components), distance0).toColor(false), components);
				setColor(data, integer, size, color, components);
				//LOGF(Info, CoreEngine, "distance0: %f", distance0);
			}

			/*if (integer.x < size.x - 1)
			{
				Vector3i integerXP = Vector3i(integer.x + 1, integer.y, integer.z);
				double distanceXP = Vector3d::distance(position, integerXP.cast<double>());
				//LOGF(Info, CoreEngine, "distanceXP: %f", distanceXP);
				setColor(data, integerXP, size, LinearColor::lerp(color.asLinear(), getLinearColor(data, integerXP, size, components), distanceXP).toColor(false), components);
			}
			if (integer.x > 0)
			{
				Vector3i integerXM = Vector3i(integer.x - 1, integer.y, integer.z);
				double distanceXM = Vector3d::distance(position, integerXM.cast<double>());
				//LOGF(Info, CoreEngine, "distanceXM: %f", distanceXM);
				setColor(data, integerXM, size, LinearColor::lerp(color.asLinear(), getLinearColor(data, integerXM, size, components), distanceXM).toColor(false), components);
			}
			if (integer.y < size.y - 1)
			{
				Vector3i integerYP = Vector3i(integer.x, integer.y + 1, integer.z);
				double distanceYP = Vector3d::distance(position, integerYP.cast<double>());
				//LOGF(Info, CoreEngine, "distanceYP: %f", distanceYP);
				setColor(data, integerYP, size, LinearColor::lerp(color.asLinear(), getLinearColor(data, integerYP, size, components), distanceYP).toColor(false), components);
			}
			if (integer.y > 0)
			{
				Vector3i integerYM = Vector3i(integer.x, integer.y - 1, integer.z);
				double distanceYM = Vector3d::distance(position, integerYM.cast<double>());
				//LOGF(Info, CoreEngine, "distanceYM: %f", distanceYM);
				setColor(data, integerYM, size, LinearColor::lerp(color.asLinear(), getLinearColor(data, integerYM, size, components), distanceYM).toColor(false), components);
			}
			if (integer.z < size.z - 1)
			{
				Vector3i integerZP = Vector3i(integer.x, integer.y, integer.z + 1);
				double distanceZP = Vector3d::distance(position, integerZP.cast<double>());
				//LOGF(Info, CoreEngine, "distanceZP: %f", distanceZP);
				setColor(data, integerZP, size, LinearColor::lerp(color.asLinear(), getLinearColor(data, integerZP, size, components), distanceZP).toColor(false), components);
			}
			if (integer.z > 0)
			{
				Vector3i integerZM = Vector3i(integer.x, integer.y, integer.z - 1);
				double distanceZM = Vector3d::distance(position, integerZM.cast<double>());
				//LOGF(Info, CoreEngine, "distanceZM: %f", distanceZM);
				setColor(data, integerZM, size, LinearColor::lerp(color.asLinear(), getLinearColor(data, integerZM, size, components), distanceZM).toColor(false), components);
			}*/
		}

		void LightProcessor::addColorFloat(uint8* data, Vector3d position, Vector3i size, const Color& color, const AxisAlignedBoundingBoxf& bounds, uint32 components)
		{
			if (position.x > bounds.getMax().x
				|| position.y > bounds.getMax().y
				|| position.z > bounds.getMax().z
				|| position.x < bounds.getMin().x
				|| position.y < bounds.getMin().y
				|| position.z < bounds.getMin().z)
			{
				return;
			}

			position.x = Math::range(position.x, (double)bounds.getMin().x, (double)bounds.getMax().x, 0.0, (double)size.x);
			position.y = Math::range(position.y, (double)bounds.getMin().y, (double)bounds.getMax().y, 0.0, (double)size.y);
			position.z = Math::range(position.z, (double)bounds.getMin().z, (double)bounds.getMax().z, 0.0, (double)size.z);

			Vector3i integer = position.cast<int>();

			if (integer.x >= 0 && integer.y >= 0 && integer.z >= 0
				&& integer.x < size.x && integer.y < size.y && integer.z < size.z)
			{
				double distance0 = Vector3d::distance(position, integer.cast<double>());
				//setColor(data, integer, size, LinearColor::lerp(color.asLinear(), getLinearColor(data, integer, size, components), distance0).toColor(false), components);

				LinearColor c = getLinearColor(data, integer, size, components);

				setColor(data, integer, size, (color.asLinear() + c).toColor(false), components);
				//LOGF(Info, CoreEngine, "distance0: %f", distance0);
			}
		}

		void LightProcessor::setColor(uint8* data, Vector3i position, Vector3i size, const Color& color, uint32 components)
		{
			if (data)
			{
				if (position.x >= 0 && position.y >= 0 && position.z >= 0
					&& position.x < size.x && position.y < size.y && position.z < size.z)
				{
					data[(position.x + size.x * (position.y + size.z * position.z)) * components + 0] = color.R;
					data[(position.x + size.x * (position.y + size.z * position.z)) * components + 1] = color.G;
					data[(position.x + size.x * (position.y + size.z * position.z)) * components + 2] = color.B;
					if (components == 4)
					{
						data[(position.x + size.x * (position.y + size.z * position.z)) * components + 3] = color.A;
					}
				}
			}
		}

		void LightProcessor::setColor(uint8* data, uint8 x, uint8 y, uint8 z, uint8 size, const Color& color, uint32 components)
		{
			setColor(data, Vector3i(x, y, z), Vector3i(size), color, components);
		}

		void LightProcessor::setRayColor(uint8* data, const Ray& ray, Vector3i size, uint32 partitions, const Color& color, const AxisAlignedBoundingBoxf& bounds, uint32 components)
		{
			double l = ray.length;

			for (uint32 i = 0; i <= partitions; i++)
			{
				Vector3d position = ray.origin + ray.direction * double(i) * l / double(partitions);
				setColorFloat(data, position, size, color, bounds, components);
				//LOGF(Info, CoreEngine, "position: %f, %f, %f", position.x, position.y, position.z);
			}
		}

		void LightProcessor::addRayColor(uint8* data, const Ray& ray, Vector3i size, uint32 partitions, const Color& color, const AxisAlignedBoundingBoxf& bounds, uint32 components)
		{
			double l = ray.length;

			for (uint32 i = 0; i <= partitions; i++)
			{
				Vector3d position = ray.origin + ray.direction * double(i) * l / double(partitions);
				addColorFloat(data, position, size, color, bounds, components);
				//LOGF(Info, CoreEngine, "position: %f, %f, %f", position.x, position.y, position.z);
			}
		}

		Color LightProcessor::getColor(uint8* data, Vector3i position, Vector3i size, uint32 components)
		{
			Color color;

			if (data)
			{
				if (position.x >= 0 && position.y >= 0 && position.z >= 0
					&& position.x < size.x && position.y < size.y && position.z < size.z)
				{
					color = Color(data[(position.x + size.x * (position.y + size.z * position.z)) * components + 0],
						data[(position.x + size.x * (position.y + size.z * position.z)) * components + 1],
						data[(position.x + size.x * (position.y + size.z * position.z)) * components + 2],
						components == 4 ? data[(position.x + size.x * (position.y + size.z * position.z)) * components + 3] : 255);
				}
			}

			return color;
		}

		LinearColor LightProcessor::getLinearColor(uint8* data, Vector3i position, Vector3i size, uint32 components)
		{
			return getColor(data, position, size, components).asLinear();
		}

		uint8* LightProcessor::blur(uint8* data, Vector3i size, Vector3i blur, uint32 components)
		{
			uint8* newData = new uint8[size.x * size.y * size.z * components];

			uint32 xblur = blur.x;
			uint32 yblur = blur.y;
			uint32 zblur = blur.z;

			xblur |= 1;
			yblur |= 1;
			zblur |= 1;


			//PASS 1   X-Y/////////////////////

			// horizontal blur
			for (int z = 0; z < size.z; z++)
			{
				float weight = 1.0f / float(xblur);
				int32 half = xblur / 2;
				for (int y = 0; y < size.y; ++y)
				{
					for (int x = 0; x < size.x; ++x)
					{
						LinearColor blurredColor;
						for (int i = -half; i <= half; ++i)
						{
							LinearColor color = getLinearColor(data, Vector3i(x + i, y, z), size, components);
							blurredColor.R += color.R * weight;
							blurredColor.G += color.G * weight;
							blurredColor.B += color.B * weight;
						}

						setColor(newData, Vector3i(x, y, z), size, blurredColor.toColor(false), components);
					}
				}
			}

			// vertical blur
			for (int z = 0; z < size.z; z++)
			{
				float weight = 1.0f / float(yblur);
				int32 half = yblur / 2;
				for (int y = 0; y < size.y; ++y)
				{
					for (int x = 0; x < size.x; ++x)
					{
						LinearColor blurredColor;
						for (int i = -half; i <= half; ++i)
						{
							LinearColor color = getLinearColor(data, Vector3i(x, y + i, z), size, components);
							blurredColor.R += color.R * weight;
							blurredColor.G += color.G * weight;
							blurredColor.B += color.B * weight;
						}

						setColor(newData, Vector3i(x, y, z), size, blurredColor.toColor(false), components);
					}
				}
			}

			//PASS 1   X-Y/////////////////////

			//PASS 2   X-Z/////////////////////

			// horizontal blur
			for (int y = 0; y < size.y; y++)
			{
				float weight = 1.0f / float(xblur);
				int32 half = xblur / 2;
				for (int z = 0; z < size.z; ++z)
				{
					for (int x = 0; x < size.x; ++x)
					{
						LinearColor blurredColor;
						for (int i = -half; i <= half; ++i)
						{
							LinearColor color = getLinearColor(newData, Vector3i(x + i, y, z), size, components);
							blurredColor.R += color.R * weight;
							blurredColor.G += color.G * weight;
							blurredColor.B += color.B * weight;
						}

						setColor(newData, Vector3i(x, y, z), size, blurredColor.toColor(false), components);
					}
				}
			}

			// vertical blur
			for (int y = 0; y < size.y; y++)
			{
				float weight = 1.0f / float(zblur);
				int32 half = zblur / 2;
				for (int z = 0; z < size.z; ++z)
				{
					for (int x = 0; x < size.x; ++x)
					{
						LinearColor blurredColor;
						for (int i = -half; i <= half; ++i)
						{
							LinearColor color = getLinearColor(newData, Vector3i(x, y, z + i), size, components);
							blurredColor.R += color.R * weight;
							blurredColor.G += color.G * weight;
							blurredColor.B += color.B * weight;
						}

						setColor(newData, Vector3i(x, y, z), size, blurredColor.toColor(false), components);
					}
				}
			}

			//PASS 2   X-Z/////////////////////

			delete[] data;

			return newData;
		}
	}
}