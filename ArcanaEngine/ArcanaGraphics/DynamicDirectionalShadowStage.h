#ifndef DYNAMIC_DIRECTIONAL_SHADOW_STAGE_H_
#define DYNAMIC_DIRECTIONAL_SHADOW_STAGE_H_

#include "GraphicsDefines.h"

#include "RenderStage.h"

#include "Framebuffer.h"
#include "Shader.h"

namespace Arcana
{
	struct ARCANA_GRAPHICS_API DirectionalShadow
	{
		Texture* depthMap;
		Matrix4f lightSpaceMatrix;
		Vector3f direction;
	};

	class ARCANA_GRAPHICS_API DynamicDirectionalShadowStage : public RenderStage
	{
	public:

		DynamicDirectionalShadowStage();

		~DynamicDirectionalShadowStage();

		virtual void initialize() override;

		virtual void finalize() override;

		virtual void render(const RenderData& data) override;

		const uint32 getNumDirectionalShadows() const;

		const std::map<UUID, DirectionalShadow>& getDirectionalShadows() const;

	private:

		Framebuffer* createDepthFramebuffer(UUID id);

		Shader _depthShader;

		uint32 _numDirectionalShadows;

		std::map<UUID, Framebuffer*> _depthFramebuffers;
		std::map<UUID, bool> _shadowUpdate;
		std::map<UUID, DirectionalShadow> _shadows;

	private:

		static const uint32 SHADOW_WIDTH;
		static const uint32 SHADOW_HEIGHT;
	};
}

#endif // !DYNAMIC_DIRECTIONAL_SHADOW_STAGE_H_

