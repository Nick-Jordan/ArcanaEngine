#ifndef TRANSPARENT_ENVIRONMENT_STAGE_H_
#define TRANSPARENT_ENVIRONMENT_STAGE_H_

#include "RenderStage.h"

namespace Arcana
{
	class TransparentEnvironmentStage : public RenderStage
	{
	public:

		TransparentEnvironmentStage();

		~TransparentEnvironmentStage();

		virtual void initialize() override;

		virtual void finalize() override;

		virtual void render() override;
	};
}

#endif // !TRANSPARENT_ENVIRONMENT_STAGE_H_

