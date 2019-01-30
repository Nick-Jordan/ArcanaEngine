#ifndef BACKGROUND_SKYBOX_STAGE_H_
#define BACKGROUND_SKYBOX_STAGE_H_

#include "RenderStage.h"

namespace Arcana
{
	class BackgroundSkyboxStage : public RenderStage
	{
	public:

		BackgroundSkyboxStage();

		~BackgroundSkyboxStage();

		virtual void initialize() override;

		virtual void finalize() override;

		virtual void render() override;
	};
}

#endif // !BACKGROUND_SKYBOX_STAGE_H_

