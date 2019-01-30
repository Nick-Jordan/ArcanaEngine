#ifndef DYNAMIC_DIRECTIONAL_SHADOW_STAGE_H_
#define DYNAMIC_DIRECTIONAL_SHADOW_STAGE_H_

#include "RenderStage.h"

#include "Framebuffer.h"

namespace Arcana
{
	struct DirectionalShadow
	{
		Texture* depthMap;
		Matrix4f lightSpaceMatrix;
		Vector3f position;
	};

	class DynamicDirectionalShadowStage : public RenderStage
	{
	public:

		DynamicDirectionalShadowStage();

		~DynamicDirectionalShadowStage();

		virtual void initialize() override;

		virtual void finalize() override;

		virtual void render() override;

		//test
		DirectionalShadow shadow;

	private:

		Shader _depthShader;
	
		Framebuffer* _depthFramebuffer;

	private:

		static const uint32 SHADOW_WIDTH;
		static const uint32 SHADOW_HEIGHT;
	};
}

#endif // !DYNAMIC_DIRECTIONAL_SHADOW_STAGE_H_

