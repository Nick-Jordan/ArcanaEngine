#ifndef DYNAMIC_POINT_SHADOW_STAGE_H_
#define DYNAMIC_POINT_SHADOW_STAGE_H_

#include "GraphicsDefines.h"

#include "RenderStage.h"

#include "Framebuffer.h"

namespace Arcana
{
	struct ARCANA_GRAPHICS_API PointShadow
	{
		Texture* depthMap;
		Matrix4f lightSpaceMatrix;
		Vector3f position;
	};

	class ARCANA_GRAPHICS_API DynamicPointShadowStage : public RenderStage
	{
	public:

		DynamicPointShadowStage();

		~DynamicPointShadowStage();

		virtual void initialize() override;

		virtual void finalize() override;

		virtual void render() override;

		//test
		PointShadow shadow;

	private:

		Shader _depthShader;

		Framebuffer* _depthFramebuffer;

	private:

		static const uint32 SHADOW_WIDTH;
		static const uint32 SHADOW_HEIGHT;
	};
}

#endif // !DYNAMIC_POINT_SHADOW_STAGE_H_

