#include "DecalComponent.h"

#include "DecalRenderProcedure.h"

namespace Arcana
{
	DecalProperties::DecalProperties() : 
		_normals(nullptr),
		_albedo(nullptr),
		_specular(nullptr),
		_roughness(nullptr),
		_emissive(nullptr),
		_metallic(nullptr),
		_ao(nullptr),
		_normalsBlend(0.0f),
		_albedoBlend(0.0f),
		_specularBlend(0.0f),
		_roughnessBlend(0.0f),
		_emissiveBlend(0.0f),
		_metallicBlend(0.0f),
		_aoBlend(0.0f),
		_angleCutoff(Math::PI),
		_color(Color(255, 255, 255, 255))
	{
	}

	DecalProperties::DecalProperties(const DecalProperties& copy) :
		_normals(copy._normals),
		_albedo(copy._albedo),
		_specular(copy._specular),
		_roughness(copy._roughness),
		_emissive(copy._emissive),
		_metallic(copy._metallic),
		_ao(copy._ao),
		_normalsBlend(copy._normalsBlend),
		_albedoBlend(copy._albedoBlend),
		_specularBlend(copy._specularBlend),
		_roughnessBlend(copy._roughnessBlend),
		_emissiveBlend(copy._emissiveBlend),
		_metallicBlend(copy._metallicBlend),
		_aoBlend(copy._aoBlend),
		_angleCutoff(copy._angleCutoff),
		_color(copy._color)
	{
	}

	DecalProperties::~DecalProperties()
	{
		AE_RELEASE(_normals);
		AE_RELEASE(_albedo);
		AE_RELEASE(_specular);
		AE_RELEASE(_roughness);
		AE_RELEASE(_emissive);
		AE_RELEASE(_metallic);
		AE_RELEASE(_ao);
	}

	void DecalProperties::setNormalsTexture(Texture* texture, float blend)
	{
		if (_normals == texture && _normalsBlend == blend)
			return;

		AE_RELEASE(_normals);
		_normals = texture;
		
		if (_normals)
		{
			_normals->reference();
			_normalsBlend = blend;
		}
		else
		{
			_normalsBlend = 0.0f;
		}
	}
	void DecalProperties::setAlbedoTexture(Texture* texture, float blend)
	{
		if (_albedo == texture && _albedoBlend == blend)
			return;

		AE_RELEASE(_albedo);
		_albedo = texture;

		if (_albedo)
		{
			_albedo->reference();
			_albedoBlend = blend;
		}
		else
		{
			_albedoBlend = 0.0f;
		}
	}
	void DecalProperties::setSpecularTexture(Texture* texture, float blend)
	{
		if (_specular == texture && _specularBlend == blend)
			return;

		AE_RELEASE(_specular);
		_specular = texture;

		if (_specular)
		{
			_specular->reference();
			_specularBlend = blend;
		}
		else
		{
			_specularBlend = 0.0f;
		}
	}
	void DecalProperties::setRoughnessTexture(Texture* texture, float blend)
	{
		if (_roughness == texture && _roughnessBlend == blend)
			return;

		AE_RELEASE(_roughness);
		_roughness = texture;

		if (_roughness)
		{
			_roughness->reference();
			_roughnessBlend = blend;
		}
		else
		{
			_roughnessBlend = 0.0f;
		}
	}
	void DecalProperties::setEmissiveTexture(Texture* texture, float blend)
	{
		if (_emissive == texture && _emissiveBlend == blend)
			return;

		AE_RELEASE(_emissive);
		_emissive = texture;

		if (_emissive)
		{
			_emissive->reference();
			_emissiveBlend = blend;
		}
		else
		{
			_emissiveBlend = 0.0f;
		}
	}
	void DecalProperties::setMetallicTexture(Texture* texture, float blend)
	{
		if (_metallic == texture && _metallicBlend == blend)
			return;

		AE_RELEASE(_metallic);
		_metallic = texture;

		if (_metallic)
		{
			_metallic->reference();
			_metallicBlend = blend;
		}
		else
		{
			_metallicBlend = 0.0f;
		}
	}
	void DecalProperties::setAmbientOcclusionTexture(Texture* texture, float blend)
	{
		if (_ao == texture && _aoBlend == blend)
			return;

		AE_RELEASE(_ao);
		_ao = texture;

		if (_ao)
		{
			_ao->reference();
			_aoBlend = blend;
		}
		else
		{
			_aoBlend = 0.0f;
		}
	}

	void DecalProperties::setNormalsBlendFactor(float blend)
	{
		_normalsBlend = blend;
	}
	void DecalProperties::setAlbedoBlendFactor(float blend)
	{
		_albedoBlend = blend;
	}
	void DecalProperties::setSpecularBlendFactor(float blend)
	{
		_specularBlend = blend;
	}
	void DecalProperties::setRoughnessBlendFactor(float blend)
	{
		_roughnessBlend = blend;
	}
	void DecalProperties::setEmissiveBlendFactor(float blend)
	{
		_emissiveBlend = blend;
	}
	void DecalProperties::setMetallicBlendFactor(float blend)
	{
		_metallicBlend = blend;
	}
	void DecalProperties::setAmbiendOcclusionBlendFactor(float blend)
	{
		_aoBlend = blend;
	}

	void DecalProperties::setAngleCutoff(float angleCutoff)
	{
		_angleCutoff = angleCutoff;
	}

	void DecalProperties::setColor(const Color& color)
	{
		_color = color;
	}

	DecalProperties& DecalProperties::operator=(const DecalProperties& rhs)
	{
		setNormalsTexture(rhs._normals, rhs._normalsBlend);
		setAlbedoTexture(rhs._albedo, rhs._albedoBlend);
		setSpecularTexture(rhs._specular, rhs._specularBlend);
		setRoughnessTexture(rhs._roughness, rhs._roughnessBlend);
		setEmissiveTexture(rhs._emissive, rhs._emissiveBlend);
		setMetallicTexture(rhs._metallic, rhs._metallicBlend);
		setAmbientOcclusionTexture(rhs._ao, rhs._aoBlend);

		_angleCutoff = rhs._angleCutoff;
		_color = rhs._color;

		return *this;
	}


	DecalComponent::DecalComponent()
	{
		initialize();
	}

	DecalComponent::DecalComponent(const DecalProperties& properties, uint32 maxDecals) : _decals(nullptr)
	{
		initialize(properties, maxDecals);
	}

	DecalComponent::~DecalComponent()
	{
		AE_DELETE_ARRAY(_decals);
	}

	void DecalComponent::initialize(const DecalProperties& properties, uint32 maxDecals)
	{
		setMaxDecals(maxDecals);
		_properties = properties;
		_numDecals = 0;

		initialize();
	}

	void DecalComponent::initialize()
	{
		GeometryComponent::initialize();
	}

	bool DecalComponent::createRenderProcedure()
	{
		_decalRenderProcedure = new DecalRenderProcedure(_properties);
		_decalRenderProcedure->reference();

		_renderProcedure = _decalRenderProcedure;

		return true;
	}

	void DecalComponent::update(double elapsedTime)
	{
		for (uint32 i = 0; i < _numDecals; i++)
		{
			Decal* decal = &_decals[i];

			decal->_currentTime += elapsedTime;

			if (decal->_lifetime != -1.0 && decal->_currentTime >= decal->_lifetime)
			{
				//fix
				if (i != i - 1)
				{
					_decals[i] = _decals[i - 1];
				}
				_numDecals = _numDecals - 1;
			}
		}
	}

	void DecalComponent::updateRenderProcedure()
	{
		_decalRenderProcedure->Decals = _decals;
		_decalRenderProcedure->NumDecals = _numDecals;
	}

	void DecalComponent::setMaxDecals(uint32 maxDecals)
	{
		if (_maxDecals != maxDecals)
		{
			_maxDecals = maxDecals;
			AE_DELETE_ARRAY(_decals);

			if (_maxDecals > 0)
			{
				_decals = new Decal[_maxDecals];
			}
		}
	}

	uint32 DecalComponent::getMaxDecals() const
	{
		return _maxDecals;
	}

	uint32 DecalComponent::getNumDecals() const
	{
		return _numDecals;
	}

	void DecalComponent::addDecal(const Transform& transform, double lifetime, Vector2f texCoordMin, Vector2f texCoordMax)
	{
		_numDecals++;
		if (_numDecals > _maxDecals)
		{
			_numDecals = _maxDecals;
			//change decal array index??
		}

		_decals[_numDecals - 1]._currentTime = 0.0;
		_decals[_numDecals - 1]._transform = transform;
		_decals[_numDecals - 1]._lifetime = lifetime;
		_decals[_numDecals - 1]._texCoords = Vector4f(texCoordMin.x, texCoordMin.y, texCoordMax.x, texCoordMax.y);
	}


	Decal::Decal() : _lifetime(-1.0), _currentTime(0.0), _opacity(1.0), _texCoords(0.0, 0.0, 1.0, 1.0)
	{

	}

	Decal::~Decal()
	{

	}

	double Decal::getOpacity() const
	{
		return _opacity * (_lifetime == -1.0 ? 1.0 : Math::range(_currentTime, 0.0, _lifetime, 1.0, 0.0));
	}

	const Transform& Decal::getTransform() const
	{
		return _transform;
	}

	const Color& Decal::getColor() const
	{
		return _color;
	}

	Vector4f Decal::getTexCoords() const
	{
		return _texCoords;
	}
}