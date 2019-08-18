#ifndef GEOMETRY_COMPONENT_H_
#define GEOMETRY_COMPONENT_H_

#include "SceneDefines.h"

#include "SceneComponent.h"
#include "RenderProcedure.h"
#include "FTLResult.h"
#include "LightProperties.h"
#include "ObjectRenderer.h"

namespace Arcana
{
	class ARCANA_SCENE_API GeometryComponent : public SceneComponent
	{
	public:

		GeometryComponent();

		virtual ~GeometryComponent();

		virtual void initialize() override;

		bool hasRenderProcedure() const;

		virtual bool createRenderProcedure();

		virtual void updateRenderProcedure();

		RenderProcedure* getRenderProcedure() const;

		void render(ObjectRenderer& renderer);

	protected:

		RenderProcedure* _renderProcedure;

		//FIX
		LightProperties _lightProperties;
	};

}

#endif // !GEOMETRY_COMPONENT_H_

