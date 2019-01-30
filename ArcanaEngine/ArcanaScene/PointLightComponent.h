#ifndef POINT_LIGHT_COMPONENT_H_
#define POINT_LIGHT_COMPONENT_H_

#include "SceneDefines.h"

#include "LocalLightComponent.h"

namespace Arcana
{
	class ARCANA_SCENE_API PointLightComponent : public LocalLightComponent
	{
	public:

		PointLightComponent();

		virtual ~PointLightComponent();

		virtual LightType getLightType() const override;

		virtual RenderLight createRenderLight() const override;

	private:

		bool _useInverseSquareFalloff;
		float _lightFalloffExponent;
		float _softSourceRadius;
		float _sourceLength;
		float _sourceRadius;
	};
}
#endif // !POINT_LIGHT_COMPONENT_H_
