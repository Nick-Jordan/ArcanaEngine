#ifndef RENDER_PROCEDURE_H_
#define RENDER_PROCEDURE_H_

#include "GraphicsDefines.h"

#include "Object.h"
#include "ObjectRenderer.h"
#include "FTLResult.h"
#include "LightProperties.h"

//test
#include "Matrix4.h"

namespace Arcana
{
	class ARCANA_GRAPHICS_API RenderDataUpdate
	{
	public:

		//test
		Matrix4d view;
		Matrix4d projection;
		Vector3d eyePosition;
		Transform transform;
		FTLResult ftlResult;
		LightProperties lightProperties;
	};

	class ARCANA_GRAPHICS_API RenderData : public Object
	{
	public:

		virtual ~RenderData() {};

		virtual void render(ObjectRenderer& renderer) = 0;
	};

	class ARCANA_GRAPHICS_API RenderProcedure : public Object
	{
	public:

		virtual ~RenderProcedure() {};

		virtual bool isDirty() const = 0;

		virtual void markDirty(bool dirty) = 0;

		virtual void createRenderData() = 0;

		virtual void updateRenderData(const RenderDataUpdate& data) = 0;

		virtual RenderData* getRenderData() const = 0;

		virtual bool isValidProcedure() = 0;
	};

}

#endif // !RENDER_PROCEDURE_H_

