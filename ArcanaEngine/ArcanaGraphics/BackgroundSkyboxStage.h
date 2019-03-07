#ifndef BACKGROUND_SKYBOX_STAGE_H_
#define BACKGROUND_SKYBOX_STAGE_H_

#include "GraphicsDefines.h"

#include "RenderStage.h"

namespace Arcana
{
	class ARCANA_GRAPHICS_API BackgroundSkyboxStage : public RenderStage
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

