#include "FasterThanLight.h"

#include "TextureAttachment.h"
#include "DepthStencilAttachment.h"
#include "MeshRenderProcedure.h"
#include "MeshIndexComponent.h"

namespace Arcana
{
	namespace FTL
	{
		LightProcessor::LightProcessor(World* world)
		{
			for (uint32 i = 0; i < world->getNumActors(); i++)
			{
				Actor* actor = world->getActor(i);

				if (actor->getMobility() == Actor::Mobility::Static)
				{
					Array<GeometryComponent*> geometryComponents;
					actor->getComponents(geometryComponents);

					for (auto iter = geometryComponents.createConstIterator(); iter; iter++)
					{
						GeometryComponent* comp = *iter;

						if (comp && comp->isActive() && comp->hasRenderProcedure())
						{
							addGeometry(comp);
						}
					}

					Array<BaseLightComponent*> lightComponents;
					actor->getComponents(lightComponents);

					for (auto iter = lightComponents.createConstIterator(); iter; iter++)
					{
						BaseLightComponent* comp = *iter;

						if (comp && comp->isActive())
						{
							addLight(comp);
						}
					}
				}
			}

			createDirectionalShadowMaps();
		}

		LightProcessor::~LightProcessor()
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

		void LightProcessor::addGeometry(GeometryComponent* geometry)
		{
			if (geometry)
			{
				geometry->reference();
				_geometry.add(geometry);
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

		void LightProcessor::createDirectionalShadowMaps()
		{
			/*Shader depthShader;
			depthShader.createProgram(Shader::Vertex, "resources/ftl/shadow_mapping_depth_vert.glsl");
			depthShader.createProgram(Shader::Fragment, "resources/ftl/shadow_mapping_depth_frag.glsl");

			Framebuffer* depthFramebuffer = new Framebuffer("depthFBO");

			const uint32 SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

			Texture::Parameters params;
			params.setMinFilter(TextureFilter::Nearest);
			params.setMagFilter(TextureFilter::Nearest);
			params.setWrapS(TextureWrap::ClampBorder);
			params.setWrapT(TextureWrap::ClampBorder);
			params.setBorderf(1.0, 1.0, 1.0, 1.0);
			Texture* depthMap = Texture::create2D(Texture::RGBA, SHADOW_WIDTH, SHADOW_HEIGHT, Texture::RGBA16F, Texture::Float, nullptr, params);

			depthFramebuffer->addAttachment(new TextureAttachment("depthMap", depthMap));


			Framebuffer* prev = depthFramebuffer->bind();

			glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
			glClear(GL_DEPTH_BUFFER_BIT);

			//for every directional light
			DirectionalLightComponent* light = _directionalLights[0];
			processDirectionalLight(depthShader, light);

			Framebuffer::bind(prev);*/




			//for every directional light

				//render with depth shader

				//for all geometry

					//generate shadow map from depth texture
					//store shadow map

			//for all geometry

				//combine all shadow maps into one texture

			Shader depthShader;
			depthShader.createProgram(Shader::Vertex, "resources/ftl/shadow_mapping_depth_vert.glsl");
			depthShader.createProgram(Shader::Fragment, "resources/ftl/shadow_mapping_depth_frag.glsl");

			Shader shadowMapShader;
			shadowMapShader.createProgram(Shader::Vertex, "resources/ftl/shadow_mapping_vert.glsl");
			shadowMapShader.createProgram(Shader::Fragment, "resources/ftl/shadow_mapping_frag.glsl");

			const uint32 SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

			Array<Texture*> finalShadowMaps;

			//for (auto i = _directionalLights.createConstIterator(); i; i++)
			{
				DirectionalLightComponent* light = _directionalLights[0];//*i;

				Texture* depthMap = createDirectionalLightDepthMap(depthShader, light, SHADOW_WIDTH, SHADOW_HEIGHT);

				//Array<Texture*> shadowMaps;

				for (uint32 j = 0; j < _geometry.size(); j++)
				{
					Texture* shadowMap = createDirectionalShadowMap(shadowMapShader, light, depthMap, _geometry[j], SHADOW_WIDTH, SHADOW_HEIGHT);
					finalShadowMaps.add(shadowMap);
				}

				AE_RELEASE(depthMap);
			}

			for (uint32 i = 0; i < _geometry.size(); i++)
			{
				_geometry[i]->_ftlResult.ShadowMap = finalShadowMaps[i];
			}

			for (uint32 i = 0; i < finalShadowMaps.size(); i++)
			{
				Texture* texture = finalShadowMaps[i];

				glBindTexture(texture->getType(), texture->getId());
				uint8* renderTextureData = new uint8[texture->getWidth() * texture->getHeight() * texture->getComponents()];
				glGetTexImage(finalShadowMaps[i]->getType(), 0, finalShadowMaps[i]->getFormat(), finalShadowMaps[i]->getPixelType(), renderTextureData);

				Image<uint8> image;
				image.init(ImageFormat::RGBA, texture->getWidth(), texture->getHeight(), renderTextureData);
				image.save("resources/test/shadow_map" + std::to_string(i) + ".png");

				AE_DELETE_ARRAY(renderTextureData);
			}
		}


		//TEST//////////////////////////////////////////////
		Matrix4f lookAt(Vector3f eye, Vector3f center, Vector3f up)
		{
			Vector3f  f = Vector3f::normalize(center - eye);
			Vector3f  u = Vector3f::normalize(up);
			Vector3f  s = Vector3f::normalize(Vector3f::cross(f, u));
			u = Vector3f::cross(s, f);

			Matrix4f Result;
			Result.set(0, 0, s.x);
			Result.set(1, 0, s.y);
			Result.set(2, 0, s.z);
			Result.set(0, 1, u.x);
			Result.set(1, 1, u.y);
			Result.set(2, 1, u.z);
			Result.set(0, 2, -f.x);
			Result.set(1, 2, -f.y);
			Result.set(2, 2, -f.z);
			Result.set(3, 0, -Vector3f::dot(s, eye));
			Result.set(3, 1, -Vector3f::dot(u, eye));
			Result.set(3, 2, Vector3f::dot(f, eye));
			return Result;
		}

		Texture* LightProcessor::createDirectionalLightDepthMap(Shader& depthShader, DirectionalLightComponent* directionalLight, uint32 width, uint32 height)
		{
			Framebuffer* depthFramebuffer = new Framebuffer("depthFBO");

			Texture::Parameters params;
			params.setMinFilter(TextureFilter::Nearest);
			params.setMagFilter(TextureFilter::Nearest);
			params.setWrapS(TextureWrap::ClampBorder);
			params.setWrapT(TextureWrap::ClampBorder);
			params.setBorderf(1.0, 1.0, 1.0, 1.0);
			Texture* depthMap = Texture::create2D(Texture::Red, width, height, Texture::R16F, Texture::Float, nullptr, params);

			depthFramebuffer->addAttachment(new TextureAttachment("depthMap", depthMap));
			//depthFramebuffer->addAttachment(new DepthStencilAttachment("depthStencil", DepthStencilAttachment::Depth, width, height));

			depthFramebuffer->setDrawBuffer(Framebuffer::None);
			depthFramebuffer->setReadBuffer(Framebuffer::None);

			Framebuffer* prev = depthFramebuffer->bind();

			glViewport(0, 0, width, height);
			glClear(GL_DEPTH_BUFFER_BIT);

			depthShader.bind();

			//get direction from light
			Vector3f direction = Vector3f(0, 0, 10);

			const float nearPlane = 1.0f, farPlane = 7.5f;
			Matrix4f lightProjection = Matrix4f::createOrthographic(20.0f, 20.0f, nearPlane, farPlane);
			Matrix4f lightView = lookAt(direction, Vector3f::zero(), Vector3f::unitY());
			Matrix4f lightSpaceMatrix = lightProjection * lightView;

			depthShader.getUniform("u_LightSpaceMatrix").setValue(lightSpaceMatrix);

			renderGeometry(depthShader);

			depthShader.unbind();

			Framebuffer::bind(prev);

			AE_DELETE(depthFramebuffer);

			return depthMap;
		}

		Texture* LightProcessor::createDirectionalShadowMap(Shader& shadowMapShader, DirectionalLightComponent* directionalLight, 
			Texture* depthMap, GeometryComponent* geometry, uint32 width, uint32 height)
		{
			Framebuffer* framebuffer = new Framebuffer("shadowFBO");

			Texture* shadowMap = Texture::create2D(Texture::RGBA, width, height, Texture::RGBA8, Texture::UnsignedByte, nullptr);

			framebuffer->addAttachment(new TextureAttachment("shadowMap", shadowMap));

			framebuffer->setDrawBuffer(Framebuffer::Color0);

			Framebuffer* prev = framebuffer->bind();

			glViewport(0, 0, width, height);
			glClear(GL_COLOR_BUFFER_BIT);

			Vector3f direction = Vector3f(0, 0, 10);

			const float nearPlane = 1.0f, farPlane = 7.5f;
			Matrix4f lightProjection = Matrix4f::createOrthographic(20.0f, 20.0f, nearPlane, farPlane);
			Matrix4f lightView = lookAt(direction, Vector3f::zero(), Vector3f::unitY());
			Matrix4f lightSpaceMatrix = lightProjection * lightView;

			shadowMapShader.bind();
			shadowMapShader.getUniform("u_LightDirection").setValue(direction);
			shadowMapShader.getUniform("u_LightSpaceMatrix").setValue(lightSpaceMatrix);

			int32 unit = depthMap->bind();
			shadowMapShader.getUniform("u_ShadowDepthMap").setValue(unit);

			renderGeometry(shadowMapShader, geometry);

			shadowMapShader.unbind();

			Framebuffer::bind(prev);

			AE_DELETE(framebuffer);

			return shadowMap;
		}

		void LightProcessor::renderGeometry(Shader& shader)
		{
			for (auto i = _geometry.createConstIterator(); i; i++)
			{
				GeometryComponent* component = *i;

				renderGeometry(shader, component);
			}
		}

		void LightProcessor::renderGeometry(Shader& shader, GeometryComponent* component)
		{
			//SOMEHOW RENDER GEOMETRY

			MeshRenderData* data = dynamic_cast<MeshRenderData*>(component->getRenderProcedure()->getRenderData());

			if (data)
			{
				MeshRenderContext context = data->getContext();

				if (context.isValid())
				{
					context.renderState.bind();
					context.mesh->getVertexBuffer()->bind();

					uint32 componentCount = context.mesh->getNumIndexComponents();

					if (componentCount == 0)
					{
						shader.getUniform("u_ModelMatrix").setValue(component->getWorldTransform().getMatrix().cast<float>());
						shader.getUniform("u_NormalMatrix").setValue(component->getWorldTransform().getMatrix().toMatrix3().inverse().transpose().cast<float>());

						glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
						glDrawArrays(context.mesh->getPrimitive(), 0, context.mesh->getNumVertices());

						shader.unbind();
					}
					else
					{
						for (uint32 c = 0; c < componentCount; c++)
						{
							MeshIndexComponent* indexComponent = context.mesh->getIndexComponent(c);

							shader.getUniform("u_ModelMatrix").setValue(component->getWorldTransform().getMatrix().cast<float>());
							shader.getUniform("u_NormalMatrix").setValue(component->getWorldTransform().getMatrix().toMatrix3().inverse().transpose().cast<float>());

							indexComponent->getIndexBuffer()->bind();
							glDrawElements(indexComponent->getPrimitive(), indexComponent->getNumIndices(), indexComponent->getIndexFormat(), 0);
							indexComponent->getIndexBuffer()->unbind();
						}

						context.mesh->getVertexBuffer()->unbind();
						context.renderState.unbind();
					}
				}
			}
		}
	}
}