#ifndef DEFERRED_DECAL_STAGE_H_
#define DEFERRED_DECAL_STAGE_H_

#include "GraphicsDefines.h"

#include "RenderStage.h"

#include "Texture.h"

namespace Arcana
{
	class ARCANA_GRAPHICS_API DeferredDecalStage : public RenderStage
	{
	public:

		DeferredDecalStage();

		~DeferredDecalStage();

		virtual void initialize() override;

		virtual void finalize() override;

		virtual void render(const RenderData& data) override;

		void useDepthTexture(Texture* texture);

		void useTexture(std::string s, Texture* texture);

	private:

		std::vector<KeyValuePair<std::string, Texture*>> _textures;
	};
}

#endif // !DEFERRED_DECAL_STAGE_H_