#include "BaseLightComponent.h"

namespace Arcana
{
	BaseLightComponent::BaseLightComponent()  :
		_subsurfaceTransmission(false),
		_castVolumetricShadow(false),
		_castDynamicShadows(false),
		_castStaticShadows(false),
		_indirectLightingIntensity(0.0),
		_lightColor(255, 255, 255),
		_intensity(Math::PI),
		_volumetricScatteringIntensity(0.0)
	{
		_lightId = UUID::newUUID();
	}

	BaseLightComponent::BaseLightComponent(const BaseLightComponent& copy) :
		_subsurfaceTransmission(copy._subsurfaceTransmission),
		_castVolumetricShadow(copy._castVolumetricShadow),
		_castDynamicShadows(copy._castDynamicShadows),
		_castStaticShadows(copy._castStaticShadows),
		_indirectLightingIntensity(copy._indirectLightingIntensity),
		_lightColor(copy._lightColor),
		_intensity(copy._intensity),
		_volumetricScatteringIntensity(copy._volumetricScatteringIntensity)
	{

	}

	/*BaseLightComponent::BaseLightComponent(const Properties& properties) : _properties(properties)
	{

	}*/

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

	const UUID& BaseLightComponent::getLightId() const
	{
		return _lightId;
	}

	/*const BaseLightComponent::Properties& BaseLightComponent::getProperties() const
	{
		return _properties;
	}

	void BaseLightComponent::setProperties(const Properties& properties)
	{
		_properties = properties;
	}*/

	LinearColor BaseLightComponent::getLightColor() const
	{
		return _lightColor.asLinear();
	}

	float BaseLightComponent::getIntensity() const
	{
		return _intensity;
	}

	float BaseLightComponent::getVolumetricScatteringIntensity() const
	{
		return _volumetricScatteringIntensity;
	}

	float BaseLightComponent::getIndirectLightingIntensity() const
	{
		return _indirectLightingIntensity;
	}

	bool BaseLightComponent::hasStaticShadows() const
	{
		return _castStaticShadows;
	}

	bool BaseLightComponent::hasDynamicShadows() const
	{
		return _castDynamicShadows;
	}

	bool BaseLightComponent::hasVolumetricShadows() const
	{
		return _castVolumetricShadow;
	}

	bool BaseLightComponent::hasShadows() const
	{
		return _castStaticShadows || _castDynamicShadows || _castVolumetricShadow;
	}

	bool BaseLightComponent::hasSubsurfaceTransmission() const
	{
		return _subsurfaceTransmission;
	}

	void BaseLightComponent::setLightColor(const LinearColor& color)
	{
		_lightColor = color.toColor(false);
	}

	void BaseLightComponent::setLightColor(const Color& color)
	{
		_lightColor = color;
	}

	void BaseLightComponent::setIntensity(float intensity)
	{
		_intensity = intensity;
	}

	void BaseLightComponent::setVolumetricScatteringIntensity(float intensity)
	{
		_volumetricScatteringIntensity = intensity;
	}

	void BaseLightComponent::setIndirectLightingIntensity(float intensity)
	{
		_indirectLightingIntensity = intensity;
	}

	void BaseLightComponent::setStaticShadows(bool staticShadows)
	{
		_castStaticShadows = staticShadows;
	}

	void BaseLightComponent::setDynamicShadows(bool dynamicShadows)
	{
		_castDynamicShadows = dynamicShadows;
	}

	void BaseLightComponent::setVolumetricShadows(bool volumetricShadows)
	{
		_castVolumetricShadow = volumetricShadows;
	}

	void BaseLightComponent::setSubsurfaceTransmission(bool subsurfaceTransmission)
	{
		_subsurfaceTransmission = subsurfaceTransmission;
	}

	bool BaseLightComponent::isStaticLight() const
	{
		Actor* owner = getOwner();

		return owner && (getMobility() == Mobility::Static);
	}

	/*BaseLightComponent::Properties::Properties() :
		SubsurfaceTransmission(false),
		CastVolumetricShadow(false),
		CastDynamicShadows(false),
		CastStaticShadows(false),
		IndirectLightingIntensity(0.0),
		LightColor(255, 255, 255),
		Intensity(Math::PI),
		VolumetricScatteringIntensity(0.0)
	{
	}*/

	Resource::Type<BaseLightComponentResource> baseLightComponentResource("baseLightComponent");
}
