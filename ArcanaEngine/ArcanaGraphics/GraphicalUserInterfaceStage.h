#ifndef GRAPHICAL_USER_INTERFACE_STAGE_H_
#define GRAPHICAL_USER_INTERFACE_STAGE_H_

#include "GraphicsDefines.h"

#include "RenderStage.h"

namespace Arcana
{
	class ARCANA_GRAPHICS_API GraphicalUserInterfaceStage : public RenderStage
	{
	public:

		GraphicalUserInterfaceStage();

		~GraphicalUserInterfaceStage();

		virtual void initialize() override;

		virtual void finalize() override;

		virtual void render() override;
	};
}

#endif // !GRAPHICAL_USER_INTERFACE_STAGE_H_

