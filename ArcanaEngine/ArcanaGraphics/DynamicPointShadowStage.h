#ifndef DYNAMIC_POINT_SHADOW_STAGE_H_
#define DYNAMIC_POINT_SHADOW_STAGE_H_

#include "RenderStage.h"

#include "Framebuffer.h"

namespace Arcana
{
	struct PointShadow
	{
		Texture* depthMap;
		Matrix4f lightSpaceMatrix;
		Vector3f position;
	};

	class DynamicPointShadowStage : public RenderStage
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

