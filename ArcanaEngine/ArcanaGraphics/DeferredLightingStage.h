#ifndef DEFERRED_LIGHTING_STAGE_H_
#define DEFERRED_LIGHTING_STAGE_H_

#include "GraphicsDefines.h"

#include "RenderStage.h"

#include "Texture.h"
#include "DynamicDirectionalShadowStage.h"
#include "DynamicPointShadowStage.h"

namespace Arcana
{
	class ARCANA_GRAPHICS_API DeferredLightingStage : public RenderStage
	{
	public:

		DeferredLightingStage();

		~DeferredLightingStage();

		virtual void initialize() override;

		virtual void finalize() override;

		virtual void render(const RenderData& data) override;

		void useGBufferTexture(const std::string& samplerName, Texture* texture);

		void passDirectionalShadows(DynamicDirectionalShadowStage& stage);

		void passPointShadows(DynamicPointShadowStage& stage);

	private:

		static void passDirectionalShadow(uint32 index, Shader& shader, const DirectionalShadow& shadow);

		static void passPointShadow(uint32 index, Shader& shader, const PointShadow& shadow);

	private:

		Shader _lightingShader;
		std::vector<KeyValuePair<std::string, Texture*>> _gbufferTextures;
	};
}

#endif // !DEFERRED_LIGHTING_STAGE_H_

