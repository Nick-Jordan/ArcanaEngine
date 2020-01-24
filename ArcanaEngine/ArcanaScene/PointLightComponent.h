#ifndef POINT_LIGHT_COMPONENT_H_
#define POINT_LIGHT_COMPONENT_H_

#include "SceneDefines.h"

#include "LocalLightComponent.h"

namespace Arcana
{
	class ARCANA_SCENE_API PointLightComponent : public LocalLightComponent
	{
	public:

		PointLightComponent();

		virtual ~PointLightComponent();

		virtual LightType getLightType() const override;

		virtual RenderLight createRenderLight() override;

		virtual Vector4d getLightPosition() override;

		void useInverseSquareFalloff();

		bool isUsingInverseSquareFalloff() const;

		void setConstantAttenuation(float constant);

		float getConstantAttenuation() const;

		void setLinearAttenuation(float linear);

		float getLinearAttenuation() const;

		void setQuadraticAttenuation(float quadratic);

		float getQuadraticAttenuation() const;

		void setSourceRadius(float radius);
		
		float getSourceRadius() const;

		void setSoftSourceRadius(float radius);

		float getSoftSourceRadius() const;

	private:

		float _softSourceRadius;
		float _sourceRadius;

		float _constantAttenuation;
		float _linearAttenuation;
		float _quadraticAttenuation;
	};

	class PointLightComponentResource : public ResourceCreator<PointLightComponent>
	{
	public:

		PointLightComponentResource(const GlobalObjectID& id, const std::string& type, const ResourceData& data, Scheduler* dependencyScheduler)
			: ResourceCreator<PointLightComponent>(id, type, data, dependencyScheduler)
		{
			initializePointLightComponent(this, data);
		}

		static void initializePointLightComponent(PointLightComponent* light, const ResourceData& data)
		{
			LocalLightComponentResource::initializeLocalLightComponent(light, data);

			light->setSoftSourceRadius(data.getFloatParameter("softSourceRadius"));
			light->setSourceRadius(data.getFloatParameter("sourceRadius"));
			light->setConstantAttenuation(data.getFloatParameter("constant"));
			light->setLinearAttenuation(data.getFloatParameter("linear"));
			light->setQuadraticAttenuation(data.getFloatParameter("quadratic", 1.0f));
		}
	};
}
#endif // !POINT_LIGHT_COMPONENT_H_
