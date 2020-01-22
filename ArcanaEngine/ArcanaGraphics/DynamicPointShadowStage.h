#ifndef DYNAMIC_POINT_SHADOW_STAGE_H_
#define DYNAMIC_POINT_SHADOW_STAGE_H_

#include "GraphicsDefines.h"

#include "RenderStage.h"

#include "Framebuffer.h"
#include "Shader.h"

namespace Arcana
{
#define MAX_POINT_SHADOWS 16 //test

	struct ARCANA_GRAPHICS_API PointShadow
	{
		Texture* depthMap;
		Vector3f position;

		PointShadow() : depthMap(nullptr) {}
	};

	class ARCANA_GRAPHICS_API DynamicPointShadowStage : public RenderStage
	{
	public:

		DynamicPointShadowStage();

		~DynamicPointShadowStage();

		virtual void initialize() override;

		virtual void finalize() override;

		virtual void render(const RenderData& data) override;

		const uint32 getNumPointShadows() const;

		const std::map<UUID, PointShadow>& getPointShadows() const;

	private:

		Framebuffer* createDepthFramebuffer(UUID id);

		Shader _depthShader;

		uint32 _numPointShadows;

		std::map<UUID, Framebuffer*> _depthFramebuffers;
		std::map<UUID, bool> _shadowUpdate;
		std::map<UUID, PointShadow> _shadows;

	private:

		static const uint32 SHADOW_WIDTH;
		static const uint32 SHADOW_HEIGHT;
	};
}

#endif // !DYNAMIC_POINT_SHADOW_STAGE_H_

