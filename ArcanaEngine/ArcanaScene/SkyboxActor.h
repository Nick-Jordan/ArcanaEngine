#ifndef SKYBOX_ACTOR_H_
#define SKYBOX_ACTOR_H_

#include "SceneDefines.h"

#include "Actor.h"
#include "StaticMeshComponent.h"

namespace Arcana
{
	class ARCANA_SCENE_API SkyboxActor : public Actor
	{
	public:

		SkyboxActor();

		SkyboxActor(Texture* skyboxTexture, float emissiveThreshold);

		~SkyboxActor();

		virtual void initialize(std::string name, const Actor* templateActor = nullptr) override;

		virtual void destroyed() override;

		virtual void begin() override;

		Texture* getTexture() const;

		float getEmissiveThreshold() const;

		void setTexture(Texture* skyboxTexture);

		void setEmissiveThreshold(float emissiveThreshold);


	private:

		static StaticMesh* __skyboxMesh;
		static uint32 __instances;

	private:

		Texture* _skyboxTexture;
		float _emissiveThreshold;
		bool _equirectangular;

		Technique* _technique;
	};
}

#endif // !SKYBOX_ACTOR_H_

