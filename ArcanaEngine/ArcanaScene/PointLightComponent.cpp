#include "PointLightComponent.h"

namespace Arcana
{

	PointLightComponent::PointLightComponent()
		: _constantAttenuation(0.0f),
		_linearAttenuation(0.0f),
		_quadraticAttenuation(1.0f),
		_sourceRadius(0.0f),
		_softSourceRadius(0.0f)
	{
	}


	PointLightComponent::~PointLightComponent()
	{
	}

	LightType PointLightComponent::getLightType() const
	{
		return Point;
	}

	Vector4d PointLightComponent::getLightPosition()
	{
		return Vector4d(getWorldPosition(), 1.0);
	}

	void PointLightComponent::useInverseSquareFalloff()
	{
		_constantAttenuation = 0.0f;
		_linearAttenuation = 0.0f;
		_quadraticAttenuation = 1.0f;
	}

	bool PointLightComponent::isUsingInverseSquareFalloff() const
	{
		return Math::closeEnough(_constantAttenuation, 0.0) 
			&& Math::closeEnough(_linearAttenuation, 0.0) 
			&& Math::closeEnough(_quadraticAttenuation, 1.0);
	}

	void PointLightComponent::setConstantAttenuation(float constant)
	{
		_constantAttenuation = constant;
	}

	float PointLightComponent::getConstantAttenuation() const
	{
		return _constantAttenuation;
	}

	void PointLightComponent::setLinearAttenuation(float linear)
	{
		_linearAttenuation = linear;
	}

	float PointLightComponent::getLinearAttenuation() const
	{
		return _linearAttenuation;
	}

	void PointLightComponent::setQuadraticAttenuation(float quadratic)
	{
		_quadraticAttenuation = quadratic;
	}

	float PointLightComponent::getQuadraticAttenuation() const
	{
		return _quadraticAttenuation;
	}

	void PointLightComponent::setSourceRadius(float radius)
	{
		_sourceRadius = radius;

		if (_sourceRadius > _softSourceRadius && _softSourceRadius > 0.0f)
		{
			_softSourceRadius = _sourceRadius;
		}
	}

	float PointLightComponent::getSourceRadius() const
	{
		return _sourceRadius;
	}

	void PointLightComponent::setSoftSourceRadius(float radius)
	{
		_softSourceRadius = radius;

		if (_softSourceRadius < _sourceRadius && _softSourceRadius > 0)
		{
			_sourceRadius = _softSourceRadius;
		}
	}

	float PointLightComponent::getSoftSourceRadius() const
	{
		return _softSourceRadius;
	}

	RenderLight PointLightComponent::createRenderLight()
	{
		RenderLight light;

		Vector3d position = getLightPosition().xyz();

		light.position = position.cast<float>();
		light.color = getLightColor().toVector3();
		light.intensity = getIntensity();
		light.sourceRadius = _sourceRadius;
		light.softSourceRadius = _softSourceRadius;
		light.type = Point;
		light.constant = _constantAttenuation;
		light.linear = _linearAttenuation;
		light.quadratic = _quadraticAttenuation;
		light.mobility = isStaticLight();
		light.id = getLightId().getId();

		light.lightProperties.CastsDynamicShadow = hasDynamicShadows();

		return light;
	}
}
