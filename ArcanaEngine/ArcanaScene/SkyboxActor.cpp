#include "SkyboxActor.h"

#include "GlobalShaders.h"
#include "ResourceManager.h"
#include "ResourceCreator.h"

namespace Arcana
{
	StaticMesh* SkyboxActor::__skyboxMesh = nullptr;
	uint32 SkyboxActor::__instances = 0;

	SkyboxActor::SkyboxActor() //name?
		: Actor(), _skyboxTexture(nullptr), _technique(nullptr), _emissiveThreshold(0.0f)
	{
	}

	SkyboxActor::SkyboxActor(Texture* skyboxTexture, float emissiveThreshold) //name?
		: Actor("skybox"), _skyboxTexture(skyboxTexture), _technique(nullptr), _emissiveThreshold(emissiveThreshold), _equirectangular(false)
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

		Shader skyboxShader = *GlobalShaders::get(GlobalShaders::BackgroundSkybox);

		Material* skyboxMaterial = new Material("skyboxMaterial");
		_technique = new Technique(skyboxShader);
		_technique->reference();
		skyboxMaterial->addTechnique(_technique);
		if (_skyboxTexture)
		{
			if (_skyboxTexture->getType() == Texture::Texture2D)
			{
				_technique->addAttribute("u_SkyboxTextureEquirectangular", _skyboxTexture);
			}
			else
			{
				_technique->addAttribute("u_SkyboxTexture", _skyboxTexture);
			}
		}

		_technique->addAttribute("u_EmissiveThreshold", _emissiveThreshold);
		_technique->addAttribute("u_Equirectangular", _equirectangular);

		addComponent(new StaticMeshComponent(__skyboxMesh, skyboxMaterial));
	}

	void SkyboxActor::destroyed()
	{
	}

	void SkyboxActor::begin()
	{

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

		if (_technique && _skyboxTexture)
		{
			if (_skyboxTexture->getType() == Texture::Texture2D)
			{
				_technique->removeAttribute("u_SkyboxTextureEquirectangular");
			}
			else
			{
				_technique->removeAttribute("u_SkyboxTexture");
			}

			_technique->removeAttribute("u_Equirectangular");
		}

		AE_RELEASE(_skyboxTexture);

		_skyboxTexture = skyboxTexture;

		if (_skyboxTexture)
		{
			_skyboxTexture->reference();
			if (_technique)
			{
				if (_skyboxTexture->getType() == Texture::Texture2D)
				{
					_equirectangular = true;
					_technique->addAttribute("u_SkyboxTextureEquirectangular", _skyboxTexture);
				}
				else
				{
					_equirectangular = false;
					_technique->addAttribute("u_SkyboxTexture", _skyboxTexture);
				}
				_technique->addAttribute("u_Equirectangular", _equirectangular);
			}
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

	class SkyboxActorResource : public ResourceCreator<SkyboxActor>
	{
	public:

		SkyboxActorResource(const GlobalObjectID& id, const std::string& type, const ResourceData& data, Scheduler* dependencyScheduler)
			: ResourceCreator<SkyboxActor>(id, type, data, dependencyScheduler)
		{
			this->id = id;

			textureTask = ResourceManager::instance().loadResource<Texture>(data.getResourceDependency("texture"), dependencyScheduler);
			if (textureTask)
			{
				textureTask->wait();
			}
			else
			{
				/*const ResourceData* textureData = data.getAdditionalData("texture");

				if (textureData)
				{
					textureTask = ResourceManager::instance().buildResource<Texture>(GlobalObjectID(id.getName() + "::texture"), "textureCube", *textureData, dependencyScheduler);
					if (textureTask)
					{
						textureTask->wait();
					}
				}*/
			}

			setEmissiveThreshold(data.getFloatParameter("emissiveThreshold", 1.0f));
		}

		virtual void syncInitialize() override
		{
			initialize(id.getName());

			if (textureTask)
			{
				Texture* t = textureTask->get();
				setTexture(t);
			}
		}

	private:

		LoadResourceTask<Texture>* textureTask;
		GlobalObjectID id;
	};

	Resource::Type<SkyboxActorResource, true> skyboxActorResource("skyboxActor");
}
