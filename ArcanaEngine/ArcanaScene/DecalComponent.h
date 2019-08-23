#ifndef DECAL_COMPONENT_H_
#define DECAL_COMPONENT_H_

#include "SceneDefines.h"

#include "GeometryComponent.h"

namespace Arcana
{
	class ARCANA_SCENE_API DecalProperties
	{
		friend class DecalRenderProcedure;

	public:

		DecalProperties();

		DecalProperties(const DecalProperties& copy);

		~DecalProperties();

		//need accessors

		void setNormalsTexture(Texture* texture, float blend = 1.0f);
		void setAlbedoTexture(Texture* texture, float blend = 1.0f);
		void setSpecularTexture(Texture* texture, float blend = 1.0f);
		void setRoughnessTexture(Texture* texture, float blend = 1.0f);
		void setEmissiveTexture(Texture* texture, float blend = 1.0f);
		void setMetallicTexture(Texture* texture, float blend = 1.0f);
		void setAmbientOcclusionTexture(Texture* texture, float blend = 1.0f);

		void setNormalsBlendFactor(float blend);
		void setAlbedoBlendFactor(float blend);
		void setSpecularBlendFactor(float blend);
		void setRoughnessBlendFactor(float blend);
		void setEmissiveBlendFactor(float blend);
		void setMetallicBlendFactor(float blend);
		void setAmbiendOcclusionBlendFactor(float blend);

		void setAngleCutoff(float angleCutoff);

		void setColor(const Color& color);

		DecalProperties& operator=(const DecalProperties& rhs);

	private:

		Texture* _normals;
		Texture* _albedo;
		Texture* _specular;
		Texture* _roughness;
		Texture* _emissive;
		Texture* _metallic;
		Texture* _ao;
		float _normalsBlend;
		float _albedoBlend;
		float _specularBlend;
		float _roughnessBlend;
		float _emissiveBlend;
		float _metallicBlend;
		float _aoBlend;

		float _angleCutoff;
		Color _color;
	};

	class ARCANA_SCENE_API DecalComponent : public GeometryComponent
	{
	public:

		DecalComponent();

		DecalComponent(const DecalProperties& properties);

		virtual ~DecalComponent();

		void initialize(const DecalProperties& properties);

		virtual void initialize() override;

		virtual bool createRenderProcedure() override;

	private:

		DecalProperties _properties;
	};

}

#endif // !DECAL_COMPONENT_H_