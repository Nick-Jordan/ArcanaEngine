#include "SkyboxActor.h"

#include "GlobalShaders.h"

namespace Arcana
{
	StaticMesh* SkyboxActor::__skyboxMesh = nullptr;
	uint32 SkyboxActor::__instances = 0;

	SkyboxActor::SkyboxActor() //name?
		: Actor(), _skyboxTexture(nullptr), _technique(nullptr), _emissiveThreshold(0.0f)
	{
	}

	SkyboxActor::SkyboxActor(Texture* skyboxTexture, float emissiveThreshold) //name?
		: Actor("skybox"), _skyboxTexture(skyboxTexture), _technique(nullptr), _emissiveThreshold(emissiveThreshold)
	{
		AE_REFERENCE(_skyboxTexture);
	}

	SkyboxActor::~SkyboxActor()
	{
		AE_RELEASE(_skyboxTexture);
		AE_RELEASE(_technique);

		__instances--;
		if (__instances == 0)
		{
			AE_RELEASE(__skyboxMesh);
		}
	}

	void SkyboxActor::initialize(std::string name, const Actor* templateActor)
	{
		Actor::initialize(name, templateActor);

		if (__instances == 0)
		{
			StaticMesh::Properties propertiesSkybox;
			propertiesSkybox.isEnvironmentMesh = false;
			propertiesSkybox.isTransparent = false;
			propertiesSkybox.isBackgroundSkybox = true;
			propertiesSkybox.LightMapResolution = 0;
			propertiesSkybox.LightProperties.CastsDynamicShadow = false;
			propertiesSkybox.RenderState.setDepthTestEnabled(true);
			propertiesSkybox.RenderState.setDepthFunction(RenderState::DepthFunction::LEqual);
			__skyboxMesh = new StaticMesh("resources/cube.mesh", propertiesSkybox);
			__skyboxMesh->reference();
		}

		__instances++;
	}

	void SkyboxActor::destroyed()
	{
	}

	void SkyboxActor::begin()
	{
		Shader skyboxShader = *GlobalShaders::get(GlobalShaders::BackgroundSkybox);

		Material* skyboxMaterial = new Material("skyboxMaterial");
		_technique = new Technique(skyboxShader);
		_technique->reference();
		skyboxMaterial->addTechnique(_technique);
		if (_skyboxTexture)
		{
			_technique->addAttribute("u_SkyboxTexture", _skyboxTexture);
			_technique->addAttribute("u_EmissiveThreshold", _emissiveThreshold);
		}
		addComponent(new StaticMeshComponent(__skyboxMesh, skyboxMaterial));
	}

	Texture* SkyboxActor::getTexture() const
	{
		return _skyboxTexture;
	}

	float SkyboxActor::getEmissiveThreshold() const
	{
		return _emissiveThreshold;
	}

	void SkyboxActor::setTexture(Texture* skyboxTexture)
	{
		if (_skyboxTexture == skyboxTexture)
			return;

		AE_RELEASE(_skyboxTexture);

		_skyboxTexture = skyboxTexture;
		
		if(_technique)
			_technique->removeAttribute("u_SkyboxTexture");
		
		if (_skyboxTexture)
		{
			_skyboxTexture->reference();
			if (_technique)
				_technique->addAttribute("u_SkyboxTexture", _skyboxTexture);
		}
	}

	void SkyboxActor::setEmissiveThreshold(float emissiveThreshold)
	{
		if (_emissiveThreshold == emissiveThreshold)
			return;

		_emissiveThreshold = emissiveThreshold;

		if (_technique)
		{
			_technique->removeAttribute("u_EmissiveThreshold");
			_technique->addAttribute("u_EmissiveThreshold", emissiveThreshold);
		}
	}
}
