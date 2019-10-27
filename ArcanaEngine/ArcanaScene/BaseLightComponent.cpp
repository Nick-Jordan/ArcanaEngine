#include "BaseLightComponent.h"

namespace Arcana
{

	BaseLightComponent::BaseLightComponent()
	{
	}

	BaseLightComponent::BaseLightComponent(const Properties& properties) : _properties(properties)
	{

	}

	BaseLightComponent::~BaseLightComponent()
	{
	}

	LightType BaseLightComponent::getLightType() const
	{
		return NumLightTypes;
	}

	RenderLight BaseLightComponent::createRenderLight()
	{
		return RenderLight();
	}

	const BaseLightComponent::Properties& BaseLightComponent::getProperties() const
	{
		return _properties;
	}

	void BaseLightComponent::setProperties(const Properties& properties)
	{
		_properties = properties;
	}

	LinearColor BaseLightComponent::getLightColor() const
	{
		return _properties.LightColor.asLinear();
	}

	float BaseLightComponent::getIntensity() const
	{
		return _properties.Intensity;
	}

	float BaseLightComponent::getVolumetricScatteringIntensity() const
	{
		return _properties.VolumetricScatteringIntensity;
	}

	float BaseLightComponent::getIndirectLightingIntensity() const
	{
		return _properties.IndirectLightingIntensity;
	}

	bool BaseLightComponent::hasStaticShadows() const
	{
		return _properties.CastStaticShadows;
	}

	bool BaseLightComponent::hasDynamicShadows() const
	{
		return _properties.CastDynamicShadows;
	}

	bool BaseLightComponent::hasVolumetricShadows() const
	{
		return _properties.CastVolumetricShadow;
	}

	bool BaseLightComponent::hasShadows() const
	{
		return _properties.CastStaticShadows || _properties.CastDynamicShadows || _properties.CastVolumetricShadow;
	}

	bool BaseLightComponent::hasSubsurfaceTransmission() const
	{
		return _properties.SubsurfaceTransmission;
	}

	void BaseLightComponent::setLightColor(const LinearColor& color)
	{
		_properties.LightColor = color.toColor(false);
	}

	void BaseLightComponent::setLightColor(const Color& color)
	{
		_properties.LightColor = color;
	}

	void BaseLightComponent::setIntensity(float intensity)
	{
		_properties.Intensity = intensity;
	}

	void BaseLightComponent::setVolumetricScatteringIntensity(float intensity)
	{
		_properties.VolumetricScatteringIntensity = intensity;
	}

	void BaseLightComponent::setIndirectLightingIntensity(float intensity)
	{
		_properties.IndirectLightingIntensity = intensity;
	}

	void BaseLightComponent::setStaticShadows(bool staticShadows)
	{
		_properties.CastStaticShadows = staticShadows;
	}

	void BaseLightComponent::setDynamicShadows(bool dynamicShadows)
	{
		_properties.CastDynamicShadows = dynamicShadows;
	}

	void BaseLightComponent::setVolumetricShadows(bool volumetricShadows)
	{
		_properties.CastVolumetricShadow = volumetricShadows;
	}

	void BaseLightComponent::setSubsurfaceTransmission(bool subsurfaceTransmission)
	{
		_properties.SubsurfaceTransmission = subsurfaceTransmission;
	}

	BaseLightComponent::Properties::Properties() :
		SubsurfaceTransmission(false),
		CastVolumetricShadow(false),
		CastDynamicShadows(false),
		CastStaticShadows(false),
		IndirectLightingIntensity(0.0),
		LightColor(255, 255, 255),
		Intensity(1.0),
		VolumetricScatteringIntensity(0.0)
	{
	}
}
