#ifndef RECT_LIGHT_COMPONENT_H_
#define RECT_LIGHT_COMPONENT_H_

#include "SceneDefines.h"

#include "LocalLightComponent.h"
#include "Texture.h"

namespace Arcana
{
	class ARCANA_SCENE_API RectLightComponent : public LocalLightComponent
	{
	public:

		RectLightComponent();

		virtual ~RectLightComponent();

		virtual LightType getLightType() const override;

	private:

		float _sourceHeight;
		Texture* _sourceTexture;
		float _sourceWidth;
	};
}
#endif // !RECT_LIGHT_COMPONENT_H_
