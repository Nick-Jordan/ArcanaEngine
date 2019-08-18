#ifndef OPAQUE_ENVIRONMENT_STAGE_H_
#define OPAQUE_ENVIRONMENT_STAGE_H_

#include "GraphicsDefines.h"

#include "RenderStage.h"

namespace Arcana
{
	class ARCANA_GRAPHICS_API OpaqueEnvironmentStage : public RenderStage
	{
	public:

		OpaqueEnvironmentStage();

		~OpaqueEnvironmentStage();

		virtual void initialize() override;

		virtual void finalize() override;

		virtual void render(const RenderData& data) override;
	};
}

#endif // !OPAQUE_ENVIRONMENT_STAGE_H_

