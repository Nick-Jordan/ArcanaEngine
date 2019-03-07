#ifndef GEOMETRY_COMPONENT_H_
#define GEOMETRY_COMPONENT_H_

#include "SceneDefines.h"

#include "SceneComponent.h"
#include "RenderProcedure.h"
#include "FTLResult.h"
#include "LightProperties.h"

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

		virtual void updateRenderData(Matrix4d view, Matrix4d projection, Vector3d eyePosition);

		RenderProcedure* getRenderProcedure() const;

		void render(ObjectRenderer& renderer, Matrix4d view, Matrix4d projection, Vector3d eyePosition);

	protected:

		RenderProcedure* _renderProcedure;
	public:///FIX
		FTLResult _ftlResult;

		//FIX
		LightProperties _lightProperties;
	};

}

#endif // !GEOMETRY_COMPONENT_H_

