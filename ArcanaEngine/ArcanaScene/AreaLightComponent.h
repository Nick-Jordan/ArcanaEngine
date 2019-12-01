#ifndef AREA_LIGHT_COMPONENT_H_
#define AREA_LIGHT_COMPONENT_H_

#include "SceneDefines.h"

#include "LocalLightComponent.h"
#include "Texture.h"

namespace Arcana
{
	class ARCANA_SCENE_API AreaLightComponent : public LocalLightComponent
	{
	public:

		AreaLightComponent();

		virtual ~AreaLightComponent();

		virtual LightType getLightType() const override;

		virtual RenderLight createRenderLight() override;

		virtual Vector4d getLightPosition() override;

	private:

		float _sourceHeight;
		Texture* _sourceTexture;
		float _sourceWidth;
	};
}
#endif // !AREA_LIGHT_COMPONENT_H_
