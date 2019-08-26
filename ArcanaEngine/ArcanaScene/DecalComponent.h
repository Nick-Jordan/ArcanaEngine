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

	class ARCANA_SCENE_API Decal
	{
		friend class DecalComponent;

	public:

		Decal();

		~Decal();

		double getOpacity() const;

		const Transform& getTransform() const;

		const Color& getColor() const;

		Vector4f getTexCoords() const;

		//bool isActive() const;

	private:

		bool _active;
		double _lifetime;
		double _currentTime;
		double _opacity;

		//test
		Transform _transform;
		Color _color;
		Vector4f _texCoords;
	};

	class ARCANA_SCENE_API DecalComponent : public GeometryComponent
	{
	public:

		DecalComponent();

		DecalComponent(const DecalProperties& properties, uint32 maxDecals = 100);

		virtual ~DecalComponent();

		void initialize(const DecalProperties& properties, uint32 maxDecals);

		virtual void initialize() override;

		virtual bool createRenderProcedure() override;

		virtual void update(double elapsedTime) override;

		virtual void updateRenderProcedure() override;

		void setMaxDecals(uint32 maxDecals);

		uint32 getMaxDecals() const;

		uint32 getNumDecals() const;

		//test
		void addDecal(const Transform& transform, Color color = Color(255, 255, 255, 255), double opacity = 1.0,
			double lifetime = -1.0, Vector2f texCoordMin = Vector2f::zero(), Vector2f texCoordMax = Vector2f::one());

	private:

		DecalProperties _properties;

		DecalRenderProcedure* _decalRenderProcedure;

		Decal* _decals;

		uint32 _maxDecals;
		uint32 _numDecals;
	};

}

#endif // !DECAL_COMPONENT_H_