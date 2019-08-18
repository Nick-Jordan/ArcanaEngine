#ifndef POST_PROCESSING_STAGE_H_
#define POST_PROCESSING_STAGE_H_

#include "GraphicsDefines.h"

#include "RenderStage.h"
#include "Framebuffer.h"

#include "PostProcessEffect.h"

#include <vector>

namespace Arcana
{
	class ARCANA_GRAPHICS_API PostProcessingStage : public RenderStage
	{
	public:

		PostProcessingStage();

		~PostProcessingStage();

		virtual void initialize() override;

		virtual void finalize() override;

		virtual void render(const RenderData& data) override;

		void useInitialTexture(Texture* texture);

	private:

		Texture* _initialTexture;

		Texture* _framebufferTextures[2];
		Framebuffer* _framebuffers[2];

		Shader _finalShader;
	};
}

#endif // !POST_PROCESSING_STAGE_H_

