#ifndef SPOT_LIGHT_COMPONENT_H_
#define SPOT_LIGHT_COMPONENT_H_

#include "SceneDefines.h"

#include "PointLightComponent.h"

namespace Arcana
{
	class ARCANA_SCENE_API SpotLightComponent : public PointLightComponent
	{
	public:

		SpotLightComponent();

		virtual ~SpotLightComponent();

		virtual LightType getLightType() const override;

	private:

		float _innerConeAngle;
		float _lightShaftConeAngle;
		float _outerConeAngle;
	};
}
#endif // !SPOT_LIGHT_COMPONENT_H_
