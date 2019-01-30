#ifndef OPAQUE_ENVIRONMENT_STAGE_H_
#define OPAQUE_ENVIRONMENT_STAGE_H_

#include "RenderStage.h"

namespace Arcana
{
	class OpaqueEnvironmentStage : public RenderStage
	{
	public:

		OpaqueEnvironmentStage();

		~OpaqueEnvironmentStage();

		virtual void initialize() override;

		virtual void finalize() override;

		virtual void render() override;
	};
}

#endif // !OPAQUE_ENVIRONMENT_STAGE_H_

