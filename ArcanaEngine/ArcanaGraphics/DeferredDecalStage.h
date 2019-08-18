#ifndef DEFERRED_DECAL_STAGE_H_
#define DEFERRED_DECAL_STAGE_H_

#include "GraphicsDefines.h"

#include "RenderStage.h"

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
	};
}

#endif // !DEFERRED_DECAL_STAGE_H_