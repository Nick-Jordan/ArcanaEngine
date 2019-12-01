#include "LightComponent.h"

namespace Arcana
{
	LightComponent::LightComponent() : BaseLightComponent(),
		_affectDynamicIndirectLighting(false),
		_enableLightShaftBloom(true),
		_bloomScale(1.0f),
		_bloomThreshold(0.01f),
		_bloomTint(255, 255, 255),
		_useDistanceFieldShadows(false),
		_useTemperature(false),
		_contactShadowLength(1.0),
		_disabledBrightness(0.0),
		_lightFunctionFadeDistance(1.0),
		_lightFunctionScale(1.0, 1.0, 1.0),
		_maxDistanceFadeRange(0.0),
		_maxDrawDistance(1000.0),
		_rayStartOffsetDepthScale(0.0),
		_shadowBias(0.5),
		_shadowResolutionScale(1.0),
		_shadowSharpen(0.0),
		_specularScale(1.0),
		_temperature(1000.0)
	{
	}

	LightComponent::LightComponent(const LightComponent& lightComponent) : BaseLightComponent(lightComponent),
		_affectDynamicIndirectLighting(lightComponent._affectDynamicIndirectLighting),
		_enableLightShaftBloom(lightComponent._enableLightShaftBloom),
		_bloomScale(lightComponent._bloomScale),
		_bloomThreshold(lightComponent._bloomThreshold),
		_bloomTint(lightComponent._bloomTint),
		_useDistanceFieldShadows(lightComponent._useDistanceFieldShadows),
		_useTemperature(lightComponent._useTemperature),
		_contactShadowLength(lightComponent._contactShadowLength),
		_disabledBrightness(lightComponent._disabledBrightness),
		_lightFunctionFadeDistance(lightComponent._lightFunctionFadeDistance),
		_lightFunctionScale(lightComponent._lightFunctionScale),
		_maxDistanceFadeRange(lightComponent._maxDistanceFadeRange),
		_maxDrawDistance(lightComponent._maxDrawDistance),
		_rayStartOffsetDepthScale(lightComponent._rayStartOffsetDepthScale),
		_shadowBias(lightComponent._shadowBias),
		_shadowResolutionScale(lightComponent._shadowBias),
		_shadowSharpen(lightComponent._shadowSharpen),
		_specularScale(lightComponent._specularScale),
		_temperature(lightComponent._temperature)
	{

	}

	LightComponent::~LightComponent()
	{
	}

	Vector4d LightComponent::getLightPosition()
	{
		return Vector4d::zero();
	}

	Vector3d LightComponent::getLightDirection()
	{
		return Vector3d::normalize(getWorldTransform().getForwardVector());
	}


	bool LightComponent::getDynamicIndirectLighting() const
	{
		return _affectDynamicIndirectLighting;
	}

	bool LightComponent::getLightShaftBloomEnabled() const
	{
		return _enableLightShaftBloom;
	}

	float LightComponent::getBloomScale() const
	{
		return _bloomScale;
	}

	float LightComponent::getBloomThreshold() const
	{
		return _bloomThreshold;
	}

	const Color& LightComponent::getBloomTint() const
	{
		return _bloomTint;
	}

	bool LightComponent::getUseDistanceFieldShadows() const
	{
		return _useDistanceFieldShadows;
	}

	bool LightComponent::getUseTemperature() const
	{
		return _useTemperature;
	}

	float LightComponent::getContactShadowLength() const
	{
		return _contactShadowLength;
	}

	float LightComponent::getDisabledBrightness() const
	{
		return _disabledBrightness;
	}

	float LightComponent::getLightFunctionFadeDistance() const
	{
		return _lightFunctionFadeDistance;
	}

	const Vector3f& LightComponent::getLightFunctionScale() const
	{
		return _lightFunctionScale;
	}

	float LightComponent::getMaxDistanceFadeRange() const
	{
		return _maxDistanceFadeRange;
	}

	float LightComponent::getMaxDrawDistance() const
	{
		return _maxDrawDistance;
	}

	float LightComponent::getRayStartOffsetDepthScale() const
	{
		return _rayStartOffsetDepthScale;
	}

	float LightComponent::getShadowBias() const
	{
		return _shadowBias;
	}

	float LightComponent::getShadowResolutionScale() const
	{
		return _shadowResolutionScale;
	}

	float LightComponent::getShadowSharpen() const
	{
		return _shadowSharpen;
	}

	float LightComponent::getSpecularScale() const
	{
		return _specularScale;
	}

	float LightComponent::getTemperature() const
	{
		return _temperature;
	}


	void LightComponent::setDynamicIndirectLighting(bool affectDynamicIndirectLighting)
	{
		_affectDynamicIndirectLighting = affectDynamicIndirectLighting;
	}

	void LightComponent::setLightShaftBloomEnabled(bool enableLightShaftBloom)
	{
		_enableLightShaftBloom = enableLightShaftBloom;
	}

	void LightComponent::setBloomScale(float bloomScale)
	{
		_bloomScale = bloomScale;
	}

	void LightComponent::setBloomThreshold(float bloomThreshold)
	{
		_bloomThreshold = bloomThreshold;
	}

	void LightComponent::setBloomTint(const Color& color)
	{
		_bloomTint = color;
	}

	void LightComponent::setUseDistanceFieldShadows(bool distanceFieldShadows)
	{
		_useDistanceFieldShadows = distanceFieldShadows;
	}

	void LightComponent::setUseTemperature(bool useTemperature)
	{
		_useTemperature = useTemperature;
	}

	void LightComponent::setContactShadowLength(float contactShadowLength)
	{
		_contactShadowLength = contactShadowLength;
	}

	void LightComponent::setDisabledBrightness(float disabledBrightness)
	{
		_disabledBrightness = disabledBrightness;
	}

	void LightComponent::setLightFunctionFadeDistance(float lightFunctionFadeDistance)
	{
		_lightFunctionFadeDistance = lightFunctionFadeDistance;
	}

	void LightComponent::setLightFunctionScale(const Vector3f& lightFunctionScale)
	{
		_lightFunctionScale = lightFunctionScale;
	}

	void LightComponent::setMaxDistanceFadeRange(float maxDistanceFadeRange)
	{
		_maxDistanceFadeRange = maxDistanceFadeRange;
	}

	void LightComponent::setMaxDrawDistance(float maxDrawDistance)
	{
		_maxDrawDistance = maxDrawDistance;
	}

	void LightComponent::setRayStartOffsetDepthScale(float rayStartOffsetDepthScale)
	{
		_rayStartOffsetDepthScale = rayStartOffsetDepthScale;
	}

	void LightComponent::setShadowBias(float shadowBias)
	{
		_shadowBias = shadowBias;
	}

	void LightComponent::setShadowResolutionScale(float shadowResolutionScale)
	{
		_shadowResolutionScale = shadowResolutionScale;
	}

	void LightComponent::setShadowSharpen(float shadowSharpen)
	{
		_shadowSharpen = shadowSharpen;
	}

	void LightComponent::setSpecularScale(float specularScale)
	{
		_specularScale = specularScale;
	}

	void LightComponent::setTemperature(float temperature)
	{
		_temperature = temperature;
	}
}