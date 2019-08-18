#ifndef TRANSPARENT_ENVIRONMENT_STAGE_H_
#define TRANSPARENT_ENVIRONMENT_STAGE_H_

#include "GraphicsDefines.h"

#include "RenderStage.h"

namespace Arcana
{
	class ARCANA_GRAPHICS_API TransparentEnvironmentStage : public RenderStage
	{
	public:

		TransparentEnvironmentStage();

		~TransparentEnvironmentStage();

		virtual void initialize() override;

		virtual void finalize() override;

		virtual void render(const RenderData& data) override;
	};
}

#endif // !TRANSPARENT_ENVIRONMENT_STAGE_H_

